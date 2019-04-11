#include "Inject.h"

namespace Inject
{
	char szDllFilePath[MAX_PATH] = { 0 };
	char szCurrentDir[MAX_PATH] = { 0 };

	DWORD WINAPI LoadDllStub( PMANUAL_INJECT ManualInject )
	{
		if ( !ManualInject )
			return 0;

		HMODULE hModule = 0;
		ULONG_PTR Function = 0;

		PIMAGE_IMPORT_DESCRIPTOR pIID = nullptr;
		PIMAGE_IMPORT_BY_NAME pIBN = nullptr;
		PIMAGE_THUNK_DATA FirstThunk = nullptr , OrigFirstThunk = nullptr;

		PDLL_MAIN EntryPoint = nullptr;

		ULONG_PTR uiLibraryAddress , uiBaseAddress , uiValueA , uiValueB , uiValueC , uiValueD;

		uiBaseAddress = (ULONG_PTR)ManualInject->ImageBase;
		uiLibraryAddress = uiBaseAddress - ManualInject->NtHeaders->OptionalHeader.ImageBase;

		uiValueB = (ULONG_PTR)&ManualInject->NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

		if ( ( (PIMAGE_DATA_DIRECTORY)uiValueB )->Size )
		{
			uiValueC = ( uiBaseAddress + ( (PIMAGE_DATA_DIRECTORY)uiValueB )->VirtualAddress );

			while ( ( (PIMAGE_BASE_RELOCATION)uiValueC )->SizeOfBlock )
			{
				uiValueA = ( uiBaseAddress + ( (PIMAGE_BASE_RELOCATION)uiValueC )->VirtualAddress );

				uiValueB = ( ( (PIMAGE_BASE_RELOCATION)uiValueC )->SizeOfBlock - sizeof( IMAGE_BASE_RELOCATION ) ) / sizeof( IMAGE_RELOC );

				uiValueD = uiValueC + sizeof( IMAGE_BASE_RELOCATION );

				while ( uiValueB-- )
				{
					if ( ( (PIMAGE_RELOC)uiValueD )->type == IMAGE_REL_BASED_DIR64 )
						*(ULONG_PTR *)( uiValueA + ( (PIMAGE_RELOC)uiValueD )->offset ) += uiLibraryAddress;
					else if ( ( (PIMAGE_RELOC)uiValueD )->type == IMAGE_REL_BASED_HIGHLOW )
						*(DWORD *)( uiValueA + ( (PIMAGE_RELOC)uiValueD )->offset ) += (DWORD)uiLibraryAddress;
					else if ( ( (PIMAGE_RELOC)uiValueD )->type == IMAGE_REL_BASED_HIGH )
						*(WORD *)( uiValueA + ( (PIMAGE_RELOC)uiValueD )->offset ) += HIWORD( uiLibraryAddress );
					else if ( ( (PIMAGE_RELOC)uiValueD )->type == IMAGE_REL_BASED_LOW )
						*(WORD *)( uiValueA + ( (PIMAGE_RELOC)uiValueD )->offset ) += LOWORD( uiLibraryAddress );

					uiValueD += sizeof( IMAGE_RELOC );
				}

				uiValueC = uiValueC + ( (PIMAGE_BASE_RELOCATION)uiValueC )->SizeOfBlock;
			}
		}

		pIID = ManualInject->ImportDirectory;

		while ( pIID->Characteristics )
		{
			OrigFirstThunk = (PIMAGE_THUNK_DATA)( (ULONG_PTR)ManualInject->ImageBase + pIID->OriginalFirstThunk );
			FirstThunk = (PIMAGE_THUNK_DATA)( (ULONG_PTR)ManualInject->ImageBase + pIID->FirstThunk );

			hModule = ManualInject->fnGetModuleHandleA( (LPCSTR)ManualInject->ImageBase + pIID->Name );

			if ( !hModule )
			{
				hModule = ManualInject->fnLoadLibraryA( (LPCSTR)ManualInject->ImageBase + pIID->Name );

				if ( !hModule )
					return FALSE;
			}

			while ( OrigFirstThunk->u1.AddressOfData )
			{
				if ( OrigFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG )
				{
					Function = (ULONG_PTR)ManualInject->fnGetProcAddress( hModule , (LPCSTR)( OrigFirstThunk->u1.Ordinal & 0xFFFF ) );

					if ( !Function )
					{
						return FALSE;
					}

					FirstThunk->u1.Function = (ULONG_PTR)Function;
				}
				else
				{
					pIBN = (PIMAGE_IMPORT_BY_NAME)( (ULONG_PTR)ManualInject->ImageBase + OrigFirstThunk->u1.AddressOfData );
					Function = (ULONG_PTR)ManualInject->fnGetProcAddress( hModule , (LPCSTR)pIBN->Name );

					if ( !Function )
					{
						return FALSE;
					}

					FirstThunk->u1.Function = Function;
				}

				OrigFirstThunk++;
				FirstThunk++;
			}

			pIID++;
		}

		if ( ManualInject->NtHeaders->OptionalHeader.AddressOfEntryPoint )
		{
			/*for ( DWORD i = 0; i < 0x400; i++ )
			{
				*(PBYTE)( (DWORD)ManualInject->ImageBase + i ) = 0x00;
			}

			PIMAGE_DATA_DIRECTORY RelocDirectory = (PIMAGE_DATA_DIRECTORY)uiValueB;

			for ( DWORD i = 0; i < RelocDirectory->Size; i++ )
			{
				*(PBYTE)( (DWORD)uiValueB + i ) = 0x00;
			}*/

			EntryPoint = (PDLL_MAIN)( (ULONG_PTR)ManualInject->ImageBase + ManualInject->NtHeaders->OptionalHeader.AddressOfEntryPoint );
			EntryPoint( (HMODULE)ManualInject->ImageBase , DLL_PROCESS_ATTACH , ManualInject );

			return TRUE;
		}

		return FALSE;
	}

	void __declspec(naked) LoadDllEnd()
	{

	}

	bool FileExist( LPCTSTR fname )
	{
		return GetFileAttributesA( fname ) != INVALID_FILE_ATTRIBUTES;
	}

	bool GetDllInject()
	{
		GetModuleFileNameA( 0 , szDllFilePath , MAX_PATH );

		int len = lstrlenA( szDllFilePath );

		szDllFilePath[len - 1] = 'l';
		szDllFilePath[len - 2] = 'l';
		szDllFilePath[len - 3] = 'd';

		if ( FileExist( szDllFilePath ) )
			return true;

		return false;
	}

	bool InjectDll( DWORD dwPID )
	{
		PIMAGE_DOS_HEADER pIDH;
		PIMAGE_NT_HEADERS pINH;
		PIMAGE_SECTION_HEADER pISH;

		HANDLE hThread , hProcess , hDllFile;
		PVOID image = nullptr , mem = nullptr;
		DWORD ExitCode = 0;

		PVOID pDll;

		MANUAL_INJECT ManualInject;

		hDllFile = CreateFileA( szDllFilePath , GENERIC_READ , NULL , NULL , OPEN_ALWAYS , OPEN_EXISTING , NULL );

		if ( !hDllFile )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #2" ) , XorStr( "Error" ) , MB_ICONERROR );
			return false;
		}

		DWORD dwDllFileSize = GetFileSize( hDllFile , NULL );

		if ( dwDllFileSize == INVALID_FILE_SIZE || dwDllFileSize == 0 )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #3" ) , XorStr( "Error" ) , MB_ICONERROR );
			return false;
		}

		pDll = HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , dwDllFileSize );

		DWORD dwReadSize = 0;
		ReadFile( hDllFile , pDll , dwDllFileSize , &dwReadSize , false );

		if ( dwReadSize != dwDllFileSize )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #4" ) , XorStr( "Error" ) , MB_ICONERROR );
			return false;
		}

		pIDH = (PIMAGE_DOS_HEADER)pDll;
		pINH = (PIMAGE_NT_HEADERS)( (LPBYTE)pDll + pIDH->e_lfanew );

		hProcess = OpenProcess( PROCESS_ALL_ACCESS , TRUE , dwPID );

		if ( !hProcess )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #5" ) , XorStr( "Error" ) , MB_ICONERROR );
			CloseHandle( hDllFile );
			CloseHandle( hProcess );
			return false;
		}

#if DEBUG_INJECT == 1
		printf( "hProcess: %p\n" , hProcess );
#endif

		image = VirtualAllocEx( hProcess , NULL , pINH->OptionalHeader.SizeOfImage , MEM_COMMIT | MEM_RESERVE , PAGE_EXECUTE_READWRITE );

#if DEBUG_INJECT == 1
		printf( "ImageBase: %p\n" , image );
#endif

		if ( !image )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #6" ) , XorStr( "Error" ) , MB_ICONERROR );
			CloseHandle( hDllFile );
			CloseHandle( hProcess );
			return false;
		}

		if ( !WriteProcessMemory( hProcess , image , pDll , pINH->OptionalHeader.SizeOfHeaders , NULL ) )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #7" ) , XorStr( "Error" ) , MB_ICONERROR );
			VirtualFreeEx( hProcess , image , 0 , MEM_RELEASE );
			CloseHandle( hDllFile );
			CloseHandle( hProcess );
			return false;
		}

		pISH = (PIMAGE_SECTION_HEADER)( pINH + 1 );

		for ( DWORD i = 0; i < pINH->FileHeader.NumberOfSections; i++ )
		{
			WriteProcessMemory( hProcess , (PVOID)( (LPBYTE)image + pISH[i].VirtualAddress ) , (PVOID)( (LPBYTE)pDll + pISH[i].PointerToRawData ) , pISH[i].SizeOfRawData , NULL );
		}

		DWORD_PTR dwSizeStruct = sizeof( MANUAL_INJECT );
		DWORD_PTR dwSizeStub = (DWORD_PTR)LoadDllEnd - (DWORD_PTR)LoadDllStub;
		DWORD_PTR dwShellCodeSize = dwSizeStruct + dwSizeStub + 1;

		mem = VirtualAllocEx( hProcess , NULL , dwShellCodeSize , MEM_COMMIT | MEM_RESERVE , PAGE_EXECUTE_READWRITE );

#if DEBUG_INJECT == 1
		printf( "ShellCode: %p\n" , mem );
		printf( "LoadDllEnd: %p\n" , LoadDllEnd );
		printf( "LoadDllStub: %p\n" , LoadDllStub );
		printf( "dwSizeStub: %i\n" , dwSizeStub );
#endif

		if ( !mem )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #8" ) , XorStr( "Error" ) , MB_ICONERROR );
			VirtualFreeEx( hProcess , image , 0 , MEM_RELEASE );
			CloseHandle( hDllFile );
			CloseHandle( hProcess );
			return false;
		}

		ManualInject.ImageBase = image;
		ManualInject.ShellCode = mem;
		ManualInject.NtHeaders = (PIMAGE_NT_HEADERS)( (LPBYTE)image + pIDH->e_lfanew );
		ManualInject.ImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)( (LPBYTE)image + pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );
		ManualInject.fnLoadLibraryA = LoadLibraryA;
		ManualInject.fnGetModuleHandleA = GetModuleHandleA;
		ManualInject.fnGetProcAddress = GetProcAddress;

		memcpy( ManualInject.m_cheat_dir , szCurrentDir , MAX_PATH );

		ManualInject.m_cheat_key = DLL_LIC_KEY;

		bool bWriteStruct = WriteProcessMemory( hProcess , mem , &ManualInject , dwSizeStruct , NULL );

		if ( !bWriteStruct )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #9" ) , XorStr( "Error" ) , MB_ICONERROR );
			VirtualFreeEx( hProcess , mem , 0 , MEM_RELEASE );
			VirtualFreeEx( hProcess , image , 0 , MEM_RELEASE );
			CloseHandle( hDllFile );
			CloseHandle( hProcess );
			return false;
		}

		PVOID ShellEntry = (PVOID)( (DWORD_PTR)mem + dwSizeStruct + 1 );

		bool bWriteShell = WriteProcessMemory( hProcess , ShellEntry , (PVOID)LoadDllStub , dwSizeStub , NULL );

		if ( !bWriteShell )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #10" ) , XorStr( "Error" ) , MB_ICONERROR );
			VirtualFreeEx( hProcess , mem , 0 , MEM_RELEASE );
			VirtualFreeEx( hProcess , image , 0 , MEM_RELEASE );
			CloseHandle( hDllFile );
			CloseHandle( hProcess );
			return false;
		}

#if DEBUG_INJECT == 1
		printf( "ShellEntry: %p\n" , ShellEntry );
		getchar();
#endif

		hThread = CreateRemoteThread( hProcess , NULL , 0 , (LPTHREAD_START_ROUTINE)ShellEntry , mem , 0 , NULL );

		if ( !hThread )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #10 (1)" ) , XorStr( "Error" ) , MB_ICONERROR );
			VirtualFreeEx( hProcess , mem , 0 , MEM_RELEASE );
			VirtualFreeEx( hProcess , image , 0 , MEM_RELEASE );
			CloseHandle( hDllFile );
			CloseHandle( hProcess );
			return false;
		}

		WaitForSingleObject( hThread , INFINITE );
		GetExitCodeThread( hThread , &ExitCode );

		if ( !ExitCode )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #10 (2)" ) , XorStr( "Error" ) , MB_ICONERROR );
			VirtualFreeEx( hProcess , mem , 0 , MEM_RELEASE );
			VirtualFreeEx( hProcess , image , 0 , MEM_RELEASE );
			CloseHandle( hDllFile );
			CloseHandle( hThread );
			CloseHandle( hProcess );
			return false;
		}

		VirtualFreeEx( hProcess , mem , 0 , MEM_RELEASE );

		CloseHandle( hDllFile );
		CloseHandle( hThread );
		CloseHandle( hProcess );

#if DEBUG_INJECT == 1
		printf( "Mem Free\n" );
		getchar();
#endif

		return true;
	}

	DWORD dwGetProcessID( const char* szProcName )
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS , 0 );
		DWORD dwGetProcessID = 0;

		if ( hSnapshot != INVALID_HANDLE_VALUE )
		{
			PROCESSENTRY32 ProcEntry32 = { 0 };
			ProcEntry32.dwSize = sizeof( MODULEENTRY32 );

			if ( Process32First( hSnapshot , &ProcEntry32 ) )
			{
				do
				{
					if ( _stricmp( ProcEntry32.szExeFile , szProcName ) == 0 )
					{
						dwGetProcessID = (DWORD)ProcEntry32.th32ProcessID;
						break;
					}
				} while ( Process32Next( hSnapshot , &ProcEntry32 ) );
			}

			CloseHandle( hSnapshot );
		}

		return dwGetProcessID;
	}

	bool GetPrivileges()
	{
		HANDLE hToken;
		TOKEN_PRIVILEGES tp;

		if ( OpenProcessToken( (HANDLE)-1 , TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY , &hToken ) )
		{
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			tp.Privileges[0].Luid.LowPart = 20;
			tp.Privileges[0].Luid.HighPart = 0;

			AdjustTokenPrivileges( hToken , FALSE , &tp , 0 , NULL , NULL );
			CloseHandle( hToken );

			return true;
		}

		return false;
	}

	void InjectCSGO()
	{
		DWORD PID = 0;

		if ( !GetDllInject() )
		{
			MessageBoxA( 0 , XorStr( "Error Code: #1" ) , XorStr( "Error" ) , MB_ICONERROR );
			goto exit;
		}

		while ( PID == 0 )
		{
			PID = dwGetProcessID( XorStr( "csgo.exe" ) );
			Sleep( 500 );
		}

		GetCurrentDirectoryA( MAX_PATH , szCurrentDir );

		if ( GetPrivileges() && InjectDll( PID ) )
			printf( "%s" , XorStr( "[+] inject success\n" ) );
		else
			printf( "%s" , XorStr( "[-] inject error\n" ) );

exit:;
		Sleep( 3000 );
	}
}
#pragma once

#include "win32.hpp"

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <filesystem>
#include <fstream>

using namespace std;

#include "../Andromeda/Core.hpp"
#include "../Andromeda/Log.hpp"
#include "../Andromeda/Memory.hpp"
#include "../Andromeda/Input.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "AngelScript/angelscript.h"
#include "AngelScript/as_jit.h"

#include "AngelScript/datetime/datetime.h"
#include "AngelScript/scriptany/scriptany.h"
#include "AngelScript/scriptarray/scriptarray.h"
#include "AngelScript/scripthelper/scripthelper.h"
#include "AngelScript/scriptstdstring/scriptstdstring.h"
#include "AngelScript/scriptbuilder/scriptbuilder.h"
#include "AngelScript/scriptdictionary/scriptdictionary.h"
#include "AngelScript/scriptmath/scriptmath.h"

#pragma comment(lib,"Andromeda/Include/AngelScript/AngelScript.lib")
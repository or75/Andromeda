#pragma once

#include "../Include/Auto.hpp"

#include "Math/Matrix.hpp"
#include "Math/QAngle.hpp"

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"

#pragma region region_index

namespace index
{
	namespace IGameEvent
	{
		constexpr auto GetName = 1;
		constexpr auto IsReliable = 2;
		constexpr auto IsLocal = 3;
		constexpr auto IsEmpty = 4;
		constexpr auto GetBool = 5;
		constexpr auto GetInt = 6;
		constexpr auto GetUint64 = 7;
		constexpr auto GetFloat = 8;
		constexpr auto GetString = 9;
		constexpr auto SetBool = 12;
		constexpr auto SetInt = 13;
		constexpr auto SetUint64 = 14;
		constexpr auto SetFloat = 15;
		constexpr auto SetString = 16;
	}
	namespace IBaseClientDLL
	{
		constexpr auto GetAllClasses = 8;
	}
	namespace IClientEntityList
	{
		constexpr auto GetClientEntity = 3;
		constexpr auto GetClientEntityFromHandle = 4;
		constexpr auto GetHighestEntityIndex = 6;
	}
	namespace IVEngineClient
	{
		constexpr auto GetScreenSize = 5;
		constexpr auto GetPlayerInfo = 8;
		constexpr auto GetPlayerForUserID = 9;
		constexpr auto Con_IsVisible = 11;
		constexpr auto GetLocalPlayer = 12;
		constexpr auto GetLastTimeStamp = 14;
		constexpr auto GetViewAngles = 18;
		constexpr auto SetViewAngles = 19;
		constexpr auto GetMaxClients = 20;
		constexpr auto IsInGame = 26;
		constexpr auto Con_NPrintf = 30;
		constexpr auto GetGameDirectory = 36;
		constexpr auto WorldToScreenMatrix = 37;
		constexpr auto GetLevelNameShort = 53;
		constexpr auto ClientCmd_Unrestricted = 114;
		constexpr auto IsActiveApp = 196;
	}
	namespace IEngineSound
	{
		constexpr auto GetActiveSounds = 19;
	}
	namespace IFileSystem
	{
		constexpr auto String = 42;
	}
	namespace IEngineTrace
	{
		constexpr auto GetPointContents = 0;
		constexpr auto ClipRayToEntity = 3;
		constexpr auto TraceRay = 5;
	}
	namespace ICvar
	{
		constexpr auto FindVar = 14;
		constexpr auto ConsoleColorPrintf = 25;
		constexpr auto ConsolePrintf = 26;
	}
	namespace IClientMode
	{
		constexpr auto CreateMove = 24;
	}
	namespace IInputSystem
	{
		constexpr auto EnableInput = 11;
		constexpr auto IsButtonDown = 15;
		constexpr auto ButtonCodeToString = 40;
		constexpr auto StringToButtonCode = 42;
		constexpr auto VirtualKeyToButtonCode = 45;
		constexpr auto ButtonCodeToVirtualKey = 46;
	}
	namespace ISurface
	{
		constexpr auto UnlockCursor = 66;
		constexpr auto LockCursor = 67;
		constexpr auto PlaySoundFile = 82;
	}
}

#pragma endregion

#pragma region region_define

#define VirtualFn( cast ) using Fn = cast(__thiscall*)

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40
#define   CONTENTS_OPAQUE               0x80
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800
#define   CONTENTS_TEAM2                0x1000
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000

#define   CONTENTS_MONSTER              0x2000000
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001
#define   SURF_SKY2D                    0x0002
#define   SURF_SKY                      0x0004
#define   SURF_WARP                     0x0008
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020
#define   SURF_TRIGGER                  0x0040
#define   SURF_NODRAW                   0x0080

#define   SURF_HINT                     0x0100

#define   SURF_SKIP                     0x0200
#define   SURF_NOLIGHT                  0x0400
#define   SURF_BUMPLIGHT                0x0800
#define   SURF_NOSHADOWS                0x1000
#define   SURF_NODECALS                 0x2000
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000
#define   SURF_HITBOX                   0x8000

#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)
#define	  MASK_PLAYER_VISIBLE			(CONTENTS_HITBOX|CONTENTS_MONSTER|CONTENTS_DEBRIS|CONTENTS_MOVEABLE|CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE)

// m_lifeState values
#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

// CBaseEntity::m_fFlags
// PLAYER SPECIFIC FLAGS FIRST BECAUSE WE USE ONLY A FEW BITS OF NETWORK PRECISION
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define IN_LOOKSPIN		(1 << 25)

#pragma endregion

#pragma region region_enum

enum TeamNum
{
	TEAM_UNKNOWN ,
	TEAM_SPEC ,
	TEAM_TT ,
	TEAM_CT ,
};

enum ObserverMode_t : int
{
	OBS_MODE_NONE = 0 ,
	OBS_MODE_DEATHCAM = 1 ,
	OBS_MODE_FREEZECAM = 2 ,
	OBS_MODE_FIXED = 3 ,
	OBS_MODE_IN_EYE = 4 ,
	OBS_MODE_CHASE = 5 ,
	OBS_MODE_ROAMING = 6
};

enum MoveType_t
{
	MOVETYPE_NONE = 0 ,			// never moves
	MOVETYPE_ISOMETRIC ,		// For players -- in TF2 commander view, etc.
	MOVETYPE_WALK ,				// Player only - moving on the ground
	MOVETYPE_STEP ,				// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY ,				// No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY ,		// flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS ,			// uses VPHYSICS for simulation
	MOVETYPE_PUSH ,				// no clip to world, push and crush
	MOVETYPE_NOCLIP ,			// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER ,			// Used by players only when going onto a ladder
	MOVETYPE_OBSERVER ,			// Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM ,			// Allows the entity to describe its own physics
	MOVETYPE_LAST = MOVETYPE_CUSTOM ,
	MOVETYPE_MAX_BITS = 4
};

enum SendPropType
{
	DPT_Int = 0 ,
	DPT_Float ,
	DPT_Vector ,
	DPT_VectorXY ,
	DPT_String ,
	DPT_Array ,
	DPT_DataTable ,
	DPT_Int64 ,
	DPT_NUMSendPropTypes ,
};

enum hitgroup_t
{
	HITGROUP_GENERIC ,
	HITGROUP_HEAD ,
	HITGROUP_CHEST ,
	HITGROUP_STOMACH ,
	HITGROUP_LEFTARM ,
	HITGROUP_RIGHTARM ,
	HITGROUP_LEFTLEG ,
	HITGROUP_RIGHTLEG ,
	HITGROUP_GEAR = 10 ,
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0 ,
	TRACE_WORLD_ONLY ,
	TRACE_ENTITIES_ONLY ,
	TRACE_EVERYTHING_FILTER_PROPS ,
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0 ,
	WEAPONTYPE_PISTOL ,
	WEAPONTYPE_SUBMACHINEGUN ,
	WEAPONTYPE_RIFLE ,
	WEAPONTYPE_SHOTGUN ,
	WEAPONTYPE_SNIPER_RIFLE ,
	WEAPONTYPE_MACHINEGUN ,
	WEAPONTYPE_C4 ,
	WEAPONTYPE_PLACEHOLDER ,
	WEAPONTYPE_GRENADE ,
	WEAPONTYPE_UNKNOWN ,
	MAXCSWEAPONTYPE
};

enum EntityClassId
{
	CItem = 0 ,	CAI_BaseNPC = 0 ,
	CPropVehicleChoreoGeneric = 0 ,
	CWeaponCubemap = 0 ,
	CItemAssaultSuitUseable = 0 ,
	NextBotCombatCharacter = 0 ,
	CAK47 = 1 ,
	CBaseAnimating = 2 ,
	CBaseAnimatingOverlay = 3 ,
	CBaseAttributableItem = 4 ,
	CBaseButton = 5 ,
	CBaseCombatCharacter = 6 ,
	CBaseCombatWeapon = 7 ,
	CBaseCSGrenade = 8 ,
	CBaseCSGrenadeProjectile = 9 ,
	CBaseDoor = 10 ,
	CBaseEntity = 11 ,
	CBaseFlex = 12 ,
	CBaseGrenade = 13 ,
	CBaseParticleEntity = 14 ,
	CBasePlayer = 15 ,
	CBasePropDoor = 16 ,
	CBaseTeamObjectiveResource = 17 ,
	CBaseTempEntity = 18 ,
	CBaseToggle = 19 ,
	CBaseTrigger = 20 ,
	CBaseViewModel = 21 ,
	CBaseVPhysicsTrigger = 22 ,
	CBeam = 24 ,
	CBeamSpotlight = 25 ,
	CBoneFollower = 26 ,
	CBRC4Target = 27 ,
	CBreachCharge = 28 ,
	CBreachChargeProjectile = 29 ,
	CBreakableProp = 30 ,
	CBreakableSurface = 31 ,
	CC4 = 32 ,
	CCascadeLight = 33 ,
	CChicken = 34 ,
	CColorCorrection = 35 ,
	CColorCorrectionVolume = 36 ,
	CCSGameRulesProxy = 37 ,
	CCSPlayer = 38 ,
	CCSPlayerResource = 39 ,
	CCSRagdoll = 40 ,
	CCSTeam = 41 ,
	CDangerZone = 42 ,
	CDangerZoneController = 43 ,
	CDEagle = 44 ,
	CDecoyGrenade = 45 ,
	CDecoyProjectile = 46 ,
	CDrone = 47 ,
	CDronegun = 48 ,
	CDynamicLight = 49 ,
	CDynamicProp = 50 ,
	CEconEntity = 51 ,
	CEconWearable = 52 ,
	CEmbers = 53 ,
	CEntityDissolve = 54 ,
	CEntityFlame = 55 ,
	CEntityFreezing = 56 ,
	CEntityParticleTrail = 57 ,
	CEnvAmbientLight = 58 ,
	CEnvDetailController = 59 ,
	CEnvDOFController = 60 ,
	CEnvGasCanister = 61 ,
	CEnvParticleScript = 62 ,
	CEnvProjectedTexture = 63 ,
	CEnvQuadraticBeam = 64 ,
	CEnvScreenEffect = 65 ,
	CEnvScreenOverlay = 66 ,
	CEnvTonemapController = 67 ,
	CEnvWind = 68 ,
	CFEPlayerDecal = 69 ,
	CFireCrackerBlast = 70 ,
	CFireSmoke = 71 ,
	CFireTrail = 72 ,
	CFish = 73 ,
	CFists = 74 ,
	CFlashbang = 75 ,
	CFogController = 76 ,
	CFootstepControl = 77 ,
	CFunc_Dust = 78 ,
	CFunc_LOD = 79 ,
	CFuncAreaPortalWindow = 80 ,
	CFuncBrush = 81 ,
	CFuncConveyor = 82 ,
	CFuncLadder = 83 ,
	CFuncMonitor = 84 ,
	CFuncMoveLinear = 85 ,
	CFuncOccluder = 86 ,
	CFuncReflectiveGlass = 87 ,
	CFuncRotating = 88 ,
	CFuncSmokeVolume = 89 ,
	CFuncTrackTrain = 90 ,
	CGameRulesProxy = 91 ,
	CGrassBurn = 92 ,
	CHandleTest = 93 ,
	CHEGrenade = 94 ,
	CHostage = 95 ,
	CHostageCarriableProp = 96 ,
	CIncendiaryGrenade = 97 ,
	CInferno = 98 ,
	CInfoLadderDismount = 99 ,
	CInfoMapRegion = 100 ,
	CInfoOverlayAccessor = 101 ,
	CItem_Healthshot = 102 ,
	CItemCash = 103 ,
	CItemDogtags = 104 ,
	CKnife = 105 ,
	CKnifeGG = 106 ,
	CLightGlow = 107 ,
	CMaterialModifyControl = 108 ,
	CMelee = 109 ,
	CMolotovGrenade = 110 ,
	CMolotovProjectile = 111 ,
	CMovieDisplay = 112 ,
	CParadropChopper = 113 ,
	CParticleFire = 114 ,
	CParticlePerformanceMonitor = 115 ,
	CParticleSystem = 116 ,
	CPhysBox = 117 ,
	CPhysBoxMultiplayer = 118 ,
	CPhysicsProp = 119 ,
	CPhysicsPropMultiplayer = 120 ,
	CPhysMagnet = 121 ,
	CPhysPropAmmoBox = 122 ,
	CPhysPropLootCrate = 123 ,
	CPhysPropRadarJammer = 124 ,
	CPhysPropWeaponUpgrade = 125 ,
	CPlantedC4 = 126 ,
	CPlasma = 127 ,
	CPlayerResource = 128 ,
	CPointCamera = 129 ,
	CPointCommentaryNode = 130 ,
	CPointWorldText = 131 ,
	CPoseController = 132 ,
	CPostProcessController = 133 ,
	CPrecipitation = 134 ,
	CPrecipitationBlocker = 135 ,
	CPredictedViewModel = 136 ,
	CProp_Hallucination = 137 ,
	CPropCounter = 138 ,
	CPropDoorRotating = 139 ,
	CPropJeep = 140 ,
	CPropVehicleDriveable = 141 ,
	CRagdollManager = 142 ,
	CRagdollProp = 143 ,
	CRagdollPropAttached = 144 ,
	CRopeKeyframe = 145 ,
	CSCAR17 = 146 ,
	CSceneEntity = 147 ,
	CSensorGrenade = 148 ,
	CSensorGrenadeProjectile = 149 ,
	CShadowControl = 150 ,
	CSlideshowDisplay = 151 ,
	CSmokeGrenade = 152 ,
	CSmokeGrenadeProjectile = 153 ,
	CSmokeStack = 154 ,
	CSnowball = 155 ,
	CSnowballPile = 156 ,
	CSnowballProjectile = 157 ,
	CSpatialEntity = 158 ,
	CSpotlightEnd = 159 ,
	CSprite = 160 ,
	CSpriteOriented = 161 ,
	CSpriteTrail = 162 ,
	CStatueProp = 163 ,
	CSteamJet = 164 ,
	CSun = 165 ,
	CSunlightShadowControl = 166 ,
	CSurvivalSpawnChopper = 167 ,
	CTablet = 168 ,
	CTeam = 169 ,
	CTeamplayRoundBasedRulesProxy = 170 ,
	CTEArmorRicochet = 171 ,
	CTEBaseBeam = 172 ,
	CTEBeamEntPoint = 173 ,
	CTEBeamEnts = 174 ,
	CTEBeamFollow = 175 ,
	CTEBeamLaser = 176 ,
	CTEBeamPoints = 177 ,
	CTEBeamRing = 178 ,
	CTEBeamRingPoint = 179 ,
	CTEBeamSpline = 180 ,
	CTEBloodSprite = 181 ,
	CTEBloodStream = 182 ,
	CTEBreakModel = 183 ,
	CTEBSPDecal = 184 ,
	CTEBubbles = 185 ,
	CTEBubbleTrail = 186 ,
	CTEClientProjectile = 187 ,
	CTEDecal = 188 ,
	CTEDust = 189 ,
	CTEDynamicLight = 190 ,
	CTEEffectDispatch = 191 ,
	CTEEnergySplash = 192 ,
	CTEExplosion = 193 ,
	CTEFireBullets = 194 ,
	CTEFizz = 195 ,
	CTEFootprintDecal = 196 ,
	CTEFoundryHelpers = 197 ,
	CTEGaussExplosion = 198 ,
	CTEGlowSprite = 199 ,
	CTEImpact = 200 ,
	CTEKillPlayerAttachments = 201 ,
	CTELargeFunnel = 202 ,
	CTEMetalSparks = 203 ,
	CTEMuzzleFlash = 204 ,
	CTEParticleSystem = 205 ,
	CTEPhysicsProp = 206 ,
	CTEPlantBomb = 207 ,
	CTEPlayerAnimEvent = 208 ,
	CTEPlayerDecal = 209 ,
	CTEProjectedDecal = 210 ,
	CTERadioIcon = 211 ,
	CTEShatterSurface = 212 ,
	CTEShowLine = 213 ,
	CTesla = 214 ,
	CTESmoke = 215 ,
	CTESparks = 216 ,
	CTESprite = 217 ,
	CTESpriteSpray = 218 ,
	CTest_ProxyToggle_Networkable = 219 ,
	CTestTraceline = 220 ,
	CTEWorldDecal = 221 ,
	CTriggerPlayerMovement = 222 ,
	CTriggerSoundOperator = 223 ,
	CVGuiScreen = 224 ,
	CVoteController = 225 ,
	CWaterBullet = 226 ,
	CWaterLODControl = 227 ,
	CWeaponAug = 228 ,
	CWeaponAWP = 229 ,
	CWeaponBaseItem = 230 ,
	CWeaponBizon = 231 ,
	CWeaponCSBase = 232 ,
	CWeaponCSBaseGun = 233 ,
	CWeaponCycler = 234 ,
	CWeaponElite = 235 ,
	CWeaponFamas = 236 ,
	CWeaponFiveSeven = 237 ,
	CWeaponGalil = 239 ,
	CWeaponGalilAR = 240 ,
	CWeaponGlock = 241 ,
	CWeaponM3 = 244 ,
	CWeaponM4A1 = 245 ,
	CWeaponMAC10 = 246 ,
	CWeaponMag7 = 247 ,
	CWeaponMP5Navy = 248 ,
	CWeaponM249 = 243 ,
	CWeaponMP7 = 249 ,
	CWeaponMP9 = 250 ,
	CWeaponNegev = 251 ,
	CWeaponNOVA = 252 ,
	CWeaponP228 = 253 ,
	CWeaponP250 = 254 ,
	CWeaponP90 = 255 ,
	CWeaponSawedoff = 256 ,
	CWeaponSCAR20 = 257 ,
	CWeaponScout = 258 ,
	CWeaponSSG08 = 262 ,
	CWeaponTaser = 263 ,
	CWeaponTec9 = 264 ,
	CWeaponTMP = 265 ,
	CWeaponUMP45 = 266 ,
	CWeaponUSP = 267 ,
	CWorld = 269 ,
	CWorldVguiText = 270 ,
	DustTrail = 271 ,
	MovieExplosion = 272 ,
	ParticleSmokeGrenade = 273 ,
	RocketTrail = 274 ,
	SmokeTrail = 275 ,
	SporeExplosion = 276 ,
	SporeTrail = 277 ,
	CWeaponSG550 = 259 ,
	CWeaponSG552 = 260 ,
	CWeaponSG556 = 261 ,
	CWeaponXM1014 = 268 ,
	CWeaponHKP2000 = 242 ,
	CWeaponG3SG1 = 238 ,
};

#pragma endregion

#pragma region region_struct

struct model_t;
struct player_info_t;
struct SndInfo_t;
struct cplane_t;
struct csurface_t;
struct Ray_t;

#pragma endregion

#pragma region region_class

class Color;

class IHandleEntity;
class IClientUnknown;
class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;

class C_BaseEntity;
class C_BaseAnimating;
class C_BaseCombatCharacter;
class C_BasePlayer;
class C_CSPlayer;
class C_BaseCombatWeapon;
class C_WeaponCSBase;

class InterfaceReg;
class DVariant;
class CRecvProxyData;
class RecvProp;
class RecvTable;
class ClientClass;
class CGlobalVarsBase;
class CBaseClientState;
class IGameEvent;
class IGameEventListener;
class CBaseTrace;
class CGameTrace;
class ITraceFilter;
class CTraceFilter;
class CUserCmd;
class CCSWeaponInfo;
class CCommand;
class ConCommandBase;
class ConVar;

#pragma endregion

#pragma region region_interface

class IBaseClientDLL;
class IClientEntityList;
class IVEngineClient;
class IGameEventManager;
class IEngineSound;
class IFileSystem;
class IEngineTrace;
class ICvar;
class IClientMode;
class IInputSystem;
class ISurface;

#pragma endregion

#pragma region region_type

using CBaseHandle = unsigned long;
using VectorAligned = Vector4;
using trace_t = CGameTrace;
using CRC32_t = unsigned int;

using InstantiateInterfaceFn = void* ( *)( );
using RecvVarProxyFn = void( *)( const CRecvProxyData* pData , void* pStruct , void* pOut );
using ArrayLengthRecvProxyFn = void( *)( void* pStruct , int objectID , int currentArrayLength );
using DataTableRecvVarProxyFn = void( *)( const RecvProp* pProp , void** pOut , void* pData , int objectID );
using CreateClientClassFn = IClientNetworkable * ( *)( int entnum , int serialNum );
using CreateEventFn = IClientNetworkable * ( *)( );

using FileNameHandle_t = void*;

using FnChangeCallback_t = void( *)( const CCommand& command );

#pragma endregion

#pragma region region_procedure

auto CRC32_Init( CRC32_t* pulCRC ) -> void;
auto CRC32_ProcessBuffer( CRC32_t* pulCRC , const void* p , int len ) -> void;
auto CRC32_Final( CRC32_t* pulCRC ) -> void;
auto CRC32_GetTableEntry( unsigned int slot )->CRC32_t;

#pragma endregion

#pragma region region_struct_impl

constexpr auto MAX_PLAYER_NAME_LENGTH = 128;
constexpr auto SIGNED_GUID_LEN = 32;

struct model_t
{
	FileNameHandle_t fnHandle = nullptr; //0x0000 
	char szName[260] = { }; //0x0004 
	__int32 nLoadFlags = 0; //0x0108 
	__int32 nServerCount = 0; //0x010C 
	__int32 type = 0; //0x0110 
	__int32 flags = 0; //0x0114 
	Vector3 vecMins = { }; //0x0118 
	Vector3 vecMaxs = { }; //0x0124 
	float radius = 0.f; //0x0130 
};

struct player_info_t
{
private:
	int64_t version = 0;
public:
	union
	{
		uint64_t steamID64 = 0;
		struct
		{
			int xuid_low;
			int xuid_high;
		};
	};
	char name[MAX_PLAYER_NAME_LENGTH] = { };
	int userid = 0;
	int m_nUserID = 0;
	char guid[SIGNED_GUID_LEN + 1] = { };
	unsigned int friendsid = 0;
	char friendsname[MAX_PLAYER_NAME_LENGTH] = { };
	bool fakeplayer = false;
	bool ishltv = false;
	unsigned int customfiles[4] = { };
	unsigned char filesdownloaded = 0;
};

struct SndInfo_t
{
	int m_nGuid = 0;
	FileNameHandle_t m_filenameHandle = nullptr;
	int m_nSoundSource = 0;
	int m_nChannel = 0;

	int m_nSpeakerEntity = 0;
	float m_flVolume = 0.f;
	float m_flLastSpatializedVolume = 0.f;

	float m_flRadius = 0.f;
	int m_nPitch = 0;

	Vector3* m_pOrigin = nullptr;
	Vector3* m_pDirection = nullptr;

	bool m_bUpdatePositions = false;
	bool m_bIsSentence = false;
	bool m_bDryMix = false;
	bool m_bSpeaker = false;
	bool m_bSpecialDSP = false;
	bool m_bFromServer = false;
};

struct cplane_t
{
	Vector3 normal = { };
	float dist = 0.f;
	BYTE type = 0;
	BYTE signbits = 0;
	BYTE pad[2];
};

struct csurface_t
{
	const char* name = nullptr;
	short surfaceProps = 0;
	unsigned short flags = 0;
};

struct Ray_t
{
	VectorAligned m_Start = { };
	VectorAligned m_Delta = { };
	VectorAligned m_StartOffset = { };
	VectorAligned m_Extents = { };

	const matrix3x4_t* m_pWorldAxisTransform = nullptr;

	bool m_IsRay = false;
	bool m_IsSwept = false;

	Ray_t() : m_pWorldAxisTransform( nullptr ) {}

	auto Init( const Vector3& start , const Vector3& end ) -> void
	{
		m_Delta = ( end - start ).ToVector4D();
		m_IsSwept = ( m_Delta.LengthSquared() != 0.f );
		m_Extents.Init();
		m_pWorldAxisTransform = nullptr;
		m_IsRay = true;
		m_StartOffset.Init();
		m_Start = start.ToVector4D();
	}

	auto Init( const Vector3 & start , const Vector3 & end , const Vector3 & mins , const Vector3 & maxs )
	{
		m_Delta = ( end - start ).ToVector4D();
		m_pWorldAxisTransform = nullptr;
		m_IsSwept = ( m_Delta.LengthSquared() != 0.f );
		m_Extents = ( maxs - mins ).ToVector4D();
		m_Extents *= 0.5f;
		m_IsRay = ( m_Extents.LengthSquared() < 1e-6f );
		m_StartOffset = ( maxs + mins ).ToVector4D();
		m_StartOffset *= 0.5f;
		m_Start = start.ToVector4D() + m_StartOffset;
		m_StartOffset *= -1.f;
	}
};

#pragma endregion

#pragma region region_class_impl

template< class T , class I = int >
class CUtlMemory
{
public:
	T& operator[]( I i )
	{
		return m_pMemory[i];
	}

	T* Base()
	{
		return m_pMemory;
	}

	int NumAllocated() const
	{
		return m_nAllocationCount;
	}

protected:
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T>
inline void Destruct( T * pMemory )
{
	pMemory->~T();
}

template< class T , class A = CUtlMemory<T> >
class CUtlVector
{
	typedef A CAllocator;
public:
	T& operator[]( int i )
	{
		return m_Memory[i];
	}

	T& Element( int i )
	{
		return m_Memory[i];
	}

	T* Base()
	{
		return m_Memory.Base();
	}

	int Count() const
	{
		return m_Size;
	}

	void RemoveAll()
	{
		for ( int i = m_Size; --i >= 0; )
			Destruct( &Element( i ) );

		m_Size = 0;
	}

	CAllocator m_Memory;
	int m_Size;
	T * m_pElements;
};

class InterfaceReg
{
public:
	InstantiateInterfaceFn m_CreateFn = nullptr;
	const char* m_pName = nullptr;
	InterfaceReg* m_pNext = nullptr;
};

class DVariant
{
public:
	union
	{
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
		float m_Vector[3];
		__int64	m_Int64;
	};
	SendPropType m_Type;
};

class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp;
	DVariant m_Value = {};
	int m_iElement = 0;
	int m_ObjectID = 0;
};

class RecvProp
{
public:
	const char* m_pVarName = nullptr;
	SendPropType m_RecvType = { };
	int m_Flags = 0;
	int m_StringBufferSize = 0;
	bool m_bInsideArray = false;
	const void* m_pExtraData = nullptr;
	RecvProp* m_pArrayProp = nullptr;
	ArrayLengthRecvProxyFn m_ArrayLengthProxy = nullptr;
	RecvVarProxyFn m_ProxyFn = nullptr;
	DataTableRecvVarProxyFn m_DataTableProxyFn = nullptr;
	RecvTable* m_pDataTable = nullptr;
	int m_Offset = 0;
	int m_ElementStride = 0;
	int m_nElements = 0;
	const char* m_pParentArrayPropName = nullptr;
};

class RecvTable
{
public:
	RecvProp* m_pProps = nullptr;
	int m_nProps = 0;
	void* m_pDecoder = nullptr;
	const char* m_pNetTableName = nullptr;
	bool m_bInitialized = false;
	bool m_bInMainList = false;
};

class ClientClass
{
public:
	CreateClientClassFn m_pCreateFn = nullptr;
	CreateEventFn m_pCreateEventFn = nullptr;
	char* m_pNetworkName = nullptr;
	RecvTable* m_pRecvTable = nullptr;
	ClientClass* m_pNext = nullptr;
	int m_ClassID = 0;
};

class CGlobalVarsBase
{
public:
	float realtime = 0.f;
	int framecount = 0;
	float absoluteframetime = 0.f;
	float absoluteframestarttimestddev = 0.f;
	float curtime = 0.f;
	float frametime = 0.f;
	int maxClients = 0;
	int tickcount = 0;
	float interval_per_tick = 0.f;
	float interpolation_amount = 0.f;
	int simTicksThisFrame = 0;
	int network_protocol = 0;
	void* pSaveData = nullptr;
	bool m_bClient = false;
	int nTimestampNetworkingBase = 0;
	int nTimestampRandomizeWindow = 0;
};

class CBaseClientState
{
public:
	auto ForceFullUpdate() -> void
	{
		*reinterpret_cast<int*>( reinterpret_cast<uintptr_t>( this ) + 0x174 ) = -1;
	};
};

class IGameEvent
{
public:
	virtual ~IGameEvent() {};

	auto GetName() -> string
	{
		VirtualFn( char const* )( IGameEvent* );
		return string( Andromeda::Memory::vget< Fn >( this , index::IGameEvent::GetName )( this ) );
	}
	auto IsReliable() -> bool
	{
		VirtualFn( bool )( IGameEvent* );
		return Andromeda::Memory::vget< Fn >( this , index::IGameEvent::IsReliable )( this );
	}
	auto IsLocal() -> bool
	{
		VirtualFn( bool )( IGameEvent* );
		return Andromeda::Memory::vget< Fn >( this , index::IGameEvent::IsLocal )( this );
	}
	auto IsEmpty( string keyname ) -> bool
	{
		VirtualFn( bool )( IGameEvent* , const char* );
		return Andromeda::Memory::vget< Fn >( this , index::IGameEvent::IsEmpty )( this , keyname.c_str() );
	}
	auto GetBool( string keyname , bool default_value = false ) -> bool
	{
		VirtualFn( bool )( IGameEvent* , const char* , bool );
		return Andromeda::Memory::vget< Fn >( this , index::IGameEvent::GetBool )( this , keyname.c_str() , default_value );
	}
	auto GetInt( string keyname , int default_value = false ) -> int
	{
		VirtualFn( int )( IGameEvent* , const char* , int );
		return Andromeda::Memory::vget< Fn >( this , index::IGameEvent::GetInt )( this , keyname.c_str() , default_value );
	}
	auto GetUint64( string keyname , uint64_t default_value = 0 ) -> uint64_t
	{
		VirtualFn( uint64_t )( IGameEvent* , const char* , uint64_t );
		return Andromeda::Memory::vget< Fn >( this , index::IGameEvent::GetUint64 )( this , keyname.c_str() , default_value );
	}
	auto GetFloat( string keyname , float default_value = 0.f ) -> float
	{
		VirtualFn( float )( IGameEvent* , const char* , float );
		return Andromeda::Memory::vget< Fn >( this , index::IGameEvent::GetFloat )( this , keyname.c_str() , default_value );
	}
	auto GetString( string keyname , string default_value = "" ) -> string
	{
		VirtualFn( const char* )( IGameEvent* , const char* , const char* );
		return string( Andromeda::Memory::vget< Fn >( this , index::IGameEvent::GetString )( this , keyname.c_str() , default_value.c_str() ) );
	}
	auto SetBool( string keyname , bool default_value = false ) -> void
	{
		VirtualFn( void )( IGameEvent* , const char* , bool );
		Andromeda::Memory::vget< Fn >( this , index::IGameEvent::SetBool )( this , keyname.c_str() , default_value );
	}
	auto SetInt( string keyname , int default_value = false ) -> void
	{
		VirtualFn( void )( IGameEvent* , const char* , int );
		Andromeda::Memory::vget< Fn >( this , index::IGameEvent::SetInt )( this , keyname.c_str() , default_value );
	}
	auto SetUint64( string keyname , uint64_t default_value = false ) -> void
	{
		VirtualFn( void )( IGameEvent* , const char* , uint64_t );
		Andromeda::Memory::vget< Fn >( this , index::IGameEvent::SetUint64 )( this , keyname.c_str() , default_value );
	}
	auto SetFloat( string keyname , float default_value = false ) -> void
	{
		VirtualFn( void )( IGameEvent* , const char* , float );
		Andromeda::Memory::vget< Fn >( this , index::IGameEvent::SetFloat )( this , keyname.c_str() , default_value );
	}
	auto SetString( string keyname , string default_value = "" ) -> void
	{
		VirtualFn( void )( IGameEvent* , const char* , const char* );
		Andromeda::Memory::vget< Fn >( this , index::IGameEvent::SetString )( this , keyname.c_str() , default_value.c_str() );
	}
};

class IGameEventListener
{
public:
	virtual ~IGameEventListener() {};
	virtual void FireGameEvent( IGameEvent* event ) = 0;
	virtual int GetEventDebugID() = 0;
};

class CBaseTrace
{
public:
	Vector3 startpos = { };
	Vector3 endpos = { };
	cplane_t plane = { };
	float fraction = 0.f;
	int contents = 0;
	unsigned short dispFlags = 0;
	bool allsolid = false;
	bool startsolid = false;
};

class CGameTrace : public CBaseTrace
{
public:
	float fractionleftsolid = 0.f;
	csurface_t surface = { };
	int hitgroup = 0;
	short physicsbone = 0;
	unsigned short worldSurfaceIndex = 0;
	IClientEntity* m_pEnt = nullptr;
	int hitbox = 0;

	auto DidHit() const -> bool;
	auto IsVisible() const -> bool;
	auto DidHitWorld() const -> bool;
	auto DidHitNonWorldEntity() const -> bool;
	auto GetEntityIndex() const -> int;
};

class ITraceFilter
{
public:
	virtual auto ShouldHitEntity( IHandleEntity* pEntity , int contentsMask ) -> bool = 0;
	virtual auto GetTraceType() const->TraceType_t = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual auto GetTraceType() const -> TraceType_t
	{
		return TRACE_EVERYTHING;
	}
};

class CUserCmd
{
public:
	CUserCmd()
	{
		Reset();
	}

	CUserCmd( const CUserCmd& cmd )
	{
		*this = cmd;
	}

	virtual ~CUserCmd() = default;

public:
	auto Reset() -> void
	{
		command_number = 0;
		tick_count = 0;
		viewangles.Init();
		aimdirection.Init();
		forwardmove = 0.f;
		sidemove = 0.f;
		upmove = 0.f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
		mousedx = 0;
		mousedy = 0;
		hasbeenpredicted = false;
		headangles.Init();
		headoffset.Init();
	}

	auto GetChecksum() const -> CRC32_t
	{
		CRC32_t crc = 0u;

		CRC32_Init( &crc );
		CRC32_ProcessBuffer( &crc , &command_number , sizeof( command_number ) );
		CRC32_ProcessBuffer( &crc , &tick_count , sizeof( tick_count ) );
		CRC32_ProcessBuffer( &crc , &viewangles , sizeof( viewangles ) );
		CRC32_ProcessBuffer( &crc , &aimdirection , sizeof( aimdirection ) );
		CRC32_ProcessBuffer( &crc , &forwardmove , sizeof( forwardmove ) );
		CRC32_ProcessBuffer( &crc , &sidemove , sizeof( sidemove ) );
		CRC32_ProcessBuffer( &crc , &upmove , sizeof( upmove ) );
		CRC32_ProcessBuffer( &crc , &buttons , sizeof( buttons ) );
		CRC32_ProcessBuffer( &crc , &impulse , sizeof( impulse ) );
		CRC32_ProcessBuffer( &crc , &weaponselect , sizeof( weaponselect ) );
		CRC32_ProcessBuffer( &crc , &weaponsubtype , sizeof( weaponsubtype ) );
		CRC32_ProcessBuffer( &crc , &random_seed , sizeof( random_seed ) );
		CRC32_ProcessBuffer( &crc , &mousedx , sizeof( mousedx ) );
		CRC32_ProcessBuffer( &crc , &mousedy , sizeof( mousedy ) );
		CRC32_Final( &crc );

		return crc;
	}

public:
	auto operator = ( const CUserCmd& cmd ) -> CUserCmd &
	{
		if ( this == &cmd )
			return *this;

		command_number = cmd.command_number;
		tick_count = cmd.tick_count;
		viewangles = cmd.viewangles;
		aimdirection = cmd.aimdirection;
		forwardmove = cmd.forwardmove;
		sidemove = cmd.sidemove;
		upmove = cmd.upmove;
		buttons = cmd.buttons;
		impulse = cmd.impulse;
		weaponselect = cmd.weaponselect;
		weaponsubtype = cmd.weaponsubtype;
		random_seed = cmd.random_seed;
		mousedx = cmd.mousedx;
		mousedy = cmd.mousedy;
		hasbeenpredicted = cmd.hasbeenpredicted;
		headangles = cmd.headangles;
		headoffset = cmd.headoffset;

		return *this;
	}

public:
	int command_number = 0;
	int tick_count = 0;
	QAngle viewangles = { };
	Vector3 aimdirection = { };
	float forwardmove = 0.f;
	float sidemove = 0.f;
	float upmove = 0.f;
	int buttons = 0;
	uint8_t impulse = 0u;
	int weaponselect = 0;
	int weaponsubtype = 0;
	int random_seed = 0;
	short mousedx = 0;
	short mousedy = 0;
	bool hasbeenpredicted = false;
	QAngle headangles = { };
	Vector3 headoffset = { };
};

class CCSWeaponInfo
{
public:
	char _0x0000[20];
	__int32 max_clip;			//0x0014 
	char _0x0018[12];
	__int32 max_reserved_ammo;	//0x0024 
	char _0x0028[96];
	char* hud_name;				//0x0088 
	char* weapon_name;			//0x008C 
	char _0x0090[60];
	__int32 type;				//0x00CC 
	__int32 price;				//0x00D0 
	__int32 reward;				//0x00D4 
	char _0x00D8[20];
	BYTE full_auto;				//0x00EC 
	char _0x00ED[3];
	__int32 damage;				//0x00F0 
	float armor_ratio;			//0x00F4 
	__int32 bullets;			//0x00F8 
	float penetration;			//0x00FC 
	char _0x0100[8];
	float range;				//0x0108 
	float range_modifier;		//0x010C 
	char _0x0110[16];
	BYTE silencer;				//0x0120 
	char _0x0121[15];
	float max_speed;			//0x0130 
	float max_speed_alt;		//0x0134 
	char _0x0138[76];
	__int32 recoil_seed;		//0x0184 
	char _0x0188[32];
};// Size=0x01A8

class ConCommandBase
{
public:
	virtual ~ConCommandBase() = default;
public:
	ConCommandBase* m_pNext = nullptr;
	int m_bRegistered = 0;
	const char* m_pszName = nullptr;
	const char* m_pszHelpString = nullptr;
	int m_nFlags = 0;
};

class ConVar : public ConCommandBase
{
public:
	virtual ~ConVar() = default;

public:
	auto GetFloat() -> float
	{
		uint32_t xored = *( uint32_t* )&m_pParent->m_Value.m_fValue ^ ( uint32_t )this;
		return *(float*)&xored;
	}

	auto GetInt() -> int
	{
		return (int)( m_pParent->m_Value.m_nValue ^ (int)this );
	}

	auto GetBool() -> bool
	{
		return !!GetInt();
	}

	auto SetValue( float value ) -> void
	{
		float fNewValue = value;

		*(uint32_t*)&m_pParent->m_Value.m_fValue = *(uint32_t*)&fNewValue ^ (uint32_t)this;
		*(uint32_t*)&m_pParent->m_Value.m_nValue = (uint32_t)fNewValue ^ (uint32_t)this;
	}

	auto SetValue( int value ) -> void
	{
		SetValue( static_cast<float>( value ) );
	}

	auto SetValue( bool value ) -> void
	{
		SetValue( static_cast<int>( value ) );
	}

public:
	struct CVValue_t
	{
		char* m_pszString = nullptr;
		int m_StringLength = 0;
		float m_fValue = 0.f;
		int m_nValue = 0;
	};

private:
	char pad_04[0x04];
	ConVar* m_pParent = nullptr;
	const char* m_pszDefaultValue = nullptr;
	CVValue_t m_Value = {};
	int m_bHasMin = 0;
	float m_fMinVal = 0.f;
	int m_bHasMax = 0;
	float m_fMaxVal = 0.f;
	CUtlVector< FnChangeCallback_t > m_fnChangeCallbacks = {};
};

#pragma endregion

#pragma region region_interface_impl

class IBaseClientDLL
{
public:
	auto GetAllClasses() -> ClientClass*
	{
		VirtualFn( ClientClass* )( IBaseClientDLL* );
		return Andromeda::Memory::vget< Fn >( this , index::IBaseClientDLL::GetAllClasses )( this );
	}
};

class IClientEntityList
{
public:
	auto GetClientEntity( int entnum ) -> IClientEntity*
	{
		VirtualFn( IClientEntity* )( IClientEntityList* , int );
		return Andromeda::Memory::vget< Fn >( this , index::IClientEntityList::GetClientEntity )( this , entnum );
	}

	auto GetClientEntityFromHandle( CBaseHandle hEnt ) -> IClientEntity*
	{
		VirtualFn( IClientEntity* )( IClientEntityList* , CBaseHandle );
		return Andromeda::Memory::vget< Fn >( this , index::IClientEntityList::GetClientEntityFromHandle )( this , hEnt );
	}

	auto GetHighestEntityIndex() -> int
	{
		VirtualFn( int )( IClientEntityList* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientEntityList::GetHighestEntityIndex )( this );
	}
};

class IVEngineClient
{
public:
	auto GetScreenSize( int& width , int& height ) -> void
	{
		VirtualFn( void )( IVEngineClient* , int& , int& );
		Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetScreenSize )( this , width , height );
	}
	auto GetPlayerInfo( int index , player_info_t* player_info ) -> bool
	{
		VirtualFn( bool )( IVEngineClient* , int , player_info_t* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetPlayerInfo )( this , index , player_info );
	}
	auto GetPlayerForUserID( int userid ) -> int
	{
		VirtualFn( int )( IVEngineClient* , int );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetPlayerForUserID )( this , userid );
	}
	auto GetLocalPlayer() -> int
	{
		VirtualFn( int )( IVEngineClient* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetLocalPlayer )( this );
	}
	auto GetLastTimeStamp() -> float
	{
		VirtualFn( float )( IVEngineClient* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetLastTimeStamp )( this );
	}
	auto GetViewAngles( QAngle& va ) -> void
	{
		VirtualFn( void )( IVEngineClient* , QAngle& );
		Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetViewAngles )( this , va );
	}
	auto SetViewAngles( QAngle& va ) -> void
	{
		VirtualFn( void )( IVEngineClient* , QAngle& );
		Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::SetViewAngles )( this , va );
	}
	auto GetMaxClients() -> int
	{
		VirtualFn( int )( IVEngineClient* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetMaxClients )( this );
	}
	auto IsInGame() -> bool
	{
		VirtualFn( bool )( IVEngineClient* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::IsInGame )( this );
	}
	auto Con_NPrintf( int pos , const char* buff ) -> void
	{
		typedef void( __cdecl* Fn )( void* , int , const char* , ... );

		if ( buff == nullptr )
			return;

		Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::Con_NPrintf )( this , pos , buff );
	}
	auto GetGameDirectory() -> string
	{
		VirtualFn( const char* )( IVEngineClient* );
		return string( Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetGameDirectory )( this ) ) + XorStr( "\\" );
	}
	auto WorldToScreenMatrix() -> const VMatrix&
	{
		VirtualFn( const VMatrix& )( IVEngineClient* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::WorldToScreenMatrix )( this );
	}
	auto Con_IsVisible() -> bool
	{
		VirtualFn( bool )( IVEngineClient* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::Con_IsVisible )( this );
	}
	auto GetLevelNameShort() -> string
	{
		VirtualFn( char const* )( IVEngineClient* );
		return string( Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::GetLevelNameShort )( this ) );
	}
	auto ClientCmd_Unrestricted( string command ) -> void
	{
		VirtualFn( void )( IVEngineClient* , const char* , bool );
		Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::ClientCmd_Unrestricted )( this , command.c_str() , false );
	}
	auto IsActiveApp() -> bool
	{
		VirtualFn( bool )( IVEngineClient* );
		return Andromeda::Memory::vget< Fn >( this , index::IVEngineClient::IsActiveApp )( this );
	}
};

class IGameEventManager
{
public:
	virtual ~IGameEventManager() {};
	virtual int LoadEventsFromFile( const char* filename ) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener( IGameEventListener* listener , const char* name , bool serverside ) = 0;
	virtual bool FindListener( IGameEventListener* listener , const char* name ) = 0;
	virtual void RemoveListener( IGameEventListener* listener ) = 0;
	virtual void AddListenerGlobal( IGameEventListener* listener , bool serverside ) = 0;
	virtual IGameEvent* EventCreate( const char* name , bool force = false , int* cookie = nullptr ) = 0;
	virtual bool FireEvent( IGameEvent* event , bool bDontBroadcast = false ) = 0;
	virtual bool FireEventClientSide( IGameEvent* event ) = 0;
	virtual IGameEvent* DuplicateEvent( IGameEvent* event ) = 0;
	virtual void FreeEvent( IGameEvent* event ) = 0;
};

class IEngineSound
{
public:
	auto GetActiveSounds( CUtlVector<SndInfo_t>& sndlist ) -> void
	{
		VirtualFn( void )( IEngineSound* , CUtlVector<SndInfo_t>& );
		Andromeda::Memory::vget< Fn >( this , index::IEngineSound::GetActiveSounds )( this , sndlist );
	}
};

class IFileSystem
{
public:
	auto String( const FileNameHandle_t& handle , char* buf , int buflen ) -> bool
	{
		VirtualFn( bool )( IFileSystem* , const FileNameHandle_t& , char* , int );
		return Andromeda::Memory::vget< Fn >( this , index::IFileSystem::String )( this , handle , buf , buflen );
	}
};

class IEngineTrace
{
public:
	auto GetPointContents( const Vector3& vecAbsPosition , int iMask , C_BaseEntity** ppEntity = nullptr ) -> int
	{
		VirtualFn( int )( IEngineTrace* , const Vector3& , int , C_BaseEntity** );
		return Andromeda::Memory::vget< Fn >( this , index::IEngineTrace::GetPointContents )( this , vecAbsPosition , iMask , ppEntity );
	}

	auto ClipRayToEntity( const Ray_t& ray , unsigned int fMask , IHandleEntity* pEnt , trace_t* pTrace ) -> void
	{
		VirtualFn( void )( IEngineTrace* , const Ray_t& , unsigned int , IHandleEntity* , trace_t* );
		return Andromeda::Memory::vget< Fn >( this , index::IEngineTrace::ClipRayToEntity )( this , ray , fMask , pEnt , pTrace );
	}

	auto TraceRay( const Ray_t& ray , unsigned int fMask , ITraceFilter* pTraceFilter , trace_t* pTrace ) -> void
	{
		VirtualFn( void )( IEngineTrace* , const Ray_t& , unsigned int , ITraceFilter* , trace_t* );
		return Andromeda::Memory::vget< Fn >( this , index::IEngineTrace::TraceRay )( this , ray , fMask , pTraceFilter , pTrace );
	}
};

class ICvar
{
public:
	auto FindVar( string var_name ) -> ConVar*
	{
		VirtualFn( ConVar* )( PVOID , const char* );
		return Andromeda::Memory::vget< Fn >( this , index::ICvar::FindVar )( this , var_name.c_str() );
	}

	auto ConsoleColorPrintf( const Color& clr , const char* buff ) -> void
	{
		typedef void( __cdecl* Fn )( void* , const Color& , const char* , ... );

		if ( buff == nullptr )
			return;

		Andromeda::Memory::vget< Fn >( this , index::ICvar::ConsoleColorPrintf )( this , clr , buff );
	}

	auto ConsolePrintf( const char* buff ) -> void
	{
		typedef void( __cdecl* Fn )( void* , const char* , ... );

		if ( buff == nullptr )
			return;

		Andromeda::Memory::vget< Fn >( this , index::ICvar::ConsolePrintf )( this , buff );
	}

	/*auto ConsoleColorPrintf( const Color& clr , const char* pFormat , ... ) -> void
	{
		typedef void( __cdecl* Fn )( void* , const Color& , const char* , ... );

		if ( pFormat == nullptr )
			return;

		char buf[2048] = { 0 };

		va_list list;
		va_start( list , pFormat );
		vsnprintf_s( buf , sizeof( buf ) - 1 , _TRUNCATE , pFormat , list );
		va_end( list );

		Andromeda::Memory::vget< Fn >( this , index::ICvar::ConsoleColorPrintf )( this , clr , buf , list );
	}

	auto ConsolePrintf( const char *pFormat , ... ) -> void
	{
		typedef void( __cdecl* Fn )( void* , const char* , ... );

		if ( pFormat == nullptr )
			return;

		char buf[2048] = { 0 };

		va_list list;
		va_start( list , pFormat );
		vsnprintf_s( buf , sizeof( buf ) - 1 , _TRUNCATE , pFormat , list );
		va_end( list );

		Andromeda::Memory::vget< Fn >( this , index::ICvar::ConsolePrintf )( this , buf , list );
	}*/
};

class IClientMode
{
public:
	auto CreateMove( float flInputSampleTime , CUserCmd* cmd ) -> bool
	{
		VirtualFn( bool )( PVOID , float , CUserCmd* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientMode::CreateMove )( this , flInputSampleTime , cmd );
	}
};

class IInputSystem
{
public:
	auto EnableInput( bool bEnable ) -> void
	{
		VirtualFn( void )( PVOID , bool );
		Andromeda::Memory::vget< Fn >( this , index::IInputSystem::EnableInput )( this , bEnable );
	}
	auto IsButtonDown( int code ) -> bool
	{
		VirtualFn( bool )( PVOID , int );
		return Andromeda::Memory::vget< Fn >( this , index::IInputSystem::IsButtonDown )( this , code );
	}
	auto ButtonCodeToString( int code ) -> string
	{
		VirtualFn( const char* )( PVOID , int );
		return string( Andromeda::Memory::vget< Fn >( this , index::IInputSystem::ButtonCodeToString )( this , code ) );
	}
	auto StringToButtonCode( string str ) -> int
	{
		VirtualFn( int )( PVOID , const char* );
		return Andromeda::Memory::vget< Fn >( this , index::IInputSystem::StringToButtonCode )( this , str.c_str() );
	}
	auto VirtualKeyToButtonCode( int vkey ) -> int
	{
		VirtualFn( int )( PVOID , int );
		return Andromeda::Memory::vget< Fn >( this , index::IInputSystem::VirtualKeyToButtonCode )( this , vkey );
	}
	auto ButtonCodeToVirtualKey( int code ) -> int
	{
		VirtualFn( int )( PVOID , int );
		return Andromeda::Memory::vget< Fn >( this , index::IInputSystem::ButtonCodeToVirtualKey )( this , code );
	}
};

class ISurface
{
public:
	void UnlockCursor()
	{
		VirtualFn( void )( PVOID );
		Andromeda::Memory::vget< Fn >( this , index::ISurface::UnlockCursor )( this );
	}
	void LockCursor()
	{
		VirtualFn( void )( PVOID );
		Andromeda::Memory::vget< Fn >( this , index::ISurface::LockCursor )( this );
	}
	void PlaySoundFile( string sound_name )
	{
		VirtualFn( void )( PVOID , const char* );
		Andromeda::Memory::vget< Fn >( this , index::ISurface::PlaySoundFile )( this , sound_name.c_str() );
	}
};

#pragma endregion
#ifndef defs_h
#define defs_h
#define GL_SILENCE_DEPRECATION
void errorExit(int code, std::string exitData);
std::string randomString();
void pressVKey(unsigned short key);
bool isPressed(unsigned short inKeyCode);
void click();
void trigger();
void esp();
void bhop();
bool fov = false;

pid_t csgoPid = NULL;
task_t csgoTask = NULL;
mach_vm_address_t clientModule;
mach_vm_address_t engineModule;
uint64_t glowObjectManager;

struct clr { float r, g, b, a; };
struct clrNoAlpha { float r, g, b; };
struct Vec3 { float x, y, z; };
static const char alphanum[] = "pNHXL9ynxU!ZS3Kdf4Gutl%qr7Ag0O2z@#J1RYWi*kQM5ovwTPEcDe$hjCs&a^8BVb6FmI";

//OFFSETS (thanks mrbarbossa man)
namespace offsets {
    uint64_t m_iGlowIndex = 0xAD18;
    uint64_t crosshairid = 0xbce8;
    uint64_t dwLocalPlayer = 0x14f0108;
    uint64_t dwRadarBase = 0x1e77c98;
    uint64_t dwEntityList = 0x14fc3e8;
    uint64_t dwGlowObjectManager = 0x1d64100;
    uint64_t m_Local = 0x3700;
    uint64_t m_aimPunchAngle = 0x3774;
    uint64_t m_viewPunchAngle = 0x3768;
    uint64_t m_vecViewOffset = 0x140;
    uint64_t m_nTickBase = 0x3c50;
    uint64_t m_vecVelocity = 0x14c;
    uint64_t m_iHealth = 0x138;
    uint64_t m_lifeState = 0x297;
    uint64_t m_fFlags = 0x13c;
    uint64_t m_iObserverMode = 0x3b70;
    uint64_t m_hObserverTarget = 0x3b84;
    uint64_t m_hViewModel = 0x3ad4;
    uint64_t m_szLastPlaceName = 0x3df0;
    uint64_t m_iAccount = 0xbc48;
    uint64_t m_angEyeAngles = 0xbc60;
    uint64_t m_angEyeAnglesOne = 0xbc64;
    uint64_t m_flLowerBodyYawTarget = 0x4370;
    uint64_t deadflag = 0x3998;
    uint64_t m_vecOrigin = 0x170;
    uint64_t m_nRenderMode = 0x293;
    uint64_t m_iTeamNum = 0x12c;
    uint64_t m_Collision = 0x378;
    uint64_t m_bSpotted = 0xecd;
    uint64_t m_hActiveWeapon = 0x3638;
    uint64_t m_hMyWeapons = 0x6a70;
    uint64_t m_iPing = 0x11c4;
    uint64_t m_iKills = 0x12c8;
    uint64_t m_iAssists = 0x13cc;
    uint64_t m_iDeaths = 0x14d0;
    uint64_t m_bConnected = 0x1180;
    uint64_t m_iPendingTeam = 0x16d8;
    uint64_t m_bAlive = 0x17dc;
    uint64_t m_iPlayerC4 = 0x1cf4;
    uint64_t m_iPlayerVIP = 0x1cf8;
    uint64_t m_bHostageAlive = 0x1d14;
    uint64_t m_isHostageFollowingSomeone = 0x1d20;
    uint64_t m_iHostageEntityIDs = 0x1d2c;
    uint64_t m_bombsiteCenterB = 0x1d08;
    uint64_t m_hostageRescueX = 0x1d5c;
    uint64_t m_hostageRescueY = 0x1d6c;
    uint64_t m_hostageRescueZ = 0x1d7c;
    uint64_t m_iMVPs = 0x1d8c;
    uint64_t m_iArmor = 0x1f14;
    uint64_t m_bHasHelmet = 0x1ed1;
    uint64_t m_bHasDefuser = 0x1e90;
    uint64_t m_iScore = 0x2018;
    uint64_t m_iCompetitiveRanking = 0x211c;
    uint64_t m_iCompetitiveWins = 0x2220;
    uint64_t m_iCompTeammateColor = 0x2368;
    uint64_t m_bControllingBot = 0x2674;
    uint64_t m_iControlledPlayer = 0x26b8;
    uint64_t m_iControlledByPlayer = 0x27bc;
    uint64_t m_iBotDifficulty = 0x4940;
    uint64_t m_szClan = 0x4a44;
    uint64_t m_iTotalCashSpent = 0x4e54;
    uint64_t m_iCashSpentThisRound = 0x505c;
    uint64_t m_nEndMatchNextMapVotes = 0x66f8;
    uint64_t m_bEndMatchNextMapAllVoted = 0x67fc;
    uint64_t m_nActiveCoinRank = 0x5160;
    uint64_t m_nMusicID = 0x5264;
    uint64_t m_nPersonaDataPublicLevel = 0x5368;
    uint64_t m_nPersonaDataPublicCommendsLeader = 0x546c;
    uint64_t m_nPersonaDataPublicCommendsTeacher = 0x5570;
    uint64_t m_nPersonaDataPublicCommendsFriendly = 0x5674;
    uint64_t m_bBombTicking = 0x3018;
    uint64_t m_flC4Blow = 0x3028;
    uint64_t m_bBombDefused = 0x3048;
    uint64_t m_hBombDefuser = 0x304c;
    uint64_t m_iShotsFired = 0xac90;
    uint64_t m_ArmorValue = 0xbc5c;
    uint64_t m_bIsDefusing = 0x4208;
    uint64_t m_bIsGrabbingHostage = 0x4209;
    uint64_t m_bIsScoped = 0x41fe;
    uint64_t m_bGunGameImmunity = 0x421c;
    uint64_t m_bIsPlayerGhost = 0x4375;
    uint64_t m_bIsRescuing = 0x4210;
    uint64_t m_flFlashDuration = 0xad00;
    uint64_t m_flFlashMaxAlpha = 0xacfc;
    uint64_t m_iItemDefinitionIndex = 0x37b2;
    uint64_t m_iItemIDHigh = 0x37c8;
    uint64_t m_iAccountID = 0x37d0;
    uint64_t m_iEntityQuality = 0x37b4;
    uint64_t m_szCustomName = 0x3888;
    uint64_t m_nFallbackPaintKit = 0x3a18;
    uint64_t m_nFallbackSeed = 0x3a1c;
    uint64_t m_flFallbackWear = 0x3a20;
    uint64_t m_nFallbackStatTrak = 0x3a24;
    uint64_t m_nModelIndex = 0x290;
    uint64_t m_hWeapon = 0x3070;
    uint64_t m_hOwner = 0x3074;
    uint64_t m_fAccuracyPenalty = 0x3b90;
    uint64_t m_bStartedArming = 0x3c6c;
    uint64_t m_flNextPrimaryAttack = 0x3a98;
    uint64_t m_iClip1 = 0x3ac4;
    uint64_t m_bShouldGlow = 0x3058;
};

#define TEAM_SPECTATOR 1
#define TEAM_TERRORIST 2
#define TEAM_COUNTERTERRORIST 3
#define PL_INAIR (1<<0)
#endif /* defs_h */

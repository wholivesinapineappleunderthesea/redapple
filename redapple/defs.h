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
static const char alphanum[] = "!@#wvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA9876543210*&^%$xyz";

//OFFSETS
uint64_t crosshairid = 0xbce8;
uint64_t healthOffset = 0x138;
uint64_t iDefaultFOV = 0x3B14;
uint64_t m_clrRender = 0x0A8;
uint64_t m_fFlags = 0x13C;
uint64_t teamOffset = 0x12C;
uint64_t b_isScoped = 0x41FE;
uint64_t isSpotted = 0x0ECD;
uint64_t isDefusing = 0x4208;
uint64_t isTakingHostage = 0x4209;
uint64_t m_iGlowIndex = 0xAD18;
uint64_t boneMatrix = 0x2C70;
uint64_t clrRender = 0x0A8;
uint64_t dwLocalPlayer = 0x14f0108;
uint64_t dwRadarBase = 0x1e77c98;
uint64_t dwEntityList = 0x14fc3e8;
uint64_t dwGlowObjectManager = 0x1d64100;
uint64_t localPlayerAddress; // localplayer
uint64_t m_bUseCustomAutoExposureMin = 0x0F65;
uint64_t m_bUseCustomAutoExposureMax =  0x0F66;
uint64_t m_flCustomAutoExposureMin =  0x0F68;
uint64_t m_flCustomAutoExposureMax =  0x0F6C;

#define TEAM_SPECTATOR 1
#define TEAM_TERRORIST 2
#define TEAM_COUNTERTERRORIST 3
#define PL_INAIR (1<<0)
#endif /* defs_h */

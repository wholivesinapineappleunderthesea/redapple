//CONFIGURATION:
//ESP:
#define espEnabled // COMMENT OUT TO DISABLE
//#define fullBloom // COMMENT OUT TO DISABLE

//TRIGGERBOT
#define triggerEnable // COMMENT OUT TO DISABLE

//BHOP:
#define bhopEnable // COMMENT OUT TO DISABLE

//TRIGGERBOT SPEED (MS) TODO: Get working
#define MIN_TRIGGERBOT_SPEED 26 // Min triggerbot speed
#define MAX_TRIGGERBOT_SPEED 211 // Max triggerbot speed
#define MIN_TRIGGERBOT_HOLD 99 // Min time between mouse press and release
#define MAX_TRIGGERBOT_HOLD 203 // Min time between mouse press and release

//KEYS
#define TRIGGERBOT_KEY kVK_ANSI_X // Triggerbot key
#define FORCEQUIT_KEY kVK_PageDown // Forcequit key
#define BHOP_KEY kVK_ANSI_KeypadMinus // Bunny hop key
//Colours can be changed in helper.h around line 130

//EXTRA
#define MAX_PLAYER_LIST 64 // Max of 32 players, if playing with more, set to 64

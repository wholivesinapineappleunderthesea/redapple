//CONFIGURATION:
// RADAR:
#define radarEnable true // Radar

//ESP:
#define espEnabled true
#define espAlways false // ESP has a key, turn this to true for ESP to always be on

//TRIGGERBOT
#define triggerEnable true

//BHOP:
#define bhopEnable true

//TRIGGERBOT SPEED (MS) TODO: Get working
#define MIN_TRIGGERBOT_SPEED 26 // Min triggerbot speed
#define MAX_TRIGGERBOT_SPEED 211 // Max triggerbot speed
#define MIN_TRIGGERBOT_HOLD 99 // Min time between mouse press and release
#define MAX_TRIGGERBOT_HOLD 203 // Min time between mouse press and release

//KEYS
#define TRIGGERBOT_KEY kVK_ANSI_X // Triggerbot key
#define FORCEQUIT_KEY kVK_PageDown // Forcequit key
#define BHOP_KEY kVK_ANSI_KeypadMinus // Bunny hop key
#define ESP_KEY kVK_ANSI_C
//Colours can be changed in helper.h around line 130


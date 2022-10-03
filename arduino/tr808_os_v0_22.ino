#include <EEPROM.h>

#include <Adafruit_MCP23017.h>
#include <MIDI.h>
#include <digitalWriteFast.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define  statusLed 13
//define  midiChannel 15

//max 528

#define max528_CLK 8
#define max528_DIN 9
#define max528_CS1 6
#define max528_CS2 5
#define max528_SHDN 10

byte cc1Mod = 0;
//
// TLC7226 dac :
byte dbPinTab[8] = {
  5, 6, 7, 8, 9, 10, 11, 12
};

//
unsigned long theMillis = 0;
unsigned long prevMillis = 0;

unsigned long theMicros = 0;
unsigned long prevMicros = 0;

// MCP23017
Adafruit_MCP23017 mcp;
unsigned int mcpStates = 0;
unsigned int prevMcpStates = 0;
byte mcpPinGateTab[12] = {
  4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

// gates
unsigned long gateOnTab[12][2];
unsigned long gateOffTabTimer[12];

//sequence :
word tickCount = 0;

// drum map :

//ma, cb, rs, oh, cy, ch, ht, mt, lt, bd, sn , cp,
//  49, 60, 61, 58, 56, 54, 57, 55, 53, 48, 50, 52
byte drumMap[12] = {
  1, 11, 3, 8, 10, 6, 9, 7, 5, 0, 2, 4
};
byte revDrumMap[12] = {
  9, 0, 10, 2, 11, 8, 5, 7, 3, 6, 4, 1
};
//  25, 50, 40, 50, 55, 45, 25, 25, 35, 33, 35, 37
byte voiceRangeVeloMin[12] = {
  25, 50, 40, 50, 55, 45, 25, 25, 35, 3, 35, 37
};

byte voiceDacChannel[12] = {
  5, 6, 7, 8, 8, 7, 6, 5, 4, 3, 2, 1
};

byte voiceDAC[12] = {
  2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1
};

// Accent Pot :
#define  accentReadPin 1
byte globalAccent = 0;
byte hasToResetAccent = 0;
byte currentAccentVal = 0;

unsigned long accentPotReadTime = 0;
#define accentPotReadInterval 121
unsigned int accentPotVal = 0;
unsigned int prevAccentPotVal = 0;

unsigned int accentTriggerDur = 0;

word triggerInterval = 850;
unsigned long triggerTimer = 0;

unsigned int gatesByte = 0;
unsigned long gateOffTimeTab[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

byte veloTab[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

word guiroStates = 0;
word prevGuiroStates = 0;
word guiroInterval = 44200;
unsigned long guiroTimer = 0;

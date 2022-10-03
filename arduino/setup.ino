
//==================================================================================================================
//==================================================================================================================
//================================================   SETUP    ======================================================
//==================================================================================================================
//==================================================================================================================
void setup() {


  byte midiChannel =  EEPROM.read(0) % 16;
  //byte midiChannel =  11;
  //changeMidiChannel(midiChannel);
  
  mcp.begin(); // Utilise l'adresse par défaut du mcp qui est 0

  for (byte i = 0; i < 16; i++) {
    mcp.pinMode(i, OUTPUT);
  }

  MIDI.setHandleNoteOn(handleNoteOn);
  //MIDI.setHandleNoteOff(handleNoteOff);
  //midi.setHandleClock(handleClock);
  MIDI.setHandleStart(handleStart);
  //midi.setHandleStop(handleStop);
  MIDI.setHandleControlChange(handleControlChange);
  //MIDI.setHandlePitchBend(handlePitchBend);
  //MIDI.setThruFilterMode(midi::Full);
  //MIDI.turnThruOn();

  MIDI.begin(midiChannel);

  pinMode(max528_CLK, OUTPUT);
  pinMode(max528_DIN, OUTPUT);
  pinMode(max528_CS1, OUTPUT);
  pinMode(max528_CS2, OUTPUT);
  pinMode(max528_SHDN, OUTPUT);
  digitalWrite(max528_SHDN, HIGH);

  digitalWrite(statusLed, HIGH);

  DAC_set_buffered();

}

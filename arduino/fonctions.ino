
void processNote(byte note, byte velocite)
{
  byte voiceNum = getInstrNum(note);

  if (velocite > 0 ) {

    triggerTimer = theMicros + 200;

    if ( velocite >= 110) {
      //considere la note comme un accent
      globalAccent = 1 + (velocite >> 6) + (cc1Mod >> 5);
      accentTriggerDur = (globalAccent << 12);

      //set les voix déja vues
      for (byte vnum = 0; vnum < 12; vnum++) {
        if (bitRead(gatesByte, vnum) == 1) {
          Set_DAC(veloTab[vnum], vnum);
        }
      }
    }

    if (cc1Mod > 0) {
      guiroStates = cc1Mod;
      guiroInterval = velocite << 12;
    }

    veloTab[voiceNum] = velocite;

    bitSet(mcpStates, mcpPinGateTab[voiceNum]);
    bitSet(gatesByte, voiceNum);

    Set_DAC(velocite, voiceNum);

  }
}

void Set_MCP() {
  mcp.writeGPIOAB(mcpStates);
  prevMcpStates = mcpStates;
}

void Set_DAC(byte velocite, byte voiceNum)
{
  byte dacNum = voiceDAC[voiceNum];
  byte dacChannel = voiceDacChannel[voiceNum] - 1;
  byte setdac =  1 << (dacChannel);

  if (globalAccent) {
    velocite =  max ( min( (velocite >> 1) + currentAccentVal, 127), 0);
  }

  velocite = scale( 0, 127, 0, 255, velocite, 0.2);

  DAC_CS_set(dacNum, 0);
  DAC_pushbyte(setdac);
  DAC_pushbyte(velocite);
  DAC_CS_set(dacNum, 1);
}

void DAC_CS_set(byte dacNum, byte csval)
{
  if (dacNum == 1) {
    digitalWriteFast(max528_CS1, csval);
  } else {
    digitalWriteFast(max528_CS2, csval);
  }
}

void DAC_pushbyte(byte thebyte)
{
  for (byte ii = 0; ii < 8; ii++) {
    digitalWriteFast(max528_CLK, 0);
    digitalWriteFast(max528_DIN, bitRead(thebyte, 7 - ii) );
    digitalWriteFast(max528_CLK, 1);
  }
}

void DAC_set_buffered()
{
  // no buffered 0
  // half buffered 128
  // full buffered 255
  DAC_CS_set(0, 0);
  DAC_CS_set(1, 0);
  DAC_pushbyte(0);
  DAC_pushbyte(255);
  DAC_CS_set(0, 1);
  DAC_CS_set(1, 1);
}

//===============================================================

int scale( int originalMin, int originalMax, int newBegin, int newEnd, int inputValue, float curve)
{
  int OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}

//===============================================================

byte getInstrNum(byte notenum)
{
  notenum = notenum % 12;
  return revDrumMap[notenum];
}

byte accentValue()
{
  return  scale(  0, 1024, 0, 127, accentPotVal, -2);
}

void allNotesOff()
{
  cc1Mod = 0;
  mcpStates = 0;
  gatesByte = 0;
  globalAccent = 0;
  accentTriggerDur = 0;
  Set_MCP();
}


void changeMidiChannel(byte channelNum) {
  EEPROM.write(0, channelNum);

  //feedback visuel
  for (byte ii = 0; ii < 6; ii++) {
    digitalWrite(statusLed, LOW);
    delay(200);
    digitalWrite(statusLed, HIGH);
    delay(200);
  }

}

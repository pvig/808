
void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{
  processNote(inNote, inVelocity);
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
  processNote(inNote, 0);

  if (gatesByte == 0) {
    allNotesOff();
  }
}

void handleControlChange(byte inChannel, byte inNumber, byte inValue)
{
  if (inNumber == 1) {
    cc1Mod = inValue;
  } else if (inNumber == 123) {
    //all notes off
    allNotesOff();
  } else if (inNumber == 127) {
    //change midi channel
    changeMidiChannel(inValue);
  }
  
}

void handlePitchBend(byte inChannel, int val) {

}

void handleStart() {
  allNotesOff();
}

//==================================================================================================================
//==================================================================================================================
//=================================================   LOOP    ======================================================
//==================================================================================================================
//==================================================================================================================
void loop () {


  theMillis = millis();
  theMicros =  micros();

  // ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  GUIRO

  if ( (theMicros - guiroTimer) >= guiroInterval) {
    prevGuiroStates = guiroStates;
    guiroStates = (guiroStates >> 1);
    guiroInterval = guiroInterval >> 1;

    if (guiroStates + prevGuiroStates > 0) {
      mcpStates <<=  (cc1Mod >> 4);
    }

    guiroTimer = theMicros;
  }

  // ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  TRIGGER

  if ((theMicros - triggerTimer) >= triggerInterval + accentTriggerDur) {

    if (mcpStates != prevMcpStates) {
      Set_MCP();
      mcpStates = 0;
      gatesByte = 0;
    } else {
      globalAccent = 0;
      accentTriggerDur = 0;
    }

    triggerTimer = theMicros;
  }

  // .......................       ....................       ................   ACCENT POT  ----------------

  if ( (theMillis - accentPotReadTime) >= accentPotReadInterval) {
    accentPotVal = analogRead(accentReadPin);
    if (prevAccentPotVal != accentPotVal) {
      currentAccentVal = accentValue();
    }
    prevAccentPotVal = accentPotVal;
    accentPotReadTime = theMillis;
  }

  // ==  ==  ==  MIDI reception  ==  ==  ==

  MIDI.read();

  prevMillis = theMillis;
  prevMicros = theMicros;
}


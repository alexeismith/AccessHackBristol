
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Team1_glissandoDetect.h"

#define GLISS_MIDI_CC 106
#define PITCH_MAPPING_MIDI_CC 107

#define COPPER_THRESHOLD 50

const int myInput = AUDIO_INPUT_MIC;
const int lowfreqlim = 250;
const int highfreqlim = 1050;
const int lowCClim = 0;
const int highCClim = 127;

AudioInputI2S audioInput;           // xy=295,80
AudioAnalyzeNoteFrequency notefreq; // xy=528,86
AudioSynthWaveformSine sinewave;
AudioControlSGTL5000 audioShield;
float f0Frame[FRAME_SIZE];
PitchSlideResult result;
float note;
float prob;
bool glissOn;
int pitchCCvalue;
int A2min = 1023;
int A3min = 1023;

bool pitchToggle = false;
bool glissToggle = false;

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, notefreq, 0);
// AudioConnection patchCord1(sinewave, 0, notefreq, 0);

// the MIDI channel number to send messages
const int channel = 1;

void push(float arr[], int size, float newValue)
{

  for (int i = 1; i < size; i++)
  {
    arr[i - 1] = arr[i]; // Shift each element left
  }

  arr[size - 1] = newValue; // Place the first element at the end
}

void setup()
{
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(30);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);
  audioShield.micGain(10);

  // Begin with threshold for pitch detection
  notefreq.begin(0.3);

  // Create a synthetic sine wave, for testing
  // To use this, edit the connections above
  sinewave.amplitude(0.8);
  sinewave.frequency(263);

  // fill frame with zeros
  for (auto i = 0; i < FRAME_SIZE; i++)
  {
    f0Frame[i] = 0.0f;
  }
}

void loop()
{

  // copper tape stuff
  //   A2min = min(analogRead(A2), A2min);
  // A3min = min(analogRead(A3), A3min);
  // Serial.print("A2:");
  // Serial.print(A2min);
  // Serial.print("\n");
  // Serial.print("A3:");
  // Serial.print(A3min);
  // Serial.print("\n");

  if (analogRead(A2) < COPPER_THRESHOLD)
  {
    pitchToggle = !pitchToggle;
  }
  if (analogRead(A3) < COPPER_THRESHOLD)
  {
    glissToggle = !glissToggle;
  }

  if (notefreq.available())
  {

    Serial.print("pitchToggle:");
    Serial.print(pitchToggle);
    Serial.print("\n");
    Serial.print("glissToggle:");
    Serial.print(glissToggle);
    Serial.print("\n");

    note = notefreq.read();
    prob = notefreq.probability();

    // clamp note for pitchCC
    float tempNote = 0.0f;
    if (note < lowfreqlim)
    {
      tempNote = lowfreqlim;
    }
    else if (note > highfreqlim)
    {
      tempNote = highfreqlim;
    }
    else
    {
      tempNote = note;
    }

    // update pitch CC value
    pitchCCvalue = map(tempNote, lowfreqlim, highfreqlim, lowCClim, highCClim);

    // send pitch CC signal
    if (pitchToggle)
    {
      usbMIDI.sendControlChange(PITCH_MAPPING_MIDI_CC, pitchCCvalue, 1);
    }

    // Serial.print("PitchCCvalue:");
    // Serial.print(pitchCCvalue);
    // Serial.print("\n");

    // Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);

    // Serial.print("Note:");
    // Serial.print(note);
    // Serial.print("\n");

    // add F0 to vector
    push(f0Frame, FRAME_SIZE, note);

    // Serial.printf("frame: %g %g %g %g %g\n", f0Frame[0], f0Frame[1], f0Frame[2], f0Frame[3], f0Frame[4]);

    // decide if FRAME contains a glissando
    detect_pitch_slide(f0Frame, &result);

    // Serial.print("length:");
    // Serial.print(result.length);
    // Serial.print("\n");

    // Serial.print("extent:");
    // Serial.print(result.extent);
    // Serial.print("\n");

    // Serial.print("direction:");
    // Serial.print(result.trend);
    // Serial.print("\n");

    if (result.length >= GLISS_DURATION_THRESHOLD && fabs(result.extent) >= GLISS_EXTENT_THRESHOLD)
    {
      glissOn = true;
      result.length = 0;
      result.extent = 0.0f;
      result.trend = false;

      // send MIDI 127 signal
      if (glissToggle)
      {
        usbMIDI.sendControlChange(GLISS_MIDI_CC, 127, 1);
      }

      // reset frame
      for (auto i = 0; i < FRAME_SIZE; i++)
      {
        f0Frame[i] = 0.0f;
      }
    }
    else
    {
      glissOn = false;

      // send MIDI 0 signal
      if (glissToggle)
      {
        usbMIDI.sendControlChange(GLISS_MIDI_CC, 0, 1);
      }
    }

    // Serial.print("gliss:");
    // Serial.print(glissOn);
    // Serial.print("\n");
  }

  // have loop run roughly every 1ms
  // delay(REPORT_PERIOD_MS);
}

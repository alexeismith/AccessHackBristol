
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Team1_glissandoDetect.h"

const int myInput = AUDIO_INPUT_MIC;

// GUItool: begin automatically generated code
AudioInputI2S audioInput;           // xy=295,80
AudioAnalyzeNoteFrequency notefreq; // xy=528,86
AudioSynthWaveformSine sinewave;
AudioControlSGTL5000 audioShield;
float f0Frame[FRAME_SIZE];
PitchSlideResult result;
float note;
float prob;
bool glissOn;

// GUItool: end automatically generated code

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, notefreq, 0);
// AudioConnection patchCord1(sinewave, 0, notefreq, 0);

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

  if (notefreq.available())
  {
    note = notefreq.read();
    prob = notefreq.probability();

    // Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);

    // plot note
    // Serial.print("Note:");
    // Serial.print(note);
    // Serial.print("\n");

    // add F0 to vector
    push(f0Frame, FRAME_SIZE, note);

    // Serial.printf("frame: %g %g %g %g %g\n", f0Frame[0], f0Frame[1], f0Frame[2], f0Frame[3], f0Frame[4]);

    // decide if FRAME contains a glissando
    result = detect_pitch_slide(f0Frame);

    // Serial.print("length:");
    // Serial.print(result.length);
    // Serial.print("\n");

    // Serial.print("extent:");
    // Serial.print(result.extent);
    // Serial.print("\n");

    if (result.length >= GLISS_DURATION_THRESHOLD && fabs(result.extent) >= GLISS_EXTENT_THRESHOLD)
    {
      glissOn = true;
      result.length = 0;
      result.extent = 0.0f;
    }
    else
    {
      glissOn = false;
    }

    Serial.print("gliss:");
    Serial.print(glissOn);
    Serial.print("\n");
  }

  // have loop run roughly every 1ms
  // delay(REPORT_PERIOD_MS);
}

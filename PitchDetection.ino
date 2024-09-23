
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const int myInput = AUDIO_INPUT_MIC;

// GUItool: begin automatically generated code
AudioInputI2S            audioInput;           //xy=295,80
AudioAnalyzeNoteFrequency notefreq;      //xy=528,86
AudioSynthWaveformSine sinewave;
AudioControlSGTL5000 audioShield;

// GUItool: end automatically generated code

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, notefreq, 0);
// AudioConnection patchCord1(sinewave, 0, notefreq, 0);



void setup() {
    // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(30);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);
  audioShield.micGain(20);

 // Begin with threshold for pitch detection
  notefreq.begin(.5);

  // Create a synthetic sine wave, for testing
  // To use this, edit the connections above
  sinewave.amplitude(0.8);
  sinewave.frequency(263);
}

void loop() {

   if (notefreq.available()) {
    float note = notefreq.read();
    float prob = notefreq.probability();
    // Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    Serial.print("Note:");
    Serial.print(note);  
    Serial.print("\n");
    // } else {
    // Serial.printf("  Frequency detection unavailable  \n"); // don't print "0.00"
  }

}

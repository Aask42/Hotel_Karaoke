// Sound Activated EL Wire Dress
// Blink EL Wire to music and other ambient sound.
// Aask

#include "arduinoFFT.h" // Standard Arduino FFT library

arduinoFFT FFT = arduinoFFT(); // Create FFT object

// Set up the mic for audio capture
#define SAMPLES 32
#define SAMPLING_FREQUENCY 7000

int default_decay = 0;
int decay[5];

unsigned int sampling_period_us;
//byte peak[] = {0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
int i = 0;
int peak;

void setup() {
  Serial.begin(9600);  
  // The EL channels are on pins 2 through 9
  // Initialize the pins as outputs
  pinMode(2, OUTPUT);  // channel A  
  pinMode(3, OUTPUT);  // channel B  
  pinMode(4, OUTPUT);  // channel C  

//Initialize input pins on EL Sequencer
  //pinMode(A2, INPUT);

  // Set up Microphone
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);

  decay[0] = default_decay;
  decay[1] = default_decay;
  decay[2] = default_decay;
  
  delay(250);
}
void loop() 
{
  // Enter real-time audio response mode ^_^
  // Read from the mic until you hit the max datapoints (set above)
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros()-oldTime;
    oldTime = newTime;
    vReal[i] = analogRead(A7); // A conversion takes about 1uS on an ESP32
    vImag[i] = 0;
    while (micros() < (newTime + sampling_period_us)) { /* do nothing to wait */ }
  }

  //FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  //FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  //FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  
  for (int i = 1; i < (SAMPLES/2); i++){ // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
      if (vReal[i] > 550) {
        //peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
        Serial.println(vReal[i]);
       //for(int p = 1; p <= 6; p += 1){
          if (vReal[i] >= 550) {
              digitalWrite(2, HIGH); //turn EL channel on
              decay[0] = default_decay+12; // A-range - 400ish Hz, around an A
          }
          if (vReal[i] > 625 ){ 
              digitalWrite(3, HIGH); //turn EL channel on
              decay[1] = default_decay+8; // B
          }
          if (vReal[i] > 660) {
              digitalWrite(4, HIGH); //turn EL channel on
              decay[2] = default_decay+ 6; // C
            }
        //}
      }
  }

  if ( decay[0] == 0 ) digitalWrite(2, LOW); //turn EL channel off
  else decay[0] -= 1;
  
  if ( decay[1] == 0 ) digitalWrite(3, LOW); //turn EL channel off
  else decay[1] -= 1;
  
  if ( decay[2] == 0 ) digitalWrite(4, LOW); //turn EL channel off
  else decay[2] -= 1;
 

  
}

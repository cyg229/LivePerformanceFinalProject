#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>
#include <Arduino_LSM6DS3.h>
#include <Wire.h>

const int numReadings = 20;
const int flexPin = A1; //pin A0 to read analog input
int value; //save analog value
int value1;
int last;


int readIndex = 0;
int total = 0;
int average = 0;
int vy;


const int numReadings2 = 2;
int readIndex2 = 0;
int total2 = 0;
int average2 = 0;
int vy2;

int pRed = 1;
int oldVal;
int note = 7;
static int finalV;

void setup() {
  Serial.begin(9600);

  pinMode(note, INPUT_PULLUP);
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");
}

void loop() {
  
  float x, y, z;
  
  if (IMU.gyroscopeAvailable()) {
    
    IMU.readGyroscope(x, y,z );

  total = total - vy;
  vy = y;
  total = total + vy;
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = total / numReadings;

  float  posAverage = map(abs(average), 0, 30, 0, 60);
   int noteVal = digitalRead(note);
     
   value1 = analogRead(flexPin); 
     // Serial.println(value1);

   value1 = map(value1, 100, 1023, 0, 100);//Map value 0-1023 to 0-255 (PWM)  
    
  if (oldVal != noteVal) {
    oldVal = noteVal;
        delay(30);

    value = analogRead(flexPin); 
    value = map(value, 100, 1023, 0, 100);   
        delay(30);

    
    if (value >= 0 && value <= 20) {
      value =51;
       if (noteVal == 0 && pRed == 1) {
        noteOn(0, value, 127);
        finalV = value;
        pRed = 0;
        delay(30);
       }
  
      if (noteVal == 1 && pRed == 0) {
        noteOn(0, finalV, 0);
        pRed=1;
        delay(30);
  
     
      }
    }
    if (value >= 21 && value <= 50) {
      value =56; 
       if (noteVal == 0 && pRed == 1) {
        noteOn(0, value, 127);
        finalV = value;
        pRed = 0;
        delay(30);
       }
  
      if (noteVal == 1 && pRed == 0) {
        noteOn(0, finalV, 0);
        pRed=1;
        delay(30);
  
     
      }
    }
    if (value >= 51 && value <= 84) {
      value =58; 
       if (noteVal == 0 && pRed == 1) {
        noteOn(0, value, 127);
        finalV = value;
        pRed = 0;
        delay(30);
       }
  
      if (noteVal == 1 && pRed == 0) {
        noteOn(0, finalV, 0);
        pRed=1;
        delay(30);
  
     
      }
    }
    if (value >= 85 && value <= 100) {
      value = 60; 
       if (noteVal == 0 && pRed == 1) {
        noteOn(0, value, 127);
        finalV = value;
        pRed = 0;
        delay(30);
       }
  
      if (noteVal == 1 && pRed == 0) {
        noteOn(0, finalV, 0);
        pRed=1;
        delay(30);
  
     
      }
    }
  }
       controlChange(1, 20 , posAverage);
  }
  

}
   
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

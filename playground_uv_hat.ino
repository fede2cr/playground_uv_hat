/*************************************************** 
  Sunscreen reminder hat using the Si1145 UV/IR/Visible Light Sensor
  visit https://learn.adafruit.com/sunscreen-reminder-hat for full tutorial
  
  contains Super Mario Bros theme composed for piezo by Tiago Gala: http://therandombit.wordpress.com/2011/11/21/arduino-piezo-speaker-super-mario/
  
  Written by Becky Stern for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include "Adafruit_SI1145.h"

Adafruit_SI1145 uv = Adafruit_SI1145();

//tones for reminder tune
#define toneC    1911
#define toneC1    1804
#define toneD    1703
#define toneEb    1607
#define toneE    1517
#define toneF    1432
#define toneF1    1352
#define toneG    1276
#define toneAb    1204
#define toneA    1136
#define toneBb    1073
#define toneB    1012
#define tonec       955
#define tonec1      902
#define toned       851
#define toneeb      803
#define tonee       758
#define tonef       716
#define tonef1      676
#define toneg       638
#define toneab      602
#define tonea       568
#define tonebb      536
#define toneb       506
#define tonep       0 
int speaker = 5; // piezo wired to FLORA TX


uint32_t sinceTime;
uint32_t markerTime;
boolean shouldChime = false;
float UVindex;
float UVthreshold = 0.05;
uint32_t reapplyInterval = 10000; // 15 minutes = 900000 milliseconds. One hour = 3600000 milliseconds. Two hours = 7200000 milliseconds

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  Serial.println("Adafruit SI1145 test");
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }
  Serial.println("OK!");
}

void loop() {
  Serial.println("===================");
  UVindex = uv.readUV(); 
  UVindex /= 100.0; // the index is multiplied by 100 so to get the integer index, divide by 100!  
  Serial.print("UV: ");  Serial.println(UVindex);
  Serial.print("Seconds until next alert: ");  Serial.println((reapplyInterval-sinceTime)/1000);
  delay(1000);
  
  sinceTime = millis()-markerTime;
  
  if (UVindex > UVthreshold && shouldChime){ //only chime when we are currently outside
    chime();
    Serial.println("===================");
    Serial.println("CHIME");
    shouldChime = false;
    resetTimer();
  }
  if (sinceTime > reapplyInterval) { //check to see if we've exceeded the time limit
    shouldChime = true;
    resetTimer; 
  }
  
}

void resetTimer(){
  markerTime = millis();
  sinceTime = 0;
}

void chime(){
int melody[] = {tonec, toneG, toneE, toneA, toneB, toneBb, toneA, toneG, tonee, toneg, tonea, tonef, toneg, tonee, tonec, toned, toneB};
int rhythm[] = {18, 18, 18, 12, 12, 6, 12, 8, 8, 8, 12, 6, 12, 12, 6, 6, 6};
long vel = 20000;

for (int i=0; i<17; i++) {
    int note = melody[i];
    int tempo = rhythm[i];
 
    long tvalue = tempo * vel;
 
    //tocar(note, tvalue);
    long tempo_progress = 0;
  while (tempo_progress < tvalue) {
    digitalWrite(speaker, HIGH);
    delayMicroseconds(note / 2);
 
    digitalWrite(speaker, LOW);
    delayMicroseconds(note/2);	 
    tempo_progress += note;
  }
 
    delayMicroseconds(1000);
  }

}

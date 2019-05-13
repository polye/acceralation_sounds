#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"


SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  // put your setup code here, to run once:

  mySoftwareSerial.begin(9600);    //シリアルポートを開き、転送速度を9600bpsに設定  mp3_set_serial (Serial);      //set Serial for DFPlayer-mini mp3 module
  // Serial.begin(115200);

  pinMode(6, INPUT);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(21);
  myDFPlayer.loop(1);
}

void loop() {
    myDFPlayer.playFolder(1, 2);
    delay(5000);
    myDFPlayer.playFolder(1, 3);
    delay(5000);
    myDFPlayer.playFolder(1, 4);
    delay(5000);
    myDFPlayer.playFolder(1, 5);
    delay(5000);
    myDFPlayer.playFolder(1, 6);
    delay(5000);
    myDFPlayer.playFolder(1, 7);
    delay(5000);
    myDFPlayer.playFolder(1, 8);
    delay(5000);
    myDFPlayer.playFolder(1, 9);
    delay(5000);
    myDFPlayer.playFolder(1, 10);
    delay(5000);
    myDFPlayer.playFolder(1, 11);
    delay(5000);
    myDFPlayer.playFolder(1, 12);
    delay(5000);
    myDFPlayer.playFolder(1, 13);
    delay(5000);
    
}

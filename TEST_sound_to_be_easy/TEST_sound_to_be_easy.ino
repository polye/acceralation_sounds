#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
//    Serial.println();
//    Serial.print("Com Error ");
//    Serial.println(errorCode);
  }

  static void OnPlayFinished(uint16_t globalTrack)
  {
//    Serial.println();
//    Serial.print("Play finished for #");
//    Serial.println(globalTrack);   
  }

  static void OnCardOnline(uint16_t code)
  {
//    Serial.println();
//    Serial.print("Card online ");
//    Serial.println(code);     
  }

  static void OnUsbOnline(uint16_t code)
  {
//    Serial.println();
//    Serial.print("USB Disk online ");
//    Serial.println(code);     
  }

  static void OnCardInserted(uint16_t code)
  {
//    Serial.println();
//    Serial.print("Card inserted ");
//    Serial.println(code); 
  }

  static void OnUsbInserted(uint16_t code)
  {
//    Serial.println();
//    Serial.print("USB Disk inserted ");
//    Serial.println(code); 
  }

  static void OnCardRemoved(uint16_t code)
  {
//    Serial.println();
//    Serial.print("Card removed ");
//    Serial.println(code);  
  }

  static void OnUsbRemoved(uint16_t code)
  {
//    Serial.println();
//    Serial.print("USB Disk removed ");
//    Serial.println(code);  
  }
};
DFMiniMp3<HardwareSerial, Mp3Notify> mp3(Serial);//1

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

mp3.begin();
mp3.setVolume(20);//


}

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  
mp3.playMp3FolderTrack(1);  // sd:/mp3/0001.mp3
waitMilliseconds(1000);

mp3.playMp3FolderTrack(2); // sd:/mp3/0002.mp3
waitMilliseconds(1000);

mp3.playMp3FolderTrack(3); // sd:/mp3/0002.mp3
waitMilliseconds(1000);
}

#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

#define numx 0
#define numy 1
#define numz 2

uint32_t timer;//32ビット非負整数を格納
double velX = 0, velY = 0, velZ = 0; //生の三軸速度
double comVelX = 0, comVelY = 0, comVelZ = 0; //訂正後の三軸速度
double disX = 0, disX_print = 0, disY = 0, disZ = 0; //変位test
double disR = 0;
double velR = 0; //速度の絶対値
double accX = 0, accY = 0, accZ = 0; //生の三軸加速度
double comAccX = 0, comAccY = 0, comAccZ = 0; //訂正後の三軸加速度
double gravityX = 0, gravityY = 0, gravityZ = 0; //重力成分
const double alpha = 0.8;//lowpassfilterの係数
const double beta = 0.9;//0に収束させるためのやつ
int n = 8;
int i_acc = 0;
int count = 0;
double diff[8];
int checker = 0;
int Threshold_x = 30;
int Threshold_y = 30;
int Threshold_z = 30;

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
  accX = analogRead(numx);
  accY = analogRead(numy);
  accZ = analogRead(numz);

  delay(10);//Wait for sensor to stablize

  timer = micros();
  mp3.begin();
  delay(1000);
  mp3.setVolume(10);//


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
//
//  mp3.playMp3FolderTrack(1);  // sd:/mp3/0001.mp3
//  waitMilliseconds(1000);
//
//  mp3.playMp3FolderTrack(2); // sd:/mp3/0002.mp3
//  waitMilliseconds(1000);
//
//  mp3.playMp3FolderTrack(3); // sd:/mp3/0002.mp3
//  waitMilliseconds(1000);

  //
  int check_x = 0;
  int check_y = 0;
  int check_z = 0;
  accX = analogRead(numx);
  accY = analogRead(numy);
  accZ = analogRead(numz);

  //dtを求める。micros()は開始からの時間(ミリ秒)を表す。
  double dt = (double)(micros() - timer) * 5 / 100000; ///1000000は謎 *100
  timer = micros(); //現在の時刻の再設定

  // 重力加速度を求める
  gravityX = alpha * gravityX + (1 - alpha) * accX;
  gravityY = alpha * gravityY + (1 - alpha) * accY;
  gravityZ = alpha * gravityZ + (1 - alpha) * accZ;


  // 補正した加速度
  comAccX = accX - gravityX;
  comAccY = accY - gravityY;
  comAccZ = accZ - gravityZ;

  //平滑化（ノイズ除去）highpassfilter

  if (abs(comAccX) < 10)comAccX = 0;
  if (abs(comAccY) < 10)comAccY = 0;
  if (abs(comAccZ) < 10)comAccZ = 0;



  velX = velX + comAccX * dt;
  velY = velY + comAccY * dt;
  velZ = velZ + comAccZ * dt;

  disX = disX + velX * dt;
  disY = disY + velY * dt;
  disZ = disZ + velZ * dt;

  velR = sqrt(velX * velX + velY * velY + velZ * velZ);
  disR = sqrt(disX * disX + disY * disY + disZ * disZ);

  //山の判定のための配列
  for (i_acc = 0; i_acc < n - 1; i_acc++) {
    diff[i_acc] = diff[i_acc + 1];
  }
  diff[n - 1] = velR;

  //グラフの山を判定
  if ((diff[0] < diff[2]) && (diff[4] > diff[6]))checker = 1000;


  if (( abs(disX) > Threshold_x ) && ( checker == 1000 ) && (abs(disX) > abs(disY)) && (abs(disX) > abs(disZ))) {
    //tone(6, 262, 300);
    check_x = 1200;
    delay(10);
  }
  if (( abs(disY) > Threshold_y ) && ( checker == 1000 ) && (abs(disY) > abs(disX)) && (abs(disY) > abs(disZ))) {
    //tone(6, 262, 300);
    check_y = 1200;
    delay(10);
  }
  if (( abs(disZ) > Threshold_z ) && ( checker == 1000 ) && (abs(disZ) > abs(disY)) && (abs(disZ) > abs(disX))) {
    //tone(6, 262, 300);
    check_z = 1200;
    delay(10);
  }
  if (check_x > 1000) {
    //mySoftwareSerial.listen();
    //myDFPlayer.playFolder(1, 2);
    //delay(500);
    mp3.playMp3FolderTrack(1);  // sd:/mp3/0001.mp3
//    waitMilliseconds(1000);
  }
  if (check_y > 1000) {
    //mySoftwareSerial.listen();
    //myDFPlayer.playFolder(1, 8);
    //Serial.listen();
    //delay(500);
    mp3.playMp3FolderTrack(2); // sd:/mp3/0002.mp3
//    waitMilliseconds(1000);
  }
  if (check_z > 1000) {
    //myDFPlayer.playFolder(1, 12);
    //delay(500);
    mp3.playMp3FolderTrack(3); // sd:/mp3/0002.mp3
//    waitMilliseconds(1000);
  }


  // disX = disX * 0.5;
  disX_print = disX * 0.7;



  //  Serial.print(velX); Serial.print("\t");
  //  Serial.print(velY); Serial.print("\t");
  //  Serial.print(velZ); Serial.print("\t");

  //    Serial.print(accX); Serial.print("\t");
  //    Serial.print(accY); Serial.print("\t");
  //    Serial.print(accZ); Serial.print("\t");
  //  Serial.print(disX); Serial.print("\t");
  //  Serial.print(disY); Serial.print("\t");
  //  Serial.print(disZ); Serial.print("\t");
  //  Serial.print(disR); Serial.print("\t");
  //  Serial.print(check_x); Serial.print("\t");
  //  Serial.print(check_y); Serial.print("\t");
  //  Serial.print(check_z); Serial.print("\t");
  //  //  Serial.print(checker); Serial.print("\t");
  //  Serial.print(velR); Serial.print("\t");

  /*
    Serial.print(comAccX); Serial.print("\t");
    Serial.print(comAccY); Serial.print("\t");
    Serial.print(comAccZ); Serial.print("\t");
  */
  //  Serial.print("\r\n");********
  check_x = 0;
  check_y = 0;
  check_z = 0;
  //数値初期化のためのカウント（一定時間で速度初期化）
  if (count <= 52)count++;
  if (count == 50) {
    velX = 0;
    velY = 0;
    velZ = 0;
    disX = 0;
    disY = 0;
    disZ = 0;
  }
  velX = velX * beta;
  velY = velY * beta;
  velZ = velZ * beta;
  disX = disX * beta;
  disY = disY * beta;
  disZ = disZ * beta;
  checker = 0;


  delay(2);


}

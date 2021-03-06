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
int Threshold_x = 40;
int Threshold_y = 40;
int Threshold_z = 40;
void setup()
{
  Serial.begin(115200);
  //  Serial.begin(9600);**
  //  Serial.println("start");
  //  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  //  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));**

  for (i_acc = 0; i_acc < n; i_acc++) {
    diff[i_acc] = 0;
  }

  accX = analogRead(numx);
  accY = analogRead(numy);
  accZ = analogRead(numz);

  delay(10);//Wait for sensor to stablize

  timer = micros();

}

void loop()
{
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

  /*********************************************************************
    if(abs(velX) < 100)velX = 0;
    if(abs(velY) < 100)velY = 0;
    if(abs(velZ) < 100)velZ = 0;
  */
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
  }
  if (check_y > 1000) {
    //mySoftwareSerial.listen();
    //myDFPlayer.playFolder(1, 8);
    //Serial.listen();
    //delay(500);
  }
  if (check_z > 1000) {
    //myDFPlayer.playFolder(1, 12);
    //delay(500);
  }


  // disX = disX * 0.5;
  disX_print = disX * 0.7;



  //  Serial.print(velX); Serial.print("\t");
  //  Serial.print(velY); Serial.print("\t");
  //  Serial.print(velZ); Serial.print("\t");

  //    Serial.print(accX); Serial.print("\t");
  //    Serial.print(accY); Serial.print("\t");
  //    Serial.print(accZ); Serial.print("\t");
  Serial.print(disX); Serial.print("\t");
  Serial.print(disY); Serial.print("\t");
  Serial.print(disZ); Serial.print("\t");
  Serial.print(disR); Serial.print("\t");
  Serial.print(check_x); Serial.print("\t");
  Serial.print(check_y); Serial.print("\t");
  Serial.print(check_z); Serial.print("\t");
  //  Serial.print(checker); Serial.print("\t");
  Serial.print(velR); Serial.print("\t");

  /*
    Serial.print(comAccX); Serial.print("\t");
    Serial.print(comAccY); Serial.print("\t");
    Serial.print(comAccZ); Serial.print("\t");
  */
  Serial.print("\r\n");
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

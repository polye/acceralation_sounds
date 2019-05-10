//加速度から速度を算出するプログラム

#define numx 3
#define numy 4
#define numz 5
uint32_t timer;//32ビット非負整数を格納

//Velocity
double velX=0,velY=0,velZ = 0;//生の三軸速度
double comVelX=0, comVelY=0, comVelZ=0;//訂正後の三軸速度
double velR=0;//速度の絶対値
double accX=0,accY=0,accZ=0;//生の三軸加速度
double comAccX=0,comAccY=0,comAccZ=0; //訂正後の三軸加速度
double gravityX=0,gravityY=0,gravityZ=0; //重力成分
const double alpha = 0.9;//lowpassfilterの係数

//Log
char report[80];//記録用配列

void setup()
{
  Serial.begin(9600) ;
  Serial.println("start");

  delay(100);//Wait for sensor to stablize

  accX = analogRead(numx); 
  accY = analogRead(numy);
  accZ = analogRead(numz);
  
  timer = micros();
}

void loop()
{
  accX = analogRead(numx);
  accY = analogRead(numy);
  accZ = analogRead(numz);
  
  //dtを求める。micros()は開始からの時間(ミリ秒)を表す。
  double dt = (double)(micros() - timer) / 1000000; ///1000000は謎
  timer = micros(); //現在の時刻の再設定
  
  // 重力加速度を求める
  gravityX = alpha * gravityX + (1 - alpha) * accX;
  gravityY = alpha * gravityY + (1 - alpha) * accY;
  gravityZ = alpha * gravityZ + (1 - alpha) * accZ;

  // 補正した加速度
  comAccX = accX - gravityX;
  comAccY = accY - gravityY;
  comAccZ = accZ - gravityZ;


  if(abs(velX) < 100)velX = 0;
  if(abs(velY) < 100)velY = 0;
  if(abs(velZ) < 100)velZ = 0;

  velX = velX + comAccX*dt;
  velY = velY + comAccY*dt;
  velZ = velZ + comAccZ*dt;
  velR = sqrt(velX*velX + velY*velY + velZ*velZ);

  /*
 if( x > 500){
  tone(6, 262, 300);
  delay(100);
 }
  if( y > 500){
  tone(6, 294, 300);
  delay(100);
 }
 if( z > 500){
  tone(6, 330, 300);
  delay(100);
 }
 
  Serial.print("X:") ;
  Serial.print(x) ;
  Serial.print(" Y:") ;
  Serial.print(y) ;
  Serial.print(" Z:") ;
  Serial.print(z) ;
  
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  Serial.println("");
  delay(10) ;
  */
  
  /*print*/
  /*
  Serial.print(velX); Serial.print("\t");
  Serial.print(velY); Serial.print("\t");
  Serial.print(velZ); Serial.print("\t");
  */

  Serial.print(velX); Serial.print("\t");
  Serial.print(velY); Serial.print("\t");
  Serial.print(velZ); Serial.print("\t");

  Serial.print(velR); Serial.print("\t");
  
  /*
  Serial.print(comAccX); Serial.print("\t");
  Serial.print(comAccY); Serial.print("\t");
  Serial.print(comAccZ); Serial.print("\t");
  */
  Serial.print("\r\n");

  delay(2);
}

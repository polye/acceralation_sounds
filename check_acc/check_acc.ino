#define numx 0
#define numy 1
#define numz 2
#define numX 4
#define numY 3
#define numZ 2

uint32_t timer;//32ビット非負整数を格納

struct check {
  double velX = 0, velY = 0, velZ = 0; 
  double disX = 0, disY = 0, disZ = 0; 
  double disR = 0;
  double velR = 0; //速度の絶対値
  double accX = 0, accY = 0, accZ = 0; //三軸加速度測定値
  double comAccX = 0, comAccY = 0, comAccZ = 0; //ローパス、ハイパス後の三軸加速度
  int direction_x = 0;
  int shock_x = 0;
  int direction_y = 0;
  int shock_y = 0;
  int direction_z = 0;
  int shock_z = 0;
  double diff[8];
  int d_x;
  int d_y;
  int d_z;
  int s_x;
  int s_y;
  int s_z;
  int checker = 0;
}r,l;

/*判定
r_direction_x =  1200 -> 右足上
r_direction_x = -1200 -> 右足下
r_shock_x     =  1000 -> 衝突
r_direction_y =  1200 -> 右足上
r_direction_y = -1200 -> 右足下
r_shock_y     =  1000 -> 衝突
r_direction_z =  1200 -> 右足上
r_direction_z = -1200 -> 右足下
r_shock_z     =  1000 -> 衝突
l_direction_x =  1200 -> 右足上
l_direction_x = -1200 -> 右足下
l_shock_x     =  1000 -> 衝突
l_direction_y =  1200 -> 右足上
l_direction_y = -1200 -> 右足下
l_shock_y     =  1000 -> 衝突
l_direction_z =  1200 -> 右足上
l_direction_z = -1200 -> 右足下
l_shock_z     =  1000 -> 衝突
*/

const double alpha = 0.8;//lowpassfilterの係数
const double beta = 0.9;
int n = 8;
int count = 0;



void setup() {
  Serial.begin(115200);
  pinMode(numx, INPUT);
  pinMode(numy, INPUT);
  pinMode(numz, INPUT);
  
  for (int i = 0; i < n; i++) {
    r.diff[i] = 0;
  }

  r.accX = analogRead(numx);
  r.accY = analogRead(numy);
  r.accZ = analogRead(numz);

  delay(10);//Wait for sensor to stablize

  timer = micros();

   r.d_x = 900;
   r.d_y = 900;
   r.d_z = 900;
   l.d_x = 900;
   l.d_y = 900;
   l.d_z = 900;
}

double get_comAcc(double acc){
  // 重力加速度を求める　ローパスフィルター
  double gravity = alpha * gravity + (1 - alpha) * acc;
  
  // 補正した加速度　ハイパスフィルター
  double comAcc = acc - gravity;
 
  if (abs(comAcc) < 10)comAcc = 0;

  return comAcc;
}
/*
double get_theother(int pin){
  double duration_H = pulseIn(pin, HIGH);
  double duration_L = pulseIn(pin, LOW);
  double duty = duration_H / (duration_H + duration_L);
  double str_send_data = 1024*duty;
  int value = (int)str_send_data;
  return value;
}
*/

void loop() {
  //analogReference(EXTERNAL);
  r.accX = analogRead(numx);
  r.accY = analogRead(numy);
  r.accZ = analogRead(numz);
 /* l.accX = get_theother(numX);
  l.accY = get_theother(numY);
  l.accZ = get_theother(numZ);
  */

  //dtを求める。micros()は開始からの時間(ミリ秒)を表す。
  double dt = (double)(micros() - timer) * 5 / 100000; ///1000000は謎 *100
  timer = micros(); //現在の時刻の再設定

  r.comAccX = get_comAcc(r.accX);
  r.comAccY = get_comAcc(r.accY);
  r.comAccZ = get_comAcc(r.accZ);
 /* l.comAccX = get_comAcc(l.accX);
  l.comAccY = get_comAcc(l.accY);
  l.comAccZ = get_comAcc(l.accZ);
  */

  //長方形積分
  r.velX = r.velX + r.comAccX * dt;
  r.velY = r.velY + r.comAccY * dt;
  r.velZ = r.velZ + r.comAccZ * dt;

  r.disX = r.disX + r.velX * dt;
  r.disY = r.disY + r.velY * dt;
  r.disZ = r.disZ + r.velZ * dt;

  r.velR = sqrt(r.velX * r.velX + r.velY * r.velY + r.velZ * r.velZ);
  r.disR = sqrt(r.disX * r.disX + r.disY * r.disY + r.disZ * r.disZ);

/*    //長方形積分
  l.velX = l.velX + l.comAccX * dt;
  l.velY = l.velY + l.comAccY * dt;
  l.velZ = l.velZ + l.comAccZ * dt;

  l.disX = l.disX + l.velX * dt;
  l.disY = l.disY + l.velY * dt;
  l.disZ = l.disZ + l.velZ * dt;

  l.velR = sqrt(l.velX * l.velX + l.velY * l.velY + l.velZ * l.velZ);
  l.disR = sqrt(l.disX * l.disX + l.disY * l.disY + l.disZ * l.disZ);
*/
  for (int i = 0; i < n - 1; i++) {
    r.diff[i] = r.diff[i + 1];
  }
  r.diff[n - 1] = r.velR;

  //グラフの山を判定
  if ((r.diff[0] < r.diff[2]) && (r.diff[4] > r.diff[6]))r.checker = 1000;//凸
  if ((r.diff[0] > r.diff[2]) && (r.diff[4] < r.diff[6]))r.checker = -1000;//凹

 /* 
  for (int i = 0; i < n - 1; i++) {
    l.diff[i] = l.diff[i + 1];
  }
  l.diff[n - 1] = l.velR;

  //グラフの山を判定
  if ((l.diff[0] < r.diff[2]) && (l.diff[4] > l.diff[6]))l.checker = 1000;//凸
  if ((l.diff[0] > r.diff[2]) && (l.diff[4] < l.diff[6]))l.checker = -1000;//凹
*/
  //判定
  
  if (( r.disX > r.d_x ) && ( r.checker == 1000 ) && (abs(r.disX) > abs(r.disY)) && (abs(r.disX) > abs(r.disZ))) {
    r.direction_x = 1200;
    delay(10);
  }
    if (( r.disX < -r.d_x ) && ( r.checker == -1000 ) && (abs(r.disX) > abs(r.disY)) && (abs(r.disX) > abs(r.disZ))) {
    r.direction_x = 1200;
    delay(10);
  }
  
  if (( r.disY > r.d_y ) && ( r.checker == 1000 )&& (abs(r.disY) > abs(r.disX)) && (abs(r.disY) > abs(r.disZ))) {
    r.direction_y = 1200;
    delay(10);
  }
  if (( r.disY < -r.d_y ) && ( r.checker == -1000 )&& (abs(r.disY) > abs(r.disX)) && (abs(r.disY) > abs(r.disZ))) {
    r.direction_y = -1200;
    delay(10);
  }
  
  if (( r.disZ > r.d_z ) && ( r.checker == 1000 )&& (abs(r.disZ) > abs(r.disY)) && (abs(r.disZ) > abs(r.disX))) {
    r.direction_z = 1200;
    delay(10);
  }
 if (( r.disZ < -r.d_z ) && ( r.checker == -1000 )&& (abs(r.disZ) > abs(r.disY)) && (abs(r.disZ) > abs(r.disX))) {
    r.direction_z = -1200;
    delay(10);
  }
  /*
  if (( l.disX > l.d_x ) && ( l.checker == 1000 ) && (abs(l.disX) > abs(l.disY)) && (abs(l.disX) > abs(l.disZ))) {
    l.direction_x = 1200;
    delay(10);
  }
  if (( l.disX < -l.d_x ) && ( l.checker == -1000 ) && (abs(l.disX) > abs(l.disY)) && (abs(l.disX) > abs(l.disZ))) {
    l.direction_x = 1200;
    delay(10);
  }
  
  if (( l.disY > l.d_y ) && ( l.checker == 1000 ) && (abs(l.disY) > abs(l.disX)) && (abs(l.disY) > abs(l.disZ))) {
    l.direction_y = 1200;
    delay(10);
  }
  if (( l.disY < -l.d_y ) && ( l.checker == -1000 ) && (abs(l.disY) > abs(l.disX)) && (abs(l.disY) > abs(l.disZ))) {
    l.direction_y = -1200;
    delay(10);
  }
  
  if (( l.disZ > l.d_z ) && ( l.checker == 1000 ) && (abs(l.disZ) > abs(l.disY)) && (abs(l.disZ) > abs(l.disX))) {
    l.direction_z = 1200;
    delay(10);
  }
  if (( l.disZ < -l.d_z ) && ( l.checker == -1000 ) && (abs(l.disZ) > abs(l.disY)) && (abs(l.disZ) > abs(l.disX))) {
    l.direction_z = -1200;
    delay(10);
  }
  */

/*
  Serial.print(r.direction_x); Serial.print("\t");
  Serial.print(r.direction_y); Serial.print("\t");
  Serial.print(r.direction_z); Serial.print("\t");
  Serial.print(l.direction_x); Serial.print("\t");
  Serial.print(l.direction_y); Serial.print("\t");
  Serial.print(l.direction_z); Serial.println("\t");
  */
  Serial.print(r.disX);Serial.print("\t");
  Serial.print(r.disY);Serial.print("\t");
  Serial.print(r.disZ);Serial.print("\t");
  Serial.print(l.disX);Serial.print("\t");
  Serial.print(l.disY);Serial.print("\t");
  Serial.print(l.disZ);Serial.println("\t");
  
  

  r.direction_x = 0;
  r.direction_y = 0;
  r.direction_z = 0;
  l.direction_x = 0;
  l.direction_y = 0;
  l.direction_z = 0;
  //数値初期化のためのカウント（一定時間で速度初期化）
  if (count <= 52)count++;
  if (count == 50) {
    r.velX = 0;
    r.velY = 0;
    r.velZ = 0;
    r.disX = 0;
    r.disY = 0;
    r.disZ = 0;
    l.velX = 0;
    l.velY = 0;
    l.velZ = 0;
    l.disX = 0;
    l.disY = 0;
    l.disZ = 0;
  }
  r.velX = r.velX * beta;
  r.velY = r.velY * beta;
  r.velZ = r.velZ * beta;
  r.disX = r.disX * beta;
  r.disY = r.disY * beta;
  r.disZ = r.disZ * beta;
  r.checker = 0;
  l.velX = l.velX * beta;
  l.velY = l.velY * beta;
  l.velZ = l.velZ * beta;
  l.disX = l.disX * beta;
  l.disY = l.disY * beta;
  l.disZ = l.disZ * beta;
  l.checker = 0;


  delay(2);


}

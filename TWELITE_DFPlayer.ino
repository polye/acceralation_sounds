//右足装着arduino想定
//TWELITEへのアナログ入力信号は0~2V
//TWELITE電源電圧は3.6Vまで

/*
   mp3_play ();    //start play
   mp3_play (5);  //play "mp3/0005.mp3"
   mp3_next ();   //play next 
   mp3_prev ();   //play previous
   mp3_set_volume (uint16_t volume);  //0~30
   mp3_set_EQ (); //0~5
   mp3_pause ();
   mp3_stop ();
   void mp3_get_state ();   //send get state command
   void mp3_get_volume (); 
   void mp3_get_u_sum (); 
   void mp3_get_tf_sum (); 
   void mp3_get_flash_sum (); 
   void mp3_get_tf_current (); 
   void mp3_get_u_current (); 
   void mp3_get_flash_current (); 
   void mp3_single_loop (boolean state);  //set single loop 
   void mp3_DAC (boolean state); 
   void mp3_random_play (); 
 */

#include<SoftwareSerial.h> //DFPlayer用
#include<DFPlayer_Mini_Mp3.h> //DFPlayer用

int l_x; //通信で得た左足のx軸方向の加速度
int l_y; //通信で得た左足のy軸方向の加速度
int l_z; //通信で得た左足のz軸方向の加速度

int r_x; //右足のx軸方向の加速度
int r_y; //右足のy軸方向の加速度
int r_z; //右足のz軸方向の加速度

//duty比計算用関数
double duration_H;
double duration_L;
double str_send_data;
double duty = 0;

//右脚装着加速度ピン設定
int r_x_pin = 0;
int r_y_pin = 1;
int r_z_pin = 2;

//左脚装着加速度ピン設定
int l_x_pin = 3;
int l_y_pin = 4;
int l_z_pin = 5;

void setup() {
   //TWELITEは基本115200or38400だけどこの方法なら大丈夫なはず
  Serial.begin(9600);
  
  //DFPlayer_mini mp3用のシリアル通信設定
  mp3_set_serial(Serial);
  
  //音量設定
  mp3_set_volume(15);
  
  //左足加速度（TWELITEからの受信）のためピン設定
  pinMode(l_x_pin, INPUT);
  pinMode(l_y_pin, INPUT);
  pinMode(l_z_pin, INPUT);
}

void loop() {
  //TWELITEへの入力信号が0~2Vのため、読み込み信号を外部にして、MAX電圧を制御
  analogReference(EXTERNAL);
  
  //読み込んだ信号のパルス幅を計算しduty比を計算(x)<-もしpulseInがタイムアウトしてしまったら少し時間かかる
  duration_H = pulseIn(l_x_pin, HIGH);
  duration_L = pulseIn(l_x_pin, LOW);
  duty = duration_H / (duration_H + duration_L);
  str_send_data = 1024*duty;
  l_x = (int)str_send_data;
  
  //(y)
  duration_H = pulseIn(l_y_pin, HIGH);
  duration_L = pulseIn(l_y_pin, LOW);
  duty = duration_H / (duration_H + duration_L);
  str_send_data = 1024*duty;
  l_y = (int)str_send_data;

  //(z)
  duration_H = pulseIn(l_z_pin, HIGH);
  duration_L = pulseIn(l_z_pin, LOW);
  duty = duration_H / (duration_H + duration_L);
  str_send_data = 1024*duty;
  l_z = (int)str_send_data;

  
  
  
}

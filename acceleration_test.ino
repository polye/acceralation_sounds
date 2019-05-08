//********************************
//加速度センサの値を取得するプログラム
//********************************
void setup()
{
  // シリアルモニターの初期化をする
  Serial.begin(9600) ;
  Serial.println("start");
}
void loop()
{
  long x , y , z ;
  x = y = z = 0 ;
 /* 
  int i;
  for (i=0;i<50;i++){
    x = x + analogRead(3);
    y = y + analogRead(4);
    z = z + analogRead(5);
  }
  x = x / 50 ;
  y = y / 50 ;
  z = z / 50 ;
  */
  x = analogRead(3);
  y = analogRead(4);
  z = analogRead(5);
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
 /*
  Serial.print("X:") ;
  Serial.print(x) ;
  Serial.print(" Y:") ;
  Serial.print(y) ;
  Serial.print(" Z:") ;
  Serial.print(z) ;
  */
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  Serial.println("");
  delay(10) ;
}

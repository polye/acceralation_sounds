#define BEATTIME 300
#define PIN 12

#define DO 262
#define RE 294
#define MI 330
#define FA 349
#define SO 392
#define RA 440
#define SI 494
#define HDO 523
void setup() {
}
void loop() {
  tone(PIN, DO, BEATTIME) ; // ド
  delay(BEATTIME) ;
  tone(PIN, RE, BEATTIME) ; // レ
  delay(BEATTIME) ;
  tone(PIN, MI, BEATTIME) ; // ミ
  delay(BEATTIME) ;
  tone(PIN, FA, BEATTIME) ; // ファ
  delay(BEATTIME) ;
  tone(PIN, SO, BEATTIME) ; // ソ
  delay(BEATTIME) ;
  tone(PIN, RA, BEATTIME) ; // ラ
  delay(BEATTIME) ;
  tone(PIN, SI, BEATTIME) ; // シ
  delay(BEATTIME) ;
  tone(PIN, HDO, BEATTIME) ; // ド
  delay(BEATTIME) ;
}

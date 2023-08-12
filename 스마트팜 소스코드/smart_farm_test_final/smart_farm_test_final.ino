

/* 스마트 화분 관리기 */
/* 4~5시간? */
/* 들어가는 센서 종류 : 온습도 센서, 조도 센서, 토양 수분 센서, 릴레이 모듈, LCD 모듈, 생장 LED */
/* 필요한 기능
 *  1. 조도 센서를 이용해서 자연광을 측정하고, 자연광의 강도가 낮아지면 생장 LED를 켜서 화분의 식물을 관리한다.
 *  2. 온습도 센서를 이용해서 화분 주변 환경의 온습도를 측정한다.
 *  3. 토양 수분 센서를 이용해 토양의 수분을 측정한다.
 *  4. 릴레이 모듈을 생장 LED와 연결하여 조도 센서값에 따라 생장 LED가 켜질 수 있도록 한다.
 *  5. LCD 모듈을 통해서 온습도 센서와 토양 수분 센서 측정 값을 출력한다.
 */
#include <DHT.h>                    // 온습도 센서 모듈 라이브러리
#include <LiquidCrystal.h>          // LCD 모듈 라이브러리

// 센서 핀 설정
#define relayPin 3                  // 릴레이 모듈 핀 -> 생장 LED 켜기 위함
#define cdsPin A1                   // 조도센서 모듈 핀
#define DHTPIN 4                    // 온습도센서 모듈 핀
#define DHTTYPE DHT11               // 온습도 센서타입 설정
#define soilmoisturePin A0          // 토양수분센서 핀
#define buzzer 8

// 객체 생성
DHT dht(DHTPIN, DHTTYPE);                   //온습도 센서 모듈
LiquidCrystal lcd(12, 11, 10, 7, 9, 2);   //LCD 초기화 - 핀 번호 (RS, E, D4, D5, D6, D7)
// 문자열을 출력하기 위한 변수
char str_M[10];
char str_T[10];
char str_H[10];

//특수 문자 및 이모티콘 (사용자 정의 문자)
byte temp[8] = {B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110};
byte C[8] = {B10000, B00110, B01001, B01000, B01000, B01001, B00110, B00000};
byte humi[8] = {B00100, B00100, B01010, B01010, B10001, B11011, B11111, B01110};
byte Qmark[8] = {B00100, B01110, B01110, B01110, B01110, B00100, B00000, B00100};
byte water[8] = {B00100, B00100, B01110, B01110, B11111, B11111, B11111, B01110};
byte good[8] = {B00000, B01010, B01010, B00000, B00000, B01010, B01010, B01110};
byte wind[8] = {B00000, B10011, B11010, B00100, B01011, B11001, B00000, B00000};

void setup() {
  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);
  pinMode(cdsPin, INPUT);
  pinMode(soilmoisturePin, INPUT);
  pinMode(buzzer, OUTPUT);
  
  //LCD에 인트로 출력

  lcd.begin(16, 2);
  lcd.setCursor(2,0);
  lcd.print("SMART");
  delay(1000);
  lcd.setCursor(8,0);
  lcd.print("GARDEN");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("BP LAB CodingEdu");
  delay(1000);
  lcd.clear();


  // 라이브러리로 추가한 특수 문자 및 이모티콘 추가

  lcd.createChar(1, temp);
  lcd.createChar(2, C);
  lcd.createChar(3, humi);  
  lcd.createChar(4, Qmark);
  lcd.createChar(5, water);
  lcd.createChar(6, good);
  lcd.createChar(7, wind);
  
}

void loop() {

  //센서값 측정
  int cdsValue = analogRead(cdsPin);                                 // 조도센서 값 측정: 0(밝음) ~ 1023(어두움)
  int soilmoistureValue = analogRead(soilmoisturePin);               // 토양수분 값 측정: 0(습함) ~ 1023(건조)
  int soilmoisture_per = map(soilmoistureValue, 170, 1023, 100, 0);  // 센서 값을 퍼센트로 변경
  unsigned char h_Value = dht.readHumidity();                        // 공기 중 습도 값 측정
  unsigned char t_Value = dht.readTemperature();      // 공기 중 온도 값 측정

  //LCD에 토양 습도값 출력
  lcd.setCursor(1,0);
  lcd.print("MOIST:");
  sprintf(str_M, "%03d", soilmoisture_per);
  lcd.print(str_M);
  lcd.setCursor(10,0);
  lcd.print("%");

  //LCD에 온도값 출력
  lcd.setCursor(1,1);
  lcd.write(1);
  sprintf(str_T, "%02d", t_Value);
  lcd.setCursor(3,1);
  lcd.print(str_T);
  lcd.write(2);

  //LCD에 습도값 출력
  lcd.setCursor(7,1);
  lcd.write(3);
  sprintf(str_H, "%02d", h_Value);
  lcd.setCursor(9,1);
  lcd.print(str_H);
  lcd.print("%");

  //조도센서 값에 따른 생장 LED 켜기
  if(cdsValue < 500){
    digitalWrite(relayPin, LOW);
  }
  else if(cdsValue >= 500){
    digitalWrite(relayPin, HIGH);
  }

  //토양습도 값에 따른 LCD에 이모티콘 띄우기
  if(soilmoisture_per >= 0 && soilmoisture_per < 30){
    lcd.setCursor(13,0);
    lcd.write(4);
    lcd.setCursor(14,0);
    lcd.write(5);
    tone(buzzer, 230, 1000);
    delay(1000);
    noTone(buzzer);
  }
  else if(soilmoisture_per >= 30 && soilmoisture_per < 70){
    lcd.setCursor(13,0);
    lcd.print(" ");
    lcd.setCursor(14,0);
    lcd.write(6);
  }
  else if(soilmoisture_per >= 70){
    lcd.setCursor(13,0);
    lcd.write(4);
    lcd.setCursor(14,0);
    lcd.write(7);
    tone(buzzer, 500, 1000);
    delay(1000);
    noTone(buzzer);
  }
  Serial.print(cdsValue);
  Serial.print(" ");
  Serial.println(soilmoisture_per);
  
  delay(500);
}

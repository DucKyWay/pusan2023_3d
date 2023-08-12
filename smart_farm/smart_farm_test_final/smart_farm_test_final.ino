#include <DHT.h>                 
#include <LiquidCrystal.h>     

#define relayPin 3         
#define cdsPin A1           
#define DHTPIN 4        
#define DHTTYPE DHT11               
#define soilmoisturePin A0 
#define buzzer 8

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 10, 7, 9, 2);
// 문자열을 출력하기 위한 변수
char str_M[10];
char str_T[10];
char str_H[10];

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

  lcd.createChar(1, temp);
  lcd.createChar(2, C);
  lcd.createChar(3, humi);  
  lcd.createChar(4, Qmark);
  lcd.createChar(5, water);
  lcd.createChar(6, good);
  lcd.createChar(7, wind);
}

void loop() {

  int cdsValue = analogRead(cdsPin);                              
  int soilmoistureValue = analogRead(soilmoisturePin);             
  int soilmoisture_per = map(soilmoistureValue, 170, 1023, 100, 0);
  unsigned char h_Value = dht.readHumidity();                 
  unsigned char t_Value = dht.readTemperature();  

  lcd.setCursor(1,0);
  lcd.print("MOIST:");
  sprintf(str_M, "%03d", soilmoisture_per);
  lcd.print(str_M);
  lcd.setCursor(10,0);
  lcd.print("%");

  lcd.setCursor(1,1);
  lcd.write(1);
  sprintf(str_T, "%02d", t_Value);
  lcd.setCursor(3,1);
  lcd.print(str_T);
  lcd.write(2);

  lcd.setCursor(7,1);
  lcd.write(3);
  sprintf(str_H, "%02d", h_Value);
  lcd.setCursor(9,1);
  lcd.print(str_H);
  lcd.print("%");

  if(cdsValue < 500){
    digitalWrite(relayPin, LOW);
  }
  else if(cdsValue >= 500){
    digitalWrite(relayPin, HIGH);
  }

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

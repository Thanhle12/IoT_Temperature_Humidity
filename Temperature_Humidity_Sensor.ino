#define BLYNK_TEMPLATE_ID "TMPL6KvVNLsgI"
#define BLYNK_TEMPLATE_NAME "IoT Giám Sát Nhiệt Độ Độ Ẩm"
char auth[] = "wNpmMsU0bBivdDYHM_oTDpAuYJ2cOQST"; //Mã Token của Project trên Blynk

#define BLYNK_PRINT SwSerial
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX
#include <BlynkSimpleStream.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#define DHTPIN 2 // What digital pin we're connected to
LiquidCrystal lcd(11, 10, 6, 5, 4, 3);
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Khai báo mảng byte để tạo ký tự "°"
byte degreeSymbol[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void sendSensor()
{
  int t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  int h = dht.readHumidity();
  
  if (isnan(h) || isnan(t)) {
    SwSerial.println("Failed to read from DHT sensor!");
    return;
  }

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  //------------------------
  // Hiển thị trên LCD
  lcd.setCursor(0, 0);
  lcd.print("Nhiet do: ");
  lcd.print(t);
  lcd.write((uint8_t)0); // In ký tự "°"
  lcd.print("C");

  //------------------------
  lcd.setCursor(0, 1);
  lcd.print("Do am: ");
  lcd.print(h);
  lcd.print("%");
}

void setup()
{
  // Debug console
  SwSerial.begin(9600);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  lcd.begin(16, 2);
  dht.begin();
  // Tạo ký tự "°" tại vị trí 0
  lcd.createChar(0, degreeSymbol);
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
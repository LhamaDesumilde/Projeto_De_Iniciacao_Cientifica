//-----------------------------------------------------------------------------------------------------------
//BIBLIOTECAS
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <RTClib.h>

//-----------------------------------------------------------------------------------------------------------
//PORTAS

#define DHTPIN 2
#define DHTTYPE DHT22

#define LDRPIN A0

const int chipSelect = 4; //CS do modulo SD

//-----------------------------------------------------------------------------------------------------------
//VARIAVEIS

// Variaveis strings e arquivos
char filename[15];
char results[78];
File dataFile;

// Variaveis sensores
DHT dht(DHTPIN, DHTTYPE); // Sensor DHT22
int ldrValue = 0; // Valor do sensor LDR

// Variáveis para controle de tempo
DateTime now;
int lastDay;
RTC_DS3231 rtc;

//-----------------------------------------------------------------------------------------------------------
void setup() {
  // *******************
  //Inicialização
  Serial.begin(9600);
  Serial.println(F("DHT22, LDR, RTC DS3231 and SD test"));
  Wire.begin();

  // *******************
  dht.begin(); // DHT22

  // *******************
  if (!rtc.begin()) { // RTC
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, let's set the time!"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // *******************
  Serial.print(F("Initializing SD card..."));// Inicialização do módulo SD
  if (!SD.begin(chipSelect)) {
    Serial.println(F("initialization failed!"));
    return;
  }
  Serial.println(F("initialization done."));
  
  // *******************
  

  now = rtc.now();
  lastDay= now.day();
  sprintf(filename, "a%04d_%02d_%02d.txt", now.year(), now.month(), now.day());
  dataFile = SD.open(filename, FILE_WRITE);

}
//-----------------------------------------------------------------------------------------------------------

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  ldrValue = analogRead(LDRPIN);
  now = rtc.now();
  sprintf(results, "%02d:%02d:%02d - Umidade: %f%%, Temperatura: %f *C, Intensidade Luminosa: %d", now.hour(), now.minute(), now.second(), h, t, ldrValue);
  if(now.day() != lastDay){
    lastDay = now.day();
    dataFile.close();
    sprintf(filename, "a%04d_%02d_%02d.txt", now.year(), now.month(), now.day());
    dataFile = SD.open(filename, FILE_WRITE);
    dataFile.println(results);
  }else{
    dataFile.println(results);
  }
  delay(1000);
}
//-----------------------------------------------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DHT.h>
#include <RTClib.h>

// Definições para o RTC DS3231
RTC_DS3231 rtc;

// Definindo pinos para o sensor DHT22
#define DHTPIN 2
#define DHTTYPE DHT22

// Definindo pino para o sensor LDR
#define LDRPIN A0

// Definindo pino CS para o módulo SD
const int chipSelect = 10;

DHT dht(DHTPIN, DHTTYPE); // Sensor DHT22
int ldrValue = 0; // Valor do sensor LDR

File dataFile;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT22, LDR, RTC DS3231 and SD test"));

  // Inicialização dos sensores
  dht.begin();
  Wire.begin();
  
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, let's set the time!"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Inicialização do módulo SD
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(chipSelect)) {
    Serial.println(F("initialization failed!"));
    while (1);
  }
  Serial.println(F("initialization done."));
}

void loop() {
  DateTime now = rtc.now();

  // Salva o horário atual no arquivo SD
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(' ');
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.println();
  } else {
    Serial.println(F("error opening data.txt"));
    return;
  }

  // Leitura do sensor DHT22----------------------------------
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    Serial.print(F("DHT - Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println(" *C");

    // Escrever no arquivo SD
    if (dataFile) {
      dataFile.print(F("DHT - Humidity: "));
      dataFile.print(h);
      dataFile.print(F("%  Temperature: "));
      dataFile.print(t);
      dataFile.println(" *C");
    } else {
      Serial.println(F("error writing DHT data"));
    }
  }
  //-----------------------------------------------------------

  // Leitura do sensor LDR------------------------------------
  ldrValue = analogRead(LDRPIN);
  Serial.print(F("LDR - Light intensity: "));
  Serial.println(ldrValue);

  // Escrever no arquivo SD
  if (dataFile) {
    dataFile.print(F("LDR - Light intensity: "));
    dataFile.println(ldrValue);
    dataFile.close();
  } else {
    Serial.println(F("error writing LDR data"));
  }
  //-----------------------------------------------------------

  Serial.println();
  delay(2000);
}

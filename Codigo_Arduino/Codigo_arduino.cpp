/*
DHT22:
Vcc: 5V
DATA: D2

BMP280:
Vcc: 3.3V
SCK: D8
SDI: D9
SDO: D7
CS: D6

LDR: A0
PULLDOWN 10KOhm COM A0
Vcc: 5V

SD:
MISO: D12
MOSI: D11
SCK: D13
CS: D10
Vcc: 5V

RTC DS3231:
SDA: A4
SCL: A5
Vcc: 5V
GND: GND
*/

#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <RTClib.h>

#define DHTPIN 2        // Pino digital conectado ao sensor DHT22
#define DHTTYPE DHT22   // Tipo do sensor DHT

#define LDRPIN A0       // Pino analógico conectado ao sensor LDR

// Definição dos pinos para o BMP280 utilizando SPI
#define BMP_SCK 8
#define BMP_MISO 7
#define BMP_MOSI 9
#define BMP_CS 6

const int chipSelect = 10; // Pino CS do módulo leitor de cartão SD

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp(BMP_CS);
RTC_DS3231 rtc;

File myFile;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx, LDR, BMP280, and RTC DS3231 test!"));

  dht.begin();

  // Inicialização do BMP280 usando SPI
  if (!bmp.begin(BMP_CS)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  // Inicialização do RTC DS3231
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }

  // Verifica se o relógio RTC está funcionando, caso contrário, define a data e hora
  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, setting the time!"));
    // Ajuste a data e hora para a compilação atual
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.print(F("Initializing SD card..."));

  if (!SD.begin(chipSelect)) {
    Serial.println(F("initialization failed!"));
    return;
  }

  Serial.println(F("initialization done."));

  myFile = SD.open("data.txt", FILE_WRITE);

  if (myFile) {
    Serial.println(F("Writing to data.txt..."));
    myFile.close();
    Serial.println(F("done."));
  } else {
    Serial.println(F("error opening data.txt"));
  }
}

void loop() {
  delay(5000);

  DateTime now = rtc.now();

  // Leitura do sensor DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Leitura do sensor LDR
  int ldrValue = analogRead(LDRPIN);

  // Leitura do sensor BMP280
  float pressure = bmp.readPressure() / 100.0; // Convertendo de Pa para hPa

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  myFile = SD.open("data.txt", FILE_WRITE);

  if (myFile) {
    myFile.print(now.year());
    myFile.print(':');
    myFile.print(now.month());
    myFile.print(':');
    myFile.print(now.day());
    myFile.print(' ');
    myFile.print(now.hour());
    myFile.print(':');
    myFile.print(now.minute());
    myFile.print(':');
    myFile.print(now.second());
    myFile.print(' ');

    myFile.print(F("DHT - Humidity: "));
    myFile.print(h);
    myFile.print(F("%  Temperature: "));
    myFile.println(t);

    myFile.print(F("LDR - Light intensity: "));
    myFile.println(ldrValue);

    myFile.print(F("BMP280 - Pressure: "));
    myFile.print(pressure);
    myFile.println(F(" hPa"));

    myFile.close();
  } else {
    Serial.println(F("error opening data.txt"));
  }

  // Abrir o arquivo "data.txt" para leitura
  myFile = SD.open("data.txt");
  if (myFile) {
    Serial.println(F("Data read from data.txt:"));
    // Ler o conteúdo do arquivo e imprimir na porta serial
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println(F("error opening data.txt"));
  }
}

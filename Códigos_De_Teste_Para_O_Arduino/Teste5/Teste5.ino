/*
Conexões para BMP280 com Arduino Nano utilizando SPI:

BMP280:
VCC: Conectar ao 3.3V do Arduino Nano
GND: Conectar ao GND do Arduino Nano
SCL: Conectar ao pino D13 (SCK) do Arduino Nano
SDA: Conectar ao pino D11 (MOSI) do Arduino Nano
CSB: Conectar ao pino D10 para Chip Select (CS)
SDO: Não utilizado no modo SPI

DHT22:
VCC: Conectar ao 5V do Arduino Nano
DATA: Conectar ao pino D2 do Arduino Nano
GND: Conectar ao GND do Arduino Nano

LDR:
Conectar um lado ao A0 do Arduino Nano
Conectar o outro lado a um resistor de pull-down de 10kΩ para o GND
VCC: Conectar ao 5V do Arduino Nano

RTC DS3231:
SDA: Conectar ao pino A4 do Arduino Nano (ou SDA do Arduino Nano)
SCL: Conectar ao pino A5 do Arduino Nano (ou SCL do Arduino Nano)
VCC: Conectar ao 5V do Arduino Nano
GND: Conectar ao GND do Arduino Nano

SD:
MISO: D12
MOSI: D11
SCK: D13
SC:D10
Vcc: 5V
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <RTClib.h>

// Definições para o RTC DS3231
RTC_DS3231 rtc;

// Definindo os pinos para SPI do BMP280
#define BMP_CS 10
#define BMP_SCK 13
#define BMP_MOSI 11

// Definindo pinos para o sensor DHT22
#define DHTPIN 2
#define DHTTYPE DHT22

// Definindo pino para o sensor LDR
#define LDRPIN A0

// Definindo pino CS para o módulo SD
const int chipSelect = 4;

Adafruit_BMP280 bmp; // Usando SPI com pinos específicos
DHT dht(DHTPIN, DHTTYPE); // Sensor DHT22
int ldrValue = 0; // Valor do sensor LDR

File dataFile;

// Variáveis para controle de tempo
DateTime now;
DateTime lastDay;

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280, DHT22, LDR, RTC DS3231 and SD test"));

  // Inicialização dos pinos SPI
  pinMode(BMP_CS, OUTPUT); // Definindo CSB como saída
  digitalWrite(BMP_CS, HIGH); // Inicia desativado

  // Inicialização dos sensores
  if (!bmp.begin(BMP_CS)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  dht.begin();

  // Inicialização do RTC DS3231
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
    return;
  }
  Serial.println(F("initialization done."));
}

void loop() {
  now = rtc.now();

  // Verifica se mudou o dia
  if (now.day() != lastDay.day()) {
    // Fecha o arquivo do dia anterior, se estiver aberto
    if (dataFile) {
      dataFile.close();
    }
    
    // Gera o nome do arquivo no formato aAAAA_MM_DD
    char filename[15];
    sprintf(filename, "a%04d_%02d_%02d.txt", now.year(), now.month(), now.day());

    // Abre o novo arquivo para escrita
    dataFile = SD.open(filename, FILE_WRITE);
    if (!dataFile) {
      Serial.print(F("Error opening "));
      Serial.println(filename);
    }

    // Atualiza o último dia
    lastDay = now;
  }

  // Salva o horário atual no arquivo SD
  if (dataFile) {
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.print(" - ");
  } else {
    Serial.println(F("error opening data.txt"));
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
      Serial.println(F("error opening data.txt"));
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
  } else {
    Serial.println(F("error opening data.txt"));
  }
  //-----------------------------------------------------------

  // Leitura do sensor BMP280----------------------------------
  Serial.print(F("BMP280 - "));
  Serial.print(F("Temperature: "));
  Serial.print(bmp.readTemperature());
  Serial.print(" *C, ");
  Serial.print(F("Pressure: "));
  Serial.print(bmp.readPressure() / 100.0); // Convertendo de Pa para hPa
  Serial.println(" hPa");

  // Escrever no arquivo SD
  if (dataFile) {
    dataFile.print(F("BMP280 - "));
    dataFile.print(F("Temperature: "));
    dataFile.print(bmp.readTemperature());
    dataFile.print(" *C, ");
    dataFile.print(F("Pressure: "));
    dataFile.print(bmp.readPressure() / 100.0); // Convertendo de Pa para hPa
    dataFile.println(" hPa");
  } else {
    Serial.println(F("error opening data.txt"));
  }
  //-----------------------------------------------------------

  Serial.println();
  delay(2000);
}

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <RTClib.h>

// Definindo pinos para o sensor DHT22
#define DHTPIN 2
#define DHTTYPE DHT22

// Definindo pino para o sensor LDR
#define LDRPIN A0

// Definindo pino CS para o módulo SD
const int chipSelect = 4;

DHT dht(DHTPIN, DHTTYPE); // Sensor DHT22
int ldrValue = 0; // Valor do sensor LDR

File dataFile;
RTC_DS3231 rtc;

// Protótipos das funções
bool openLogFile(const char* filename = "data.txt");
void closeLogFile();

// Variáveis para controle de tempo
DateTime now;
DateTime lastDay;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT22, LDR, RTC DS3231 and SD test"));

  // Inicialização dos sensores
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

  // Abrir o arquivo de log para escrita no início
  openLogFile();
}

void loop() {
  now = rtc.now();

  // Verificar se mudou o dia
  if (now.day() != lastDay.day()) {
    // Fechar o arquivo do dia anterior, se estiver aberto
    closeLogFile();

    // Gerar o nome do arquivo no formato aAAAA_MM_DD
    char filename[15];
    sprintf(filename, "a%04d_%02d_%02d.txt", now.year(), now.month(), now.day());

    // Abrir o novo arquivo para escrita
    if (!openLogFile(filename)) {
      Serial.print(F("Error opening "));
      Serial.println(filename);
    }

    // Atualizar o último dia
    lastDay = now;
  }

  // Salvar o horário atual no arquivo SD
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

  Serial.println();
  delay(2000);
}

bool openLogFile(const char* filename) {
  dataFile = SD.open(filename, FILE_WRITE);
  return dataFile;
}

void closeLogFile() {
  if (dataFile) {
    dataFile.close();
  }
}

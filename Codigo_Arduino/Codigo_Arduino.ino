//------------------------------Bibliotecas------------------------------
//Bibliotecas para o leitor de cartão
#include <SPI.h>
#include <SD.h>
//Bibliotecas para o DHT22
//#include <OneWire.h>
#include <DallasTemperature.h>
//Bibliotecas para o BMP280
#include <Wire.h>
#include <Adafruit_BMP280.h>
//------------------------------------------------------------

//-----------------LDR
#define LDRPIN A0 // Pino analógico conectado ao sensor LDR
//-----------------

//-----------------BMP280 (I2C)
Adafruit_BMP280 bmp;
//-----------------

//-----------------DS18B20
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//-----------------

//-----------------Leitor de cartão SD
const int chipSelect = 10;
File myFile;
//-----------------

void setup() {
  Serial.begin(9600);

  //-----------------Inicializa o BMP280
  if (!bmp.begin(0x76)) {  // Endereço padrão I2C para BMP280 é 0x76 (pode ser 0x77 em alguns casos)
    Serial.println("Erro ao inicializar o BMP280!");
    while (1); // Trava o código se não encontrar o sensor
  }
  Serial.println("BMP280 iniciado com sucesso!");
  //-----------------

  //-----------------Inicializa o DS18B20
  sensors.begin();
  //-----------------

  //-----------------Inicializa o leitor de cartão SD
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
  //-----------------

}

void loop() {
  sensors.requestTemperatures(); //Pede Temperatura ao DS18B20
  float temperatureC = sensors.getTempCByIndex(0); // Índice 0, caso tenha apenas 1 sensor
  float pressure = bmp.readPressure();  // Leitura da pressão em Pascals
  int ldrValue = analogRead(LDRPIN); //Leitura do ldr

  myFile = SD.open("data.txt", FILE_WRITE);
  if (myFile){
    myFile.print(F("%  Temperature: "));
    myFile.println(temperatureC);

    myFile.print(F("LDR - Light intensity: "));
    myFile.println(ldrValue);

    myFile.print(F("BMP280 - Pressure: "));
    myFile.print(pressure);
    myFile.println(F(" hPa"));

    myFile.close();
  } else {
    Serial.println(F("error opening data.txt"));
  }

}

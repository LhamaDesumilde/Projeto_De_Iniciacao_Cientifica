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
*/

#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>

// Definindo os pinos para SPI do BMP280
#define BMP_CS 10
#define BMP_SCK 13
#define BMP_MOSI 11

// Definindo pinos para o sensor DHT22
#define DHTPIN 2
#define DHTTYPE DHT22

// Definindo pino para o sensor LDR
#define LDRPIN A0

Adafruit_BMP280 bmp; // Usando SPI com pinos específicos
DHT dht(DHTPIN, DHTTYPE); // Sensor DHT22
int ldrValue = 0; // Valor do sensor LDR

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280, DHT22 and LDR test"));

  // Inicialização dos pinos SPI
  pinMode(BMP_CS, OUTPUT); // Definindo CSB como saída
  digitalWrite(BMP_CS, HIGH); // Inicia desativado

  // Inicialização dos sensores
  if (!bmp.begin(BMP_CS)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  dht.begin();
}

void loop() {
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
  }
  //-----------------------------------------------------------

  // Leitura do sensor LDR------------------------------------
  ldrValue = analogRead(LDRPIN);
  Serial.print(F("LDR - Light intensity: "));
  Serial.println(ldrValue);
  //-----------------------------------------------------------

  // Leitura do sensor BMP280----------------------------------
  Serial.print(F("BMP280 - "));
  Serial.print(F("Temperature: "));
  Serial.print(bmp.readTemperature());
  Serial.print(" *C, ");
  Serial.print(F("Pressure: "));
  Serial.print(bmp.readPressure() / 100.0); // Convertendo de Pa para hPa
  Serial.println(" hPa");
  //-----------------------------------------------------------

  Serial.println();
  delay(2000);
}

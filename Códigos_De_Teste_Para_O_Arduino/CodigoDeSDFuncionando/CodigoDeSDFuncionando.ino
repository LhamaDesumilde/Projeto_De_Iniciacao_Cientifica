#include <SPI.h>
#include <SD.h>

// Definindo o pino CS para o módulo SD
const int chipSelect = 10;

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);
  Serial.println("Iniciando o teste do módulo SD...");

  // Inicializa o módulo SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Falha na inicialização do cartão SD!");
    while (1); // Para o programa se a inicialização falhar
  }
  Serial.println("Cartão SD inicializado com sucesso.");

  // Cria ou abre um arquivo para escrever
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Teste de escrita no cartão SD.");
    dataFile.close();
    Serial.println("Dados escritos no arquivo test.txt.");
  } else {
    Serial.println("Erro ao abrir o arquivo test.txt para escrita.");
  }

  // Lê o arquivo para verificar a escrita
  dataFile = SD.open("test.txt");
  if (dataFile) {
    Serial.println("Conteúdo do arquivo test.txt:");
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  } else {
    Serial.println("Erro ao abrir o arquivo test.txt para leitura.");
  }
}

void loop() {
  // Não faz nada no loop
}

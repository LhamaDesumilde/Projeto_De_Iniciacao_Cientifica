#include <SPI.h>
#include <SD.h>

// Definindo as portas
const int chipSelect = 10;

void setup() {
  // Inicia a comunicação serial para a depuração
  Serial.begin(9600);
  while (!Serial) {
    ; // Aguarde até que a porta serial esteja conectada (somente para Leonardo)
  }

  Serial.print("Inicializando o cartão SD...");

  // Verifica se o cartão SD está acessível
  if (!SD.begin(chipSelect)) {
    Serial.println("Falha ao acessar o cartão SD.");
    return;
  }
  Serial.println("Cartão SD inicializado com sucesso.");

  // Tenta abrir um arquivo para teste
  File testFile = SD.open("teste.txt", FILE_WRITE);
  
  // Verifica se o arquivo foi aberto com sucesso
  if (testFile) {
    Serial.println("Escrevendo no arquivo...");
    testFile.println("Teste de escrita no cartão SD.");
    testFile.close();
    Serial.println("Escrita concluída.");
  } else {
    Serial.println("Erro ao abrir o arquivo para escrita.");
  }

  // Tenta abrir o arquivo para leitura
  testFile = SD.open("teste.txt");
  if (testFile) {
    Serial.println("Lendo do arquivo:");

    // Lê até o fim do arquivo
    while (testFile.available()) {
      Serial.write(testFile.read());
    }
    testFile.close();
  } else {
    Serial.println("Erro ao abrir o arquivo para leitura.");
  }
}

void loop() {
  // Não precisa repetir nada no loop
}

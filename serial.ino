#include <digitalWriteFast.h>

// Define constantes para os pinos
const int IGNICAO_FEEDBACK_PIN = 8;
const int SAIDA_IGNICAO_PIN = 13;

void setup() {
  Serial.begin(9600);   // Inicia a comunicação serial
  pinMode(IGNICAO_FEEDBACK_PIN, INPUT);    // Define o pino de feedback de ignição como entrada
  pinMode(SAIDA_IGNICAO_PIN, OUTPUT);  // Define o pino de saída de ignição como saída
}

void loop() {
  int fdb = digitalRead(IGNICAO_FEEDBACK_PIN);
  if (Serial.available() > 0) {                      // Verifica se há dados disponíveis na porta serial
    String mensagem = Serial.readStringUntil('\n');  // Lê a mensagem até o caractere de nova linha

    if (mensagem == "liga") {
      if (fdb == 0) {     // Verifica se o pino de feedback de ignição está em nível baixo
        digitalWriteFast(SAIDA_IGNICAO_PIN, HIGH);  // Define o pino de saída de ignição como nível alto
        fdb = digitalRead(IGNICAO_FEEDBACK_PIN);
        if (fdb == 1) {
          Serial.println("Ligado com sucesso!");  // Envia a mensagem "Ligado com sucesso" pela porta serial
        } else {
          Serial.println("Não foi posivel ligar!");  // Envia a mensagem "Não foi posivel ligar" pela porta serial
        }
      } else if (fdb == 1) {
        Serial.println("Já está ligado!");  // Envia a mensagem "Já está ligado" pela porta serial
      }

    } else if (mensagem == "desliga") {
      if (fdb == 1) {                   // Verifica se o pino de feedback de ignição está em nível alto
        digitalWriteFast(SAIDA_IGNICAO_PIN, LOW);                 // Define o pino de saída de ignição como nível baixo
        if(digitalRead(IGNICAO_FEEDBACK_PIN) == 0){
          Serial.println("Desligado com sucesso!");  // Envia a mensagem "Desligado com sucesso" pela porta serial
        }else{
          Serial.println("Não foi posivel Desligar!");
        }
      } else if (fdb == 0) {
        Serial.println("Já está desligado!");  // Envia a mensagem "Já está desligado" pela porta serial
      }
    }
  }
}

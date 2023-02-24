#include <digitalWriteFast.h>

// Define constantes para os pinos
const int IGNICAO_FEEDBACK_PIN = 8;
const int SAIDA_IGNICAO_PIN = 13;
const int MOTOR_DE_ARRANQUE = 9;
const int PORTA_DIREITA = 6; 
const int PORTA_ESQUERDA = 5;
const int PORTA_MALA = 4;

// Variáveis para monitoramento do tempo das portas
unsigned long agora_pe;
unsigned long agora_pd;
unsigned long agora_pm;
unsigned long antes_pd = 0;
unsigned long antes_pm = 0;
int intervalo_pd = 1000;
unsigned long antes_pe = 0;
int intervalo_pe = 1000;
int intervalo_pm = 2000;

// Função para dar partida no motor de arranque
void dar_partida(int tempo);

// Funções para monitorar as portas
int monitora_porta_direita();
int monitora_porta_esquerda();
int monitora_porta_mala();


void dar_partida(int);
int monitora_porta_direita();
int monitora_porta_esquerda();
void setup() {
  Serial.begin(9600);   // Inicia a comunicação serial
  pinMode(IGNICAO_FEEDBACK_PIN, INPUT);    // Define o pino de feedback de ignição como entrada
  pinMode(SAIDA_IGNICAO_PIN, OUTPUT);  // Define o pino de saída de ignição como saída
  pinMode(MOTOR_DE_ARRANQUE, OUTPUT);
  pinMode(PORTA_DIREITA, INPUT);
  pinMode(PORTA_ESQUERDA, INPUT);
}

void loop() {
	monitora_porta_esquerda();
	monitora_porta_direita();
	monitora_porta_mala();
  int fdb = digitalRead(IGNICAO_FEEDBACK_PIN);
  
  // Verifica se há dados disponíveis na porta serial
  if (Serial.available() > 0) {                      
    // Lê a mensagem até o caractere de nova linha
    String mensagem = Serial.readStringUntil('\n');  
    
    if (mensagem == "liga") {
      // Verifica se o pino de feedback de ignição está em nível baixo
      if (fdb == 0) {     
        // Define o pino de saída de ignição como nível alto
        digitalWriteFast(SAIDA_IGNICAO_PIN, HIGH);  
        
        // Verifica se o pino de feedback de ignição mudou para nível alto
        fdb = digitalRead(IGNICAO_FEEDBACK_PIN);
        if (fdb == 1) {
          Serial.println("Ligado com sucesso!");  // Envia a mensagem "Ligado com sucesso" pela porta serial
        } else {
          Serial.println("Não foi possível ligar!");  // Envia a mensagem "Não foi possível ligar" pela porta serial
        }
      } else if (fdb == 1) {
        Serial.println("Já está ligado!");  // Envia a mensagem "Já está ligado" pela porta serial
      }
    } else if (mensagem == "desliga") {
      // Verifica se o pino de feedback de ignição está em nível alto
      if (fdb == 1) {                   
        // Define o pino de saída de ignição como nível baixo
        digitalWriteFast(SAIDA_IGNICAO_PIN, LOW);                 
        // Verifica se o pino de feedback de ignição mudou para nível baixo
        if(digitalRead(IGNICAO_FEEDBACK_PIN) == 0){
          Serial.println("Desligado com sucesso!");  // Envia a mensagem "Desligado com sucesso" pela porta serial
        }else{
          Serial.println("Não foi possível desligar!");
        }
      } else if (fdb == 0) {
        Serial.println("Já está desligado!");  // Envia a mensagem "Já está desligado" pela porta serial
      }
    }
    
    // Verifica se a mensagem é "partida"
    if(mensagem == "partida"){
      // Verifica se o pino de feedback de ignição está em nível alto
      if (fdb == 1) {  
		if(monitora_porta_direita() == 1 && monitora_porta_esquerda() == 1 ){
        dar_partida(2000);  // Inicia a partida
		}
	}else if(fdb == 0){
		Serial.println("Lige a chave e tente novamente!!");
	}
        mensagem = "";  // Limpa a mensagem para evitar partidas repetidas
      }
    }
  }


void dar_partida(int tempo){
digitalWriteFast(MOTOR_DE_ARRANQUE, HIGH); // aciona o motor de arranque colocando o pino em nível alto
delay(tempo); // aguarda o tempo definido pelo parâmetro 'tempo'
digitalWriteFast(MOTOR_DE_ARRANQUE, LOW); // desliga o motor de arranque colocando o pino em nível baixo
}



// Função para monitorar o estado da porta direita
// Retorna 0 se a porta está aberta e já passou um intervalo mínimo desde a última abertura,
// ou retorna 1 se a porta está fechada ou se ainda não passou tempo suficiente desde a última abertura.
int monitora_porta_direita(){
	agora_pd = millis(); // obtém o tempo atual em milissegundos
	int status_d = digitalRead(PORTA_DIREITA); // lê o estado da porta direita (0 para aberta e 1 para fechada)
	if(status_d == 0 && (agora_pd - antes_pd >= intervalo_pd) ){ // se a porta está aberta e já passou tempo suficiente desde a última abertura
		antes_pd = agora_pd; // atualiza o tempo da última abertura da porta
		Serial.println("Porta direita aberta!"); // imprime a mensagem informando que a porta foi aberta
		return 0; // retorna 0 para indicar que a porta está aberta
	}else{
	return 1; // retorna 1 para indicar que a porta está fechada ou ainda não passou tempo suficiente desde a última abertura
	}
};

// Função para monitorar o estado da porta esquerda
// Retorna 0 se a porta está aberta e já passou um intervalo mínimo desde a última abertura,
// ou retorna 1 se a porta está fechada ou se ainda não passou tempo suficiente desde a última abertura.
int monitora_porta_esquerda(){
	agora_pe = millis(); // obtém o tempo atual em milissegundos
	int status_e = digitalRead(PORTA_ESQUERDA); // lê o estado da porta esquerda (0 para aberta e 1 para fechada)
	if(status_e == 0 && (agora_pe - antes_pe >= intervalo_pe) ){ // se a porta está aberta e já passou tempo suficiente desde a última abertura
		antes_pe = agora_pe; // atualiza o tempo da última abertura da porta
		Serial.println("Porta esquerda aberta!"); // imprime a mensagem informando que a porta foi aberta
		return 0; // retorna 0 para indicar que a porta está aberta
	}else{
	return 1; // retorna 1 para indicar que a porta está fechada ou ainda não passou tempo suficiente desde a última abertura
	}
};


// Função que monitora o estado da porta do porta-malas de um veículo.
  
// A função verifica o estado atual do sensor da porta do porta-malas e o compara com o estado anterior.
// Se a porta estiver aberta e se o intervalo mínimo de tempo definido desde a última notificação foi atingido,
// a função imprime uma mensagem informando que a porta do porta-malas foi aberta.
// 
// @return 0 se a porta do porta-malas estiver aberta e o tempo mínimo de intervalo foi atingido desde a última notificação,
//         1 caso contrário (porta fechada ou tempo mínimo de intervalo ainda não foi atingido)
 int monitora_porta_mala() {
  // Captura o tempo atual em milissegundos
  agora_pm = millis();
  
  // Lê o estado atual do sensor da porta do porta-malas
  int status_m = digitalRead(PORTA_MALA);
  
  // Verifica se a porta está aberta e se o tempo mínimo de intervalo foi atingido desde a última notificação
  if(status_m == 0 && (agora_pm - antes_pm >= intervalo_pm) ) {
    // Atualiza o tempo da última notificação para o tempo atual
    antes_pm = agora_pm;
    
    // Imprime no monitor serial a mensagem de porta aberta
    Serial.println("Porta-mala aberta!");
    
    // Retorna 0 indicando que a porta está aberta e o tempo mínimo de intervalo foi atingido
    return 0;
  }
  
  // Retorna 1 indicando que a porta está fechada ou que o tempo mínimo de intervalo ainda não foi atingido
  return 1;
}

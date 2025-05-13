#include <SoftwareSerial.h> // Para comunicação com o ESP8266
#include <RCSwitch.h>       // Para controlar o transmissor RF

// --- Configuração da Comunicação Serial com o ESP8266 ---
SoftwareSerial espSerial(2, 3); 

// --- Configuração do Transmissor RF ---
RCSwitch mySwitch = RCSwitch();
const int RF_TRANSMITTER_PIN = 11; 

// --- Códigos RF ---
#define FAN_OFF_CODE      // setar valores     
#define FAN_SPEED_1_CODE  // setar valores
#define FAN_SPEED_2_CODE  // setar valores
#define FAN_SPEED_3_CODE  // setar valores
#define FAN_SPEED_4_CODE  // setar valores
#define FAN_SPEED_5_CODE  // setar valores

// --- Configurações do Sinal RF (Exemplos - SUBSTITUA!) ---
#define RF_BIT_LENGTH   24        
#define RF_PROTOCOL     1         
// #define RF_PULSE_LENGTH 350   

void setup() {
  Serial.begin(9600);
  while (!Serial); 

  // Inicia a serial para comunicação com o ESP8266
  espSerial.begin(9600);

  // Configura o pino do transmissor RF
  mySwitch.enableTransmit(RF_TRANSMITTER_PIN);

  // Configurações opcionais do RCSwitch
  mySwitch.setProtocol(RF_PROTOCOL);
  // mySwitch.setPulseLength(RF_PULSE_LENGTH);
  // mySwitch.setRepeatTransmit(10);

  Serial.println("Arduino pronto para receber comandos do ESP8266.");
  Serial.println("Aguardando comandos como: OFF, SPEED1, SPEED2, ..., SPEED6");
  espSerial.println("Arduino pronto."); // Envia uma mensagem de confirmação para o ESP8266
}

void loop() {
  // Verifica se há dados chegando do ESP8266
  if (espSerial.available()) {
    String command = espSerial.readStringUntil('\n'); // Lê o comando até encontrar uma nova linha
    command.trim(); // Remove espaços em branco e caracteres de nova linha

    Serial.print("Comando recebido do ESP8266: [");
    Serial.print(command);
    Serial.println("]");

    if (command.equalsIgnoreCase("OFF")) {
      sendRfCode(FAN_OFF_CODE, "DESLIGAR");
    } else if (command.equalsIgnoreCase("SPEED1")) {
      sendRfCode(FAN_SPEED_1_CODE, "VELOCIDADE 1");
    } else if (command.equalsIgnoreCase("SPEED2")) {
      sendRfCode(FAN_SPEED_2_CODE, "VELOCIDADE 2");
    } else if (command.equalsIgnoreCase("SPEED3")) {
      sendRfCode(FAN_SPEED_3_CODE, "VELOCIDADE 3");
    } else if (command.equalsIgnoreCase("SPEED4")) {
      sendRfCode(FAN_SPEED_4_CODE, "VELOCIDADE 4");
    } else if (command.equalsIgnoreCase("SPEED5")) {
      sendRfCode(FAN_SPEED_5_CODE, "VELOCIDADE 5");
    } else if (command.equalsIgnoreCase("SPEED6")) {
      sendRfCode(FAN_SPEED_6_CODE, "VELOCIDADE 6");
    } else if (command.length() > 0) { 
      Serial.print("Comando desconhecido: ");
      Serial.println(command);
    }
  }
}

// Função para enviar o código RF
void sendRfCode(unsigned long code, const char* description) {
  Serial.print("Enviando comando RF: ");
  Serial.print(description);
  Serial.print(" (Código: ");
  Serial.print(code);
  Serial.print(", Bits: ");
  Serial.print(RF_BIT_LENGTH);
  Serial.print(", Protocolo: ");
  Serial.print(RF_PROTOCOL);
  Serial.println(")");

  mySwitch.send(code, RF_BIT_LENGTH);
}

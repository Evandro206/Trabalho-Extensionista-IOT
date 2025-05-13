#include <SoftwareSerial.h> // Para comunicação com o ESP8266
#include <RCSwitch.h>     // Para controlar o transmissor RF

// --- Configuração da Comunicação Serial com o ESP8266 ---
// Pino RX do Arduino (conectado ao TX do ESP8266), Pino TX do Arduino (conectado ao RX do ESP8266)
SoftwareSerial espSerial(2, 3); // (RX, TX)

// --- Configuração do Transmissor RF ---
RCSwitch mySwitch = RCSwitch();
const int RF_TRANSMITTER_PIN = 10; // Pino do Arduino conectado ao pino DATA do transmissor RF

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! URGENTE: VOCÊ PRECISA SUBSTITUIR ESTES VALORES PELOS CÓDIGOS RF REAIS DO SEU VENTILADOR !!
// !! E PELAS CONFIGURAÇÕES CORRETAS (BIT LENGTH, PROTOCOL).                                 !!
// !! Estes são apenas VALORES DE EXEMPLO.                                                   !!
// !!                                                                                        !!
// !! Como encontrar os códigos?                                                             !!
// !! Use um módulo RECEPTOR RF (como o que aparece no seu diagrama, mas não está conectado   !!
// !! para controle) e um sketch "sniffer" no Arduino para capturar os códigos quando você   !!
// !! aperta os botões do controle remoto original do seu ventilador.                        !!
// !! Pesquise por "Arduino RF 433MHz sniffer" ou "RCSwitch receive demo".                   !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// --- Códigos RF (Exemplos - SUBSTITUA!) ---
// Estes valores são geralmente números decimais longos.
#define FAN_OFF_CODE      1234567UL // Exemplo de código para DESLIGAR
#define FAN_SPEED_1_CODE  1234501UL // Exemplo de código para VELOCIDADE 1
#define FAN_SPEED_2_CODE  1234502UL // Exemplo de código para VELOCIDADE 2
#define FAN_SPEED_3_CODE  1234503UL // Exemplo de código para VELOCIDADE 3
#define FAN_SPEED_4_CODE  1234504UL // Exemplo de código para VELOCIDADE 4
#define FAN_SPEED_5_CODE  1234505UL // Exemplo de código para VELOCIDADE 5
#define FAN_SPEED_6_CODE  1234506UL // Exemplo de código para VELOCIDADE 6 (conforme seu comando de voz)

// --- Configurações do Sinal RF (Exemplos - SUBSTITUA!) ---
#define RF_BIT_LENGTH   24        // Número de bits do seu código RF (comum ser 24)
#define RF_PROTOCOL     1         // Protocolo RF (o padrão da RCSwitch é 1, tente outros se não funcionar: 1 a 7)
// #define RF_PULSE_LENGTH 350    // Opcional: Duração do pulso em microssegundos. Descomente e ajuste se necessário.

void setup() {
  // Inicia a serial para debug no Monitor Serial da IDE do Arduino
  Serial.begin(9600);
  while (!Serial); // Espera a Serial estar pronta (para Arduinos como Leonardo/Micro, opcional para UNO)

  // Inicia a serial para comunicação com o ESP8266
  // Certifique-se que o baud rate é o mesmo configurado no ESP8266
  espSerial.begin(9600);

  // Configura o pino do transmissor RF
  mySwitch.enableTransmit(RF_TRANSMITTER_PIN);

  // Configurações opcionais do RCSwitch (descomente e ajuste se necessário)
  mySwitch.setProtocol(RF_PROTOCOL);
  // mySwitch.setPulseLength(RF_PULSE_LENGTH);
  // mySwitch.setRepeatTransmit(10); // Número de repetições do sinal (padrão é 10, pode aumentar para melhor recepção)

  Serial.println("Arduino pronto para receber comandos do ESP8266.");
  Serial.println("Aguardando comandos como: OFF, SPEED1, SPEED2, ..., SPEED6");
  espSerial.println("Arduino pronto."); // Envia uma mensagem de confirmação para o ESP8266 (opcional)
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
    } else if (command.length() > 0) { // Evita mensagens para comandos vazios
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

  // Um pequeno delay pode ser útil às vezes, mas setRepeatTransmit é geralmente melhor.
  // delay(100);
}
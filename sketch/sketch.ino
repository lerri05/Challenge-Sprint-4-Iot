#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// --- CONFIGURAÇÃO Wi-Fi ---
const char* ssid = "Wokwi-GUEST";       // 🟢 Troque pelo seu SSID
const char* password = "";  // 🟢 Troque pela senha

 //Endpoint do servidor (pode ser um backend, Flask, Node, ou webhook test)
const char* serverURL = "http://SEU_SERVIDOR/api/moto"; // 🟢 Ex: http://192.168.0.10:5000/api/moto

// --- PINOS ---
const int botaoEstado = 15;
const int botaoLocal = 4;
const int ledUso = 25;
const int ledParada = 26;
const int ledManut = 27;
const int buzzer = 14;

// --- LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- ENUMERAÇÃO DE ESTADOS ---
enum EstadoMoto {
  PARADA = 0,
  EM_USO = 1,
  MANUTENCAO = 2
};

// --- VARIÁVEIS ---
EstadoMoto estadoAtual = PARADA;
int posicaoAtual = 1;
bool botaoEstadoAnterior = false;
bool botaoLocalAnterior = false;
unsigned long tempoAnterior = 0;
const unsigned long intervaloPisca = 500;
bool pisca = false;

// --- EEPROM CONFIG ---
const int EEPROM_SIZE = 2048;
const int addrEstado = 0;
const int addrPosicao = 10;

// --- FUNÇÕES EEPROM ---
void salvarEstado(EstadoMoto estado) {
  EEPROM.put(addrEstado, estado);
  EEPROM.commit();
}

void salvarPosicao(int pos) {
  EEPROM.put(addrPosicao, pos);
  EEPROM.commit();
}

EstadoMoto lerEstado() {
  EstadoMoto estado;
  EEPROM.get(addrEstado, estado);
  if (estado < PARADA || estado > MANUTENCAO) estado = PARADA;
  return estado;
}

int lerPosicao() {
  int pos;
  EEPROM.get(addrPosicao, pos);
  if (pos < 1 || pos > 9) pos = 1;
  return pos;
}

// --- FUNÇÃO: ENVIAR DADOS PARA SERVIDOR ---
void enviarDados() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"estado\": \"" + String(
      estadoAtual == EM_USO ? "Em uso" :
      estadoAtual == PARADA ? "Parada" : "Manutencao") + "\",";
    json += "\"posicao\": " + String(posicaoAtual);
    json += "}";

    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      Serial.printf("📡 Dados enviados! Código HTTP: %d\n", httpResponseCode);
    } else {
      Serial.printf("⚠️ Erro ao enviar: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("❌ WiFi desconectado. Tentando reconectar...");
    WiFi.reconnect();
  }
}

// --- DISPLAY ---
void atualizarDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Estado: ");
  switch (estadoAtual) {
    case EM_USO:
      lcd.print("Em uso");
      break;
    case PARADA:
      lcd.print("Parada");
      break;
    case MANUTENCAO:
      lcd.print("Manutencao");
      break;
  }

  lcd.setCursor(0, 1);
  for (int i = 1; i <= 9; i++) {
    if (i == posicaoAtual)
      lcd.print("X");
    else
      lcd.print(".");
    if (i % 3 == 0 && i < 9) lcd.print(" ");
  }

  Serial.print("Moto -> Estado: ");
  Serial.print(estadoAtual == EM_USO ? "Em uso" :
               estadoAtual == PARADA ? "Parada" : "Manutencao");
  Serial.print(" | Posicao: ");
  Serial.println(posicaoAtual);
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  lcd.init();
  lcd.backlight();

  pinMode(botaoEstado, INPUT_PULLUP);
  pinMode(botaoLocal, INPUT_PULLUP);
  pinMode(ledUso, OUTPUT);
  pinMode(ledParada, OUTPUT);
  pinMode(ledManut, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Conecta ao Wi-Fi
  Serial.print("Conectando ao Wi-Fi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Conectado ao Wi-Fi!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Falha ao conectar ao Wi-Fi.");
  }

  estadoAtual = lerEstado();
  posicaoAtual = lerPosicao();
  atualizarDisplay();
}

// --- LOOP ---
void loop() {
  bool estadoAtualBotao = !digitalRead(botaoEstado);
  bool localAtualBotao = !digitalRead(botaoLocal);

  // Mudar o estado
  if (estadoAtualBotao && !botaoEstadoAnterior) {
    estadoAtual = static_cast<EstadoMoto>((estadoAtual + 1) % 3);
    salvarEstado(estadoAtual);
    atualizarDisplay();
    enviarDados(); // <-- Envia atualização
    delay(200);
  }

  // Mudar a posição
  if (localAtualBotao && !botaoLocalAnterior) {
    posicaoAtual++;
    if (posicaoAtual > 9) posicaoAtual = 1;
    salvarPosicao(posicaoAtual);
    atualizarDisplay();
    enviarDados(); // <-- Envia atualização
    delay(200);
  }

  botaoEstadoAnterior = estadoAtualBotao;
  botaoLocalAnterior = localAtualBotao;

  // LEDs e buzzer
  digitalWrite(ledUso, estadoAtual == EM_USO);
  digitalWrite(ledParada, estadoAtual == PARADA);
  digitalWrite(ledManut, estadoAtual == MANUTENCAO);

  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervaloPisca) {
    tempoAnterior = tempoAtual;
    pisca = !pisca;
    if (estadoAtual == MANUTENCAO) {
      digitalWrite(ledManut, pisca);
      digitalWrite(buzzer, pisca);
    } else {
      digitalWrite(buzzer, LOW);
    }
  }
}

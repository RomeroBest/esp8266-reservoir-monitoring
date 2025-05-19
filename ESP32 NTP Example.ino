#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <PubSubClient.h>

// Definições de pinos
#define TRIG_PIN 4
#define ECHO_PIN 5
#define RELAY_PIN 2
#define LED_VERDE 13
#define LED_VERMELHO 12

// Configurações da rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações do servidor NTP
#define UTC_OFFSET -10800
#define UTC_OFFSET_DST 0
#define NTP_SERVER "pool.ntp.org"

/* ① MQTT server / porta  */
const char* mqtt_server = "mqtt.thingsboard.cloud";
const int   mqtt_port   = 1883;

/* ② Token = usuário, senha vazia */
const char* mqtt_client_id = "fuyre2ce3ewmieltbggs";
const char* mqtt_user = "omlwww90zzjzn21wiylo";
const char* mqtt_pass = "123456789";                      

/* ③ Tópico de telemetria do TB */
const char* mqtt_topic = "v1/devices/me/telemetry";


// Inicializações
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis do sistema
const float capacidadeMaxima = 390.0;
float metade = capacidadeMaxima / 2.0;
bool bombaLigada = false;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_pass)) {
      Serial.println("Conectado ao ThingsBoard!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}


float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  int duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi");

  setup_wifi();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi conectado");

  // Configura horário
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  delay(1000);
  lcd.clear();

  client.setServer(mqtt_server, mqtt_port);
  if (client.connect(mqtt_client_id, mqtt_user, mqtt_pass)) {
    Serial.println("Conectado ao ThingsBoard!");
  } else {
    Serial.print("Falhou: ");
    Serial.println(client.state());
    delay(5000);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float distancia = readDistanceCM();

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao obter hora");
    return;
  }

  char horaStr[6];
  strftime(horaStr, sizeof(horaStr), "%H:%M", &timeinfo);

// Calcular percentual
  int capacidadeTotal = 400;
  int nivelPercentual = map(distancia, capacidadeTotal, 0, 0, 100);
  nivelPercentual = constrain(nivelPercentual, 0, 100);

  // Exibe distância e hora na primeira linha do LCD
  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.print(distancia, 0);
  lcd.print("cm ");

  lcd.print(nivelPercentual);
  lcd.print("% ");

  lcd.print(horaStr);


  // Atualiza lógica da bomba
  if (distancia >= metade) {
    // A água está baixa → Ligar bomba
    bombaLigada = true;
  } else if (distancia <= 5.0) {
    // A água chegou ao topo → Desligar bomba
    bombaLigada = false;
  }


  // Executa ação e mostra estado no LCD e terminal
  if (bombaLigada) {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Bomba acionada ");
    Serial.print("Bomba acionada - Nivel: ");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Bomba desligada");
    Serial.print("Bomba desligada - Nivel: ");
  }

  // Imprime a distância e horário no terminal
  Serial.print(distancia);
  Serial.print(" cm às ");
  Serial.println(horaStr);


  // Publica os dados no tópico MQTT
  String payload = "{";
  payload += "\"nivel_percentual\": ";
  payload += String(nivelPercentual);
  payload += ", \"status\": \"";
  payload += (bombaLigada ? "bomba_ligada" : "bomba_desligada");
  payload += "\"}";

  client.publish(mqtt_topic, payload.c_str());

  delay(5000);
}

client.setCallback(callback);
client.subscribe("v1/devices/me/rpc/request/+");

void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  if (deserializeJson(doc, payload, length)) return;
  const char* method = doc["method"];

  // já existente: setValue
  if (strcmp(method, "setValue") == 0) {
    bombaLigada = doc["params"]["value"];
  }
  // novo: getValue
  else if (strcmp(method, "getValue") == 0) {
    // topic será: v1/devices/me/rpc/request/<id>
    // precisamos publicar em: v1/devices/me/rpc/response/<id>
    String req = String(topic);
    String respTopic = req.replace("request", "response");
    StaticJsonDocument<64> resp;
    resp["value"] = bombaLigada;
    char buf[64];
    serializeJson(resp, buf);
    client.publish(respTopic.c_str(), buf);
  }
}


# 📘 Explicação do Funcionamento do Código – Monitoramento de Nível com ESP32 e MQTT

Este documento explica, de forma detalhada e didática, o funcionamento do código-fonte utilizado no projeto de monitoramento e controle de nível de água com ESP32 e ThingsBoard.

---

## 1. Inclusão de Bibliotecas

```cpp
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <PubSubClient.h>
```

Essas bibliotecas permitem:
- Conectar o ESP32 ao Wi-Fi;
- Usar o display LCD via protocolo I2C;
- Trabalhar com data e hora usando o NTP;
- Comunicar com o broker MQTT (ThingsBoard).

---

## 2. Definições Iniciais

### Pinos

```cpp
#define TRIG_PIN 4
#define ECHO_PIN 5
#define RELAY_PIN 2
#define LED_VERDE 13
#define LED_VERMELHO 12
```

Pinos usados para sensor ultrassônico, relé, e LEDs indicadores.

### Rede e MQTT

```cpp
const char* ssid = "Wokwi-GUEST";
const char* mqtt_server = "mqtt.thingsboard.cloud";
const char* mqtt_topic = "v1/devices/me/telemetry";
```

Contém as credenciais de Wi-Fi, token e configuração do ThingsBoard via MQTT.

---

## 3. Objetos Globais

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient espClient;
PubSubClient client(espClient);
```

Objetos que controlam o display e a comunicação com o MQTT.

---

## 4. Variáveis do Sistema

```cpp
const float capacidadeMaxima = 390.0;
float metade = capacidadeMaxima / 2.0;
bool bombaLigada = false;
```

Define os limites do reservatório e o estado da bomba.

---

## 5. Conexão Wi-Fi e MQTT

### Função `setup_wifi()`

Conecta o ESP32 ao Wi-Fi.

### Função `reconnect()`

Garante que o ESP32 esteja conectado ao broker MQTT, tentando novamente se cair.

---

## 6. Leitura do Sensor Ultrassônico

```cpp
float readDistanceCM() { ... }
```

Envia o pulso ultrassônico e lê o tempo de retorno para calcular a distância.

---

## 7. Função `setup()`

- Inicializa o LCD;
- Define os modos dos pinos;
- Conecta à internet e configura o relógio;
- Estabelece conexão MQTT com ThingsBoard.

---

## 8. Função `loop()` – Fluxo Principal

### a) Verifica e reconecta ao MQTT se necessário

```cpp
if (!client.connected()) { reconnect(); }
client.loop();
```

### b) Lê a distância da água

```cpp
float distancia = readDistanceCM();
```

### c) Obtém o horário atual

```cpp
getLocalTime(&timeinfo);
```

### d) Calcula o nível de água em %

```cpp
int nivelPercentual = map(distancia, capacidadeTotal, 0, 0, 100);
```

### e) Exibe no display

Mostra distância, percentual e hora no LCD.

### f) Atualiza a lógica da bomba

- Liga se nível estiver abaixo da metade.
- Desliga se o nível estiver cheio (≤ 5cm).

### g) Aciona LEDs e relé

- Verde → bomba ligada
- Vermelho → bomba desligada

### h) Publica dados via MQTT

Cria um JSON com:
```json
{
  "nivel_percentual": 70,
  "status": "bomba_ligada",
  "tempo_sensor_ms": 200
}
```
E envia para o ThingsBoard.

### i) Espera 5 segundos

Evita sobrecarga no sistema.

---

## 🔚 Conclusão

Este código integra sensores, atuadores, conectividade com internet e exibição local em um único sistema funcional, ideal para projetos de automação com monitoramento remoto.

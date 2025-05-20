# 💧 Monitoramento de Nível com ESP32 e MQTT

Este projeto utiliza um ESP32 para monitorar o nível de um reservatório de água com sensor ultrassônico, exibindo os dados localmente em um display LCD e remotamente via MQTT na plataforma ThingsBoard.

---

## 🔧 Componentes Utilizados

- ESP32 DevKit-C V4
- Sensor ultrassônico HC-SR04
- Display LCD 16x2 (I2C)
- Relé
- LEDs (verde e vermelho)
- Cabos e jumpers

---

## ⚙️ Funcionamento

1. O sensor HC-SR04 mede a distância até a água.
2. A distância é convertida em nível de preenchimento.
3. O display LCD mostra o valor em tempo real.
4. LEDs indicam o estado (normal ou alerta).
5. O relé pode ser ativado se o nível estiver baixo.
6. Os dados são publicados via MQTT para o ThingsBoard.

---

## 🌐 Comunicação

- Conexão Wi-Fi (TCP/IP)
- Protocolo MQTT
- Integração com ThingsBoard usando token do dispositivo

---

## 📦 Bibliotecas

- LiquidCrystal_I2C
- HCSR04
- PubSubClient
- WiFi

---

## 📲 Como Usar

1. Configure a rede Wi-Fi e token MQTT no código.
2. Faça upload para o ESP32.
3. Acompanhe a leitura no display e na nuvem.

---

## 🔗 Simulação

Projeto Wokwi:  
https://wokwi.com/projects/431378864051833857

---

Desenvolvido por **Jorge Romero** – 2025

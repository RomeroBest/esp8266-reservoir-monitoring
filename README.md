# 💧 Monitoramento de Nível com ESP32 e MQTT

Este projeto utiliza um ESP32 para monitorar o nível de um reservatório de água com sensor ultrassônico, exibindo os dados localmente em um display LCD e remotamente via MQTT na plataforma ThingsBoard.

---

## 🔧 Componentes Utilizados

- ESP32 DevKit-C V4
- Sensor ultrassônico HC-SR04
- Display LCD 16x2 (I2C)
- Módulo Relé
- LEDs: verde (normal) e vermelho (alerta)
- Jumpers e alimentação 5V/3.3V

---

## ⚙️ Funcionamento do Sistema

1. O sensor ultrassônico mede a distância até a superfície da água.
2. A distância é convertida em nível percentual.
3. O LCD exibe a leitura em tempo real.
4. LEDs indicam o status do reservatório.
5. O relé pode ativar dispositivos (ex: bomba) automaticamente.
6. Os dados são publicados via MQTT para o ThingsBoard.

---

## 🌐 Comunicação

- Protocolo: MQTT sobre TCP/IP
- Plataforma: [ThingsBoard Cloud](https://thingsboard.cloud)
- Bibliotecas:
  - `WiFi.h`
  - `PubSubClient`
  - `LiquidCrystal_I2C`
  - `HCSR04`

---

## 🛠️ Como Configurar o Projeto

### 📌 Wokwi

1. Acesse: [https://wokwi.com/projects/431378864051833857](https://wokwi.com/projects/431378864051833857)
2. Clique em **"Code"** e cole seu código `.ino`.
3. Confirme que os componentes estão conectados conforme o `diagram.json`.
4. Se quiser testar online sem MQTT, comente as funções de Wi-Fi e MQTT no código.

---

### ☁️ ThingsBoard (MQTT)

1. Acesse [https://thingsboard.cloud](https://thingsboard.cloud) e crie uma conta.
2. Vá em **Devices > Add new device**.
3. Copie o **Access Token** do dispositivo.
4. No código `.ino`, substitua:
   ```cpp
   const char* mqttServer = "thingsboard.cloud";
   const char* token = "SEU_TOKEN_AQUI";


5. Visualize os dados criando um **Dashboard** com widgets conectados ao `device`.

🖼️ Veja o exemplo no arquivo: `DASHBOARD_THINGSBOARD.png`

---

## 📦 Arquivos do Projeto

* `ESP32 NTP Example.ino` → Código principal
* `diagram.json` → Esquema de montagem
* `libraries.txt` → Bibliotecas necessárias
* `DASHBOARD_THINGSBOARD.png` → Exemplo de dashboard
* `SISTEMA_DIAGRAMJSON.png` → Ilustração do sistema
* `wokwi-project.txt` → Link para simulação

---

## 👨‍💻 Autor

Desenvolvido por **Jorge Romero**
Disciplina: Internet das Coisas — 2025



---

### ✅ O que esse `README.md` cobre:
- Descrição geral do sistema
- Hardware usado
- Funcionamento passo a passo
- Instruções completas de **como configurar o Wokwi**
- Como **integrar o projeto ao ThingsBoard**
- Referência aos arquivos visuais (`.png`) que você já subiu

Se desejar, posso gerar esse arquivo para você agora no formato `.md`. Deseja que eu envie como arquivo?


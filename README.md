# 🏍️ Sistema de Localização de Motos no Pátio (ESP32 / Wokwi)

Este projeto implementa um **sistema de monitoramento e localização de motocicletas** utilizando um **ESP32**, com **LCD I2C**, **LEDs**, **buzzer**, **EEPROM** e **conexão Wi-Fi** para envio de dados a um servidor.  

O sistema permite alternar **estados da moto** (Parada, Em uso, Manutenção) e **posições no pátio (1–9)**, exibindo as informações em um **display LCD 16x2** e enviando as atualizações via HTTP para uma API ou servidor remoto.

---

## 📋 Descrição do Problema
Em pátios de manutenção ou empresas com múltiplas motocicletas, é comum a dificuldade de **rastrear a localização física e o estado de cada veículo**.  
Este protótipo mostra como um sistema embarcado pode **registrar, exibir e enviar dados de status e posição** de forma simples, eficiente e conectada à nuvem.

---

## 🔧 Funcionalidades
- **Troca de estado da moto (3 modos)**  
  - `PARADA`, `EM USO`, `MANUTENÇÃO`.  
  - Cada clique no botão de estado alterna o modo.  

- **Alteração da posição no pátio (1–9)**  
  - Um botão dedicado percorre as posições no grid exibido no LCD.  

- **Exibição no LCD (16x2 I2C)**  
  - Primeira linha: estado atual da moto.  
  - Segunda linha: grid de 9 posições com um “X” indicando a posição atual.  

- **Indicação visual e sonora**  
  - LEDs representam cada estado da moto:  
    - 🟢 `Em uso`  
    - 🟡 `Parada`  
    - 🔴 `Manutenção` (pisca junto ao buzzer)  

- **Conexão Wi-Fi + Envio para servidor**  
  - Envia o estado e a posição da moto via **HTTP POST** em formato **JSON**.  
  - Pode ser integrado a APIs Flask, Node.js ou webhooks de teste.  

- **Persistência de dados (EEPROM)**  
  - O último estado e posição são salvos e recuperados automaticamente após reiniciar o sistema.  

- **Dashboard Serial**  
  - Exibe logs com estado atual, posição, IP e status da conexão Wi-Fi.

---

## 🚀 Tecnologias Utilizadas
- ⚙️ **ESP32** como microcontrolador principal.  
- 📡 **Wi-Fi** integrado para comunicação com servidor remoto.  
- 💾 **EEPROM** para armazenamento persistente.  
- 💡 **LCD 16x2 I2C** para exibir status e posição.  
- 🔘 **Botões físicos** para alternar estado e posição.  
- 💡 **LEDs (x3)** como sinalização visual.  
- 🔊 **Buzzer** para alerta sonoro.  
- 🧠 **C++ / Arduino IDE** para desenvolvimento.  
- 🌐 **Wokwi** para simulação do hardware.  

---

## 🗂️ Estrutura do Projeto
- **`main.ino`**: lógica de controle dos botões, LEDs, buzzer, LCD e Wi-Fi.  
- **`EEPROM`**: salva o estado atual e posição da moto.  
- **`HTTPClient`**: envia dados para o servidor configurado.  
- **`Serial Monitor`**: mostra logs de status, IP, respostas HTTP e erros.  

---

## 🔨 Etapas do Projeto
✔️ Conexão automática ao Wi-Fi com feedback no Serial Monitor.  
✔️ Implementação de grid de localização no LCD.  
✔️ Enumeração dos estados da moto (Parada, Em uso, Manutenção).  
✔️ Alternância de estado e posição com botões físicos.  
✔️ Persistência de dados com EEPROM.  
✔️ Envio automático de dados via HTTP POST.  
✔️ Indicação visual e sonora dos modos de operação.  

---

## ▶️ Como Executar
1. **Clone este repositório**.  
2. Abra o projeto na **Arduino IDE** ou no **Wokwi**.  
3. Instale as bibliotecas necessárias:  
   - `WiFi.h`  
   - `HTTPClient.h`  
   - `LiquidCrystal_I2C.h`  
   - `EEPROM.h`  
4. Configure as credenciais Wi-Fi no código:
   ```cpp
   const char* ssid = "SEU_WIFI";
   const char* password = "SENHA";
   const char* serverURL = "http://SEU_SERVIDOR/api/moto";

Componentes e pinos definidos:
| Componente           | Pino ESP32         |
| -------------------- | ------------------ |
| Botão de Estado      | 15                 |
| Botão de Localização | 4                  |
| LED “Em uso”         | 25                 |
| LED “Parada”         | 26                 |
| LED “Manutenção”     | 27                 |
| Buzzer               | 14                 |
| LCD I2C              | SDA (21), SCL (22) |

🌐 Exemplo de JSON Enviado
{
  "estado": "Em uso",
  "posicao": 5
}

🗺️ Exemplo de Grid de Localização
1 2 3
4 5 6
7 8 9
O símbolo “X” no LCD indica a posição atual da moto no pátio.

📜 Licença

Projeto desenvolvido para fins educacionais e de prototipagem, com foco em monitoramento e rastreamento de motocicletas em pátios de manutenção.

👥 Autores
| Nome                     | RM     | GitHub                                            |
| ------------------------ | ------ | ------------------------------------------------- |
| Fernanda Budniak de Seda | 558274 | [Febudniak](https://github.com/Febudniak)         |
| Lucas Lerri de Almeida   | 554635 | [lerri05](https://github.com/lerri05)             |
| Karen Marques dos Santos | 554556 | [KarenMarquesS](https://github.com/KarenMarquesS) |



# 📺 Painel LCD via Wi-Fi — ESP32-C3 SuperMini
 
Projeto de exibição de mensagens em display LCD 16x2 controlado remotamente pelo smartphone. O ESP32-C3 cria sua própria rede Wi-Fi, permitindo que qualquer dispositivo conectado acesse um painel web e envie textos diretamente para o display — com efeito de letreiro deslizante automático para mensagens longas.
 
---
 
## Demonstração 📸
 
> **Foto do circuito montado**

> <img width="300" height="400" alt="WhatsApp Image 2026-06-06 at 22 58 32" src="https://github.com/user-attachments/assets/6b6d2fe3-a9c6-40e2-b9e3-e573654cfd02" />


 
---
 
> **Vídeo do funcionamento**

> 
https://github.com/user-attachments/assets/f2cb64d9-bd03-41e3-8d78-a3a0a2a625eb

---
 
## ✨ Funcionalidades
 
- Cria uma rede Wi-Fi própria (modo Access Point) sem depender de roteadores externos
- Interface web responsiva acessível pelo navegador do celular
- Exibição instantânea da mensagem no LCD ao enviar pelo formulário
- Efeito de letreiro deslizante automático para mensagens com mais de 16 caracteres
- Velocidade de scroll configurável via variável `velocidadeScroll`
- IP fixo configurável para acesso consistente
---
 
## 🛒 Materiais
 
| Componente | Quantidade |
|---|---|
| ESP32-C3 SuperMini | 1 |
| Display LCD 16x2 (HD44780) | 1 |
| Potenciômetro 10kΩ (ajuste de contraste) | 1 |
| Resistor 220Ω (backlight) | 1 |
| Protoboard | 1 |
| Jumpers | — |
 
---
 
## 🔌 Conexões
 
| LCD (Pino) | ESP32-C3 SuperMini |
|---|---|
| VSS | GND |
| VDD | 5V |
| V0 (contraste) | Cursor do potenciômetro 10kΩ |
| RS | GPIO 2 |
| RW | GND |
| E (Enable) | GPIO 3 |
| D4 | GPIO 4 |
| D5 | GPIO 5 |
| D6 | GPIO 6 |
| D7 | GPIO 7 |
| A (backlight +) | 5V via resistor 220Ω |
| K (backlight −) | GND |
 
---
 
## 📦 Dependências / Ambiente
 
- **Plataforma:** PlatformIO (VSCode) ou Arduino IDE 2.x
- **Board:** `esp32:esp32:esp32c3` (ESP32-C3 SuperMini)
- **Bibliotecas:**
  - `LiquidCrystal` — disponível no Library Manager
  - `WiFi.h` e `WebServer.h` — inclusas no pacote ESP32 para Arduino
**`platformio.ini`:**
```ini
[env:esp32-c3-devkitm-1]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
lib_deps =
    arduino-libraries/LiquidCrystal
```
 
---
 
## ⚙️ Configuração da Rede Wi-Fi
 
O ESP32 opera em modo **Access Point** — ele cria a própria rede, sem depender de roteador externo.
 
1. No arquivo `src/main.cpp`, edite as credenciais:
```cpp
const char* ssid     = "NomeDaSuaRede";
const char* password = "SuaSenha";
```
 
2. O IP fixo padrão é `192.168.18.55`. Para alterar, edite:
```cpp
IPAddress local_IP(192, 168, 18, 55);
IPAddress gateway(192, 168, 18, 55);
```
 
3. Grave o código no ESP32, conecte-se à rede criada por ele e acesse `http://192.168.18.55` no navegador do celular.
---
 
## ⚡ Como Funciona
 
O ESP32-C3 inicializa o display LCD e sobe um servidor web na porta 80 operando como ponto de acesso Wi-Fi. Ao acessar o IP pelo navegador, o usuário vê um formulário simples onde digita a mensagem e clica em enviar.
 
A rota `/enviar` recebe o texto via POST, atualiza a variável `mensagem` e reinicia o índice de scroll. Na `loop()`, o código verifica continuamente o tamanho da mensagem: textos com até 16 caracteres ficam estáticos na primeira linha; mensagens mais longas passam por um efeito de letreiro deslizante, avançando um caractere a cada `velocidadeScroll` milissegundos (padrão: 350 ms) e reiniciando ao chegar ao fim.
 
---
 
## 💻 Código
 
```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal.h>
 
// ================= Configurações do Display =================
// Pinos do ESP32-C3 SuperMini: RS, E, D4, D5, D6, D7
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
int lcdColumns = 16;
int lcdRows = 2;
 
// ================= Configurações do Wi-Fi =================
const char* ssid     = "SuaRede";
const char* password = "SuaSenha";
 
WebServer server(80);
 
// ================= Variáveis Globais =================
String mensagem = "Conecte no WiFi";
unsigned long ultimoScroll = 0;
int scrollIndex = 0;
int velocidadeScroll = 350;
 
void setup() {
  Serial.begin(115200);
 
  lcd.begin(lcdColumns, lcdRows);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
 
  // ================= Configuração IP Fixo do AP =================
  IPAddress local_IP(192, 168, 18, 55);
  IPAddress gateway(192, 168, 18, 55);
  IPAddress subnet(255, 255, 255, 0);
 
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
 
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Rede Wi-Fi ativa! Acesse o painel em: http://");
  Serial.println(IP);
 
  // ================= Rotas do Servidor Web =================
 
  // 1. Página inicial com o formulário
  server.on("/", HTTP_GET, []() {
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<style>body{font-family: Arial; text-align: center; background-color: #f0f0f0; margin-top: 50px;}";
    html += "input[type=text]{padding: 12px; width: 80%; border-radius: 8px; border: 1px solid #ccc; font-size: 16px;}";
    html += "input[type=submit]{padding: 12px 20px; margin-top: 15px; background-color: #4CAF50; color: white; border: none; border-radius: 8px; font-size: 16px; cursor: pointer;}</style></head>";
    html += "<body><h2>Painel LCD ESP32</h2>";
    html += "<form action='/enviar' method='POST'>";
    html += "<input type='text' name='texto' placeholder='Digite sua mensagem...' maxlength='100'>";
    html += "<br><input type='submit' value='Enviar para o Display'>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
  });
 
  // 2. Rota que recebe o texto digitado
  server.on("/enviar", HTTP_POST, []() {
    if (server.hasArg("texto")) {
      mensagem = server.arg("texto");
      lcd.clear();
      scrollIndex = 0;
      if (mensagem.length() > lcdColumns) {
        mensagem += "                "; // 16 espaços para o efeito de loop limpo
      }
    }
    server.sendHeader("Location", "/");
    server.send(303);
  });
 
  server.begin();
  lcd.clear();
}
 
void loop() {
  server.handleClient();
 
  // ================= Lógica do Display (Scroll) =================
  if (mensagem.length() <= lcdColumns) {
    // Texto curto: fixo na tela
    lcd.setCursor(0, 0);
    lcd.print(mensagem);
  } else {
    // Texto longo: efeito letreiro deslizante
    if (millis() - ultimoScroll > velocidadeScroll) {
      ultimoScroll = millis();
      lcd.setCursor(0, 0);
 
      String textoTela = mensagem.substring(scrollIndex, scrollIndex + lcdColumns);
      while (textoTela.length() < lcdColumns) {
        textoTela += " ";
      }
 
      lcd.print(textoTela);
      scrollIndex++;
 
      if (scrollIndex >= mensagem.length() - lcdColumns + 1) {
        scrollIndex = 0;
      }
    }
  }
}
```
 
---
 
## 🧑‍💻 Sobre o Projeto
 
Desenvolvido como segunda branch do repositório [Escrita na Tela LCD](https://github.com/Antonymacedo/Escrita_na_Tela_LCD/tree/main), este projeto expande a proposta original adicionando controle remoto via Wi-Fi com interface web. Ideal para painéis de avisos, displays de informação em tempo real ou qualquer aplicação onde seja necessário atualizar textos no LCD sem contato físico com o hardware.
 
Construído com **ESP32-C3 SuperMini**, **LiquidCrystal** e **WebServer**.

**Autores:** Antony Macedo, Isaac  
**Ambiente:** PlatformIO + ESP32 Arduino Core  
**Licença:** MIT

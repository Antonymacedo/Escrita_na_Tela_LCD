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
const char* ssid     = "SuaRede";   // <-- altere aqui
const char* password = "SuaSenha";  // <-- altere aqui

WebServer server(80);

// ================= Variáveis Globais =================
String mensagem = "Conecte no WiFi";
unsigned long ultimoScroll = 0;
int scrollIndex = 0;
int velocidadeScroll = 350; // ms por passo de scroll

void setup() {
  Serial.begin(115200);

  // Inicializa o LCD (16 colunas, 2 linhas)
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

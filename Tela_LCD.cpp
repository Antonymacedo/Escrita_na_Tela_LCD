#include <LiquidCrystal.h> //Inclui a biblioteca do LiquidCrytal
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Declara as portas de saída para o LCD
void setup() {
  lcd.begin(16, 2); //Inicia a escrita no LCD
  lcd.print("GITHUB ANTONY");//Exibe a mensagem que irá imprimir
}
void loop() {
  lcd.setCursor(0, 1); //Define o cursor em (0,1)
  lcd.print(millis()/1000); //Faz a contagem do tempo e o mostra na tela do LCD
  lcd.print("s"); //Escreve a letra s para representar os segundos
}
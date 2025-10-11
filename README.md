# Escrita na tela LCD🤖

Este circuito utiliza um display LCD 16x2 conectado a um Arduino para exibir mensagens e realizar uma contagem de tempo em segundos. Assim que o sistema é iniciado, a tela mostra uma mensagem personalizada (“GITHUB ANTONY”) e, em seguida, começa a atualizar continuamente o tempo de execução do programa. O projeto é simples, mas excelente para entender o funcionamento básico da biblioteca LiquidCrystal, além de servir como base para aplicações que envolvem monitoramento em tempo real, como cronômetros, temporizadores ou sistemas de status.

## Materiais⚙️

<img width="502" height="260" alt="image" src="https://github.com/user-attachments/assets/7c5d3d2d-9fb9-4264-842c-91cdb0c58e3c" />

 
 * Tela LCD

 * Potenciometro

 * Arduino Uno


## Montagem🛠️


<img width="50%" alt="Imagem do WhatsApp de 2025-10-03 à(s) 09 32 58_2a1dab3c" src="https://github.com/user-attachments/assets/c672b9b5-98b2-437a-8b14-ac83267646e4" />

Monte o Arduino na protoboard e conecte as linhas de energia para distribuir 5V e GND. Posicione o display LCD 16x2 na parte lateral da protoboard, deixando espaço para o potenciômetro logo acima dele, facilitando o ajuste de contraste.

Conecte o potenciômetro de forma que seus terminais fiquem ligados ao positivo e ao negativo da alimentação, com o pino central voltado para o pino de contraste do LCD. As conexões do display são organizadas com fios curtos e bem distribuídos entre os pinos digitais do Arduino e a fileira superior da protoboard.

Depois de concluída a montagem, verifique se os trilhos de 5V e GND estão corretamente ligados ao Arduino. Conecte o cabo USB para alimentar o circuito e gire o potenciômetro até que o texto apareça no display, confirmando o funcionamento correto.

## Esquema de Conexão⚡️

| Componente                  | Pino do Arduino       |
| --------------------------- | --------------------- |
| RS -> LCD                   | D12                   |
| E -> LCD                    | D11                   |
| D4 -> LCD                   | D5                    |
| D5 -> LCD                   | D4                    |
| D6 -> LCD                   | D3                    |
| D7 -> LCD                   | D2                    |
| V0 -> LCD (Contraste)       | Meio do potenciômetro |
| Potenciômetro -> Terminal 1 | GND                   |
| Potenciômetro -> Terminal 2 | 5V                    |


## Código💻

```
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
```
## Projeto no TinkerCAD❗

https://www.tinkercad.com/things/8pVD0WnAPAc-display-lcd-16x2

## Resultado👾


<img width="66%" alt="Imagem do WhatsApp de 2025-10-03 à(s) 09 32 58_2a1dab3c" src="https://github.com/user-attachments/assets/2e71b565-31fe-486d-b301-72e1b3865c60" />

___________________________________________________________________________________________________________________________________________________________________________________


https://github.com/user-attachments/assets/ab484c86-4815-43b7-a5cd-9c4a0c2dbe9f



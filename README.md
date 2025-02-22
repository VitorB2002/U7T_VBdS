## Embarcando na eletrônica

Esse projeto visa trazer ensinamentos básicos de eletrônica através da prática, utilizando uma Pico W para mostrar conceitos básicos como o funcionamento de portas lógicas e a conversão entre bases decimais e binárias.

## Funcionamento

### Contador

No projeto o contador será controlado através do analógico joystick da placa, a entrada do usuário será interpretada para somar ou subtrair valores do contador, após cada soma ou subtração o usuário será capaz de visualizar o valor. A entrada do joystick será interpretada da seguinte forma:
Caso o usuário mova levemente o joystick para cima, o contador irá somar de 1 em 1 a cada 500ms, se o usuário movimentar o joystick até metade o contador irá somar de 5 em 5 a cada 500ms e ao movimentar o joystick totalmente para cima, o contador irá somar de 10 em 10 a cada 500ms. O mesmo funcionamento ocorre movimentando para baixo, até alcançar o valor mínimo(zero).
Além disso, o usuário ao apertar o botão do joystick poderá trocar a base de conversão do projeto, podendo visualizar em tempo real a conversão de valores para decimal ou hexadecimal.

### Simulador de portas lógicas

No projeto as portas lógicas serão controladas pelos botões A e B da placa, o usuário poderá através do joystick alterar a porta lógica que será simulada.

## Componentes necessários

- 1 Raspeberry Pi Pico W
- 2 switch/push button
- 3 Leds
- 1 display Oled ssd1306

## Pinagem e descrição

| **Nome**      | **GPIO** | **Função**                                  |
|--------------|--------|------------------------------------------|
| vRx         | 26     | Eixo X do joystick (ADC0)               |
| vRy         | 27     | Eixo Y do joystick (ADC1)               |
| SW          | 22     | Botão do joystick                        |
| BTN_A       | 5      | Alterna entre contagem/simulação         |
| BTN_B       | 6      | Zera o contador                          |
| RED_LED     | 13     | Indica resultado 0 na simulação         |
| GREEN_LED   | 11     | Indica resultado 1 na simulação         |
| BLUE_LED    | 12     | Indica o modo atual (contagem/simulação) |
| I2C SDA     | 14     | Comunicação com display OLED            |
| I2C SCL     | 15     | Comunicação com display OLED            |

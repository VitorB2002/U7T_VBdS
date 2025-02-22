## Embarcando na eletrônica

Esse projeto visa trazer ensinamentos básicos de eletrônica através da prática, utilizando uma Pico W para mostrar conceitos básicos como o funcionamento de portas lógicas e a conversão entre bases decimais e binárias.

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

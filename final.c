#include <stdio.h>
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

const int vRx = 26; // Eixo X do joystick (conectado ao ADC)
const int vRy = 27; // Eixo Y do joystick (conectado ao ADC)
const int SW = 22;  // Botão do joystick

int contador = 0; // Variável para armazenar o contador
bool exibir_hex = false; // Estado da exibição (decimal ou hexadecimal)
bool botao_anterior = true; // Estado anterior do botão (para detectar borda de descida)

// Limiares para ajuste da velocidade de alteração do contador
const uint16_t LIMIAR_CIMA_MAX = 4000;
const uint16_t LIMIAR_CIMA_MED = 3000;
const uint16_t LIMIAR_CIMA_MIN = 2500;

const uint16_t LIMIAR_BAIXO_MAX = 100;
const uint16_t LIMIAR_BAIXO_MED = 1000;
const uint16_t LIMIAR_BAIXO_MIN = 1500;

const uint16_t ZONA_MORTA_MIN = 2000;
const uint16_t ZONA_MORTA_MAX = 2800;

// Função para configurar o joystick
void setup_joystick()
{
    adc_init();
    adc_gpio_init(vRx);
    adc_gpio_init(vRy);

    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

// Função de configuração geral
void setup()
{
    stdio_init_all();
    setup_joystick();
}

// Função para ler os valores dos eixos do joystick
void joystick_read_axis(uint16_t *eixo_x, uint16_t *eixo_y)
{
    adc_select_input(0);
    sleep_us(2);
    *eixo_x = adc_read();

    adc_select_input(1);
    sleep_us(2);
    *eixo_y = adc_read();
}

// Função principal
int main()
{
    uint16_t valor_x, valor_y;
    setup();

    printf("Joystick\n");

    while (1)
    {
        joystick_read_axis(&valor_x, &valor_y);

        if (valor_y > LIMIAR_CIMA_MAX)
        {
            contador += 10;
        }
        else if (valor_y > LIMIAR_CIMA_MED)
        {
            contador += 5;
        }
        else if (valor_y > LIMIAR_CIMA_MIN)
        {
            contador += 1;
        }
        else if (valor_y < LIMIAR_BAIXO_MAX)
        {
            contador -= 10;
        }
        else if (valor_y < LIMIAR_BAIXO_MED)
        {
            contador -= 5;
        }
        else if (valor_y < LIMIAR_BAIXO_MIN)
        {
            contador -= 1;
        }

        // Lê o estado atual do botão
        bool botao_atual = !gpio_get(SW); // Invertido porque é pull-up

        // Detecta borda de descida (transição de "solto" para "pressionado")
        if (botao_anterior && !botao_atual)
        {
            exibir_hex = !exibir_hex; // Alterna entre decimal e hexadecimal
        }
        botao_anterior = botao_atual; // Atualiza estado do botão

        // Exibe o contador no formato correspondente
        if (exibir_hex)
        {
            printf("X: %d, Y: %d, Botao: %d, Contador: 0x%X\n", valor_x, valor_y, botao_atual, contador);
        }
        else
        {
            printf("X: %d, Y: %d, Botao: %d, Contador: %d\n", valor_x, valor_y, botao_atual, contador);
        }

        sleep_ms(200);
    }
}

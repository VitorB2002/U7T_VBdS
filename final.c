#include <stdio.h>
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const int BTN_A = 5;
const int BTN_B = 6;

#define I2C_PORT i2c1
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
#define endereco 0x3C

const int vRx = 26;
const int vRy = 27;
const int SW = 22;

const int RED_LED = 13;
const int GREEN_LED = 11;
const int BLUE_LED = 12; // LED azul para indicar o modo de contagem

int contador = 0;
int prev_contador = 0;
bool exibir_hex = false;
bool botao_anterior = true;
bool modo_simulacao = false;
int porta_atual = 0;

const int num_portas = 4;
const char* portas_logicas[] = {"AND", "OR", "NAND", "NOR"};

uint32_t ultima_mudanca = 0;
const uint32_t debounce_delay = 500;

const uint16_t LIMIAR_CIMA_MAX = 4000;
const uint16_t LIMIAR_CIMA_MED = 3000;
const uint16_t LIMIAR_CIMA_MIN = 2500;

const uint16_t LIMIAR_BAIXO_MAX = 100;
const uint16_t LIMIAR_BAIXO_MED = 1000;
const uint16_t LIMIAR_BAIXO_MIN = 1500;

const uint16_t LIMIAR_ESQUERDA = 1000;
const uint16_t LIMIAR_DIREITA = 3000;

void setup_joystick() {
    adc_init();
    adc_gpio_init(vRx);
    adc_gpio_init(vRy);

    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

void setup() {
    stdio_init_all();
    setup_joystick();

    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);

    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_put(RED_LED, 0);

    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_put(GREEN_LED, 0);

    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_put(BLUE_LED, 1); // Inicia com o azul ligado no modo contagem
}

void joystick_read_axis(uint16_t *eixo_x, uint16_t *eixo_y) {
    adc_select_input(0);
    sleep_us(2);
    *eixo_x = adc_read();

    adc_select_input(1);
    sleep_us(2);
    *eixo_y = adc_read();
}

void simular_porta_logica() {
    bool entrada_a = !gpio_get(BTN_A);
    bool entrada_b = !gpio_get(BTN_B);
    bool resultado = 0;

    switch (porta_atual) {
        case 0: resultado = entrada_a && entrada_b; break;
        case 1: resultado = entrada_a || entrada_b; break;
        case 2: resultado = !(entrada_a && entrada_b); break;
        case 3: resultado = !(entrada_a || entrada_b); break;
    }

    gpio_put(RED_LED, resultado == 0);
    gpio_put(GREEN_LED, resultado == 1);
    gpio_put(BLUE_LED, 0); // Azul sempre desligado no modo simulação

    printf("Porta: %s | Entrada A: %d | Entrada B: %d | Saída: %d\n", 
           portas_logicas[porta_atual], entrada_a, entrada_b, resultado);
}

int main() {
    uint16_t valor_x, valor_y;
    setup();

    printf("Joystick\n");

    while (1) {
        joystick_read_axis(&valor_x, &valor_y);

        if (modo_simulacao) {
            bool botao_atual = !gpio_get(SW);
            if (botao_anterior && !botao_atual) {
                modo_simulacao = false;
                gpio_put(RED_LED, 0);
                gpio_put(GREEN_LED, 0);
                gpio_put(BLUE_LED, 1); // Volta a indicar o modo de contagem
            }
            botao_anterior = botao_atual;

            uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

            if (valor_x > LIMIAR_DIREITA && (tempo_atual - ultima_mudanca > debounce_delay)) {
                porta_atual = (porta_atual + 1) % num_portas;
                ultima_mudanca = tempo_atual;
            } else if (valor_x < LIMIAR_ESQUERDA && (tempo_atual - ultima_mudanca > debounce_delay)) {
                porta_atual = (porta_atual - 1 + num_portas) % num_portas;
                ultima_mudanca = tempo_atual;
            }

            simular_porta_logica();
        } else {
            if (!gpio_get(BTN_A)) {
                modo_simulacao = true;
                gpio_put(BLUE_LED, 0); // Desliga o azul ao entrar no modo simulação
            }

            if (valor_y > LIMIAR_CIMA_MAX) contador += 10;
            else if (valor_y > LIMIAR_CIMA_MED) contador += 5;
            else if (valor_y > LIMIAR_CIMA_MIN) contador += 1;
            else if (valor_y < LIMIAR_BAIXO_MAX) contador -= 10;
            else if (valor_y < LIMIAR_BAIXO_MED) contador -= 5;
            else if (valor_y < LIMIAR_BAIXO_MIN) contador -= 1;

            bool botao_atual = !gpio_get(SW);
            if (botao_anterior && !botao_atual) {
                exibir_hex = !exibir_hex;
            }
            botao_anterior = botao_atual;

            if (prev_contador != contador) {
                prev_contador = contador;
                if (exibir_hex) {
                    printf("X: %d, Y: %d, Botao: %d, Contador: 0x%X\n", valor_x, valor_y, botao_atual, contador);
                } else {
                    printf("X: %d, Y: %d, Botao: %d, Contador: %d\n", valor_x, valor_y, botao_atual, contador);
                }
            }

            gpio_put(BLUE_LED, 1); // Mantém o azul ligado no modo contagem
            gpio_put(RED_LED, 0);
            gpio_put(GREEN_LED, 0);

            sleep_ms(200);
        }
    }
}

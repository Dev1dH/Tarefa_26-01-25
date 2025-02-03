/*  ATIVIDADE EMBARCATECH - TAREFA 1 AULA SÍNCRONA 27/01
    Aluno: Devid Henrique Pereira dos Santos
    Matrícula: TIC370100380   
*/

//Bibliotecas
#include "pico/stdlib.h"     // Biblioteca padrão da pico sdk   
#include "ws2812.pio.h"      // Função para habilitar a matriz de LEDs  
#include "hardware/pio.h"    // Biblioteca para programar e controlar o PIO
#include "hardware/timer.h"  // Biblioteca para habilitar timer do hardware
#include "number.h"          // Biblioteca para desenhar cada número na matrix de LED

#define FLAG false      
#define NUM_PIXELS 25   // Define o número de pixels da matriz de LEDs

// Define as pinagens
#define LED_MATRIX 7    // Define a pinagem da matriz de LEDs
#define LED_RED 13      // Define a pinagem do LED vermelho
#define BUTTON_A 5      // Define a pinagem do botão A
#define BUTTON_B 6      // Define a pinagem do botão B

// Variável global que guarda o valor do clique nos botões
static int clique = 0;

bool led_buffer[NUM_PIXELS]; // Buffer de LEDs
bool button_pressed = false; // Flag para indicar que um botão foi pressionado

// Protótipo das funções
void init();                                                        // Função para inicializar gpios 
void case_op();                                                     // Função para escolher todas as opções da matriz de LEDs
void gpio_irq_handler(uint gpio, uint32_t events);                  // Função de interrupção
void copy_array(bool *dest, const bool *src);                       // Função copiar os arrays
void set_one_led(uint8_t r, uint8_t g, uint8_t b);                  // Função para definir a cor dos LEDs na matriz
static inline void put_pixel(uint32_t pixel_grb);                   // Função para enviar um pixel para o buffer
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);   // Função para converter um pixel para um inteiro

// Função principal
int main()
{
    
    init();
    stdio_init_all();

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicialização do buffer de LEDs com o número 0
    copy_array(led_buffer, matrix_0);
    set_one_led(30, 0, 0);

    while (true) {
        if (button_pressed) { // Verifica se um botão foi pressionado
            set_one_led(30, 0, 0); // Atualiza o valor do LED com base no contador
        } else {
            button_pressed = false; // Atualiza a flag para indicar que nenhum botão foi pressionado
        }

        gpio_put(LED_RED, true);
        sleep_ms(100);
        gpio_put(LED_RED, false);
        sleep_ms(100);
    }
}

// Função para inicializar todas as gpios
void init(){
stdio_init_all();
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    
    // Configuração do pino do LED vermelho
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    // Configuração do pinos do botão A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Configuração do pinos do botão B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Inicialização do programa do WS2812
    ws2812_program_init(pio, sm, offset, LED_MATRIX, 800000, FLAG);

    // Inicialização do buffer de LEDs com o número 0
    copy_array(led_buffer, matrix_0);
    set_one_led(30, 0, 0);
}

// Função para escolher todas as opções da matriz de LEDs
void case_op(){

switch (clique) {
        case 0: copy_array(led_buffer, matrix_0); break;
        case 1: copy_array(led_buffer, matrix_1); break;
        case 2: copy_array(led_buffer, matrix_2); break;
        case 3: copy_array(led_buffer, matrix_3); break;
        case 4: copy_array(led_buffer, matrix_4); break;
        case 5: copy_array(led_buffer, matrix_5); break;
        case 6: copy_array(led_buffer, matrix_6); break;
        case 7: copy_array(led_buffer, matrix_7); break;
        case 8: copy_array(led_buffer, matrix_8); break;
        case 9: copy_array(led_buffer, matrix_9); break;
    }
}

// Função de interrupção com debounce
void gpio_irq_handler(uint gpio, uint32_t events)
{
    volatile static uint32_t last_time = 0; // Último tempo que um botão foi pressionado
    volatile uint32_t current_time = to_ms_since_boot(get_absolute_time()); // Tempo atual

    // Verifica se o botão foi pressionado muito rapidamente
    if (current_time - last_time < 400) {
        return;
    }

    last_time = current_time; // Atualiza o tempo do último botão pressionado

    if (gpio == BUTTON_A) { // Verifica se o botão A foi pressionado
        clique++; // Incrementa o contador   
    } else if (gpio == BUTTON_B) { // Verifica se o botão B foi pressionado
        clique--; // Decrementa o contador   
    }

    // Define o buffer de LEDs com base no contador
    case_op();

    button_pressed = true; // Atualiza a flag para indicar que um botão foi pressionado
}

//Função copiar os arrays
void copy_array(bool *dest, const bool *src)
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        dest[i] = src[i];
    }
}

// Função para enviar um pixel para o buffer
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para converter um pixel para um inteiro
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para definir a cor dos LEDs na matriz
void set_one_led(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_buffer[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

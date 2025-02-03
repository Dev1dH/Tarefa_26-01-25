/*  ATIVIDADE EMBARCATECH - TAREFA 1 AULA SÍNCRONA 27/01
    Aluno: Devid Henrique Pereira dos Santos
    Matrícula: TIC370100380   
*/

//Bibliotecas
#include <stdio.h>            // Biblioteca padrão linguagem C
#include "pico/stdlib.h"      // Biblioteca padrão pico
#include "hardware/pio.h"     // Biblioteca para programar e controlar o PIO
#include "ws2818b.pio.h"      // Biblioteca gerada pelo arquivo .pio durante compilação.

// Definição do número de LEDs e pino.
#define ledR_pin  13   // Pinagem do LED vermelho
#define button_0  5    // Pinagem do botão A
#define button_1 6     // Pinagem do botão B
#define LED_COUNT 25   // Número de pixels
#define LED_PIN 7      // Pinagem da matriz de LEDs

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

// Inicializa os elementos
void init_elements();

// Case de opções
void case_op(int clique);

// Funções para configurar os LEDs
void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();
int getIndex(int x, int y);

// Funções para desenhar dígitos de 0-9 na matriz de LEDs
void matrix_0();
void matrix_1();
void matrix_2();
void matrix_3();
void matrix_4();
void matrix_5();
void matrix_6();
void matrix_7();
void matrix_8();
void matrix_9();

// Função de interrupção
void gpio_irq_handler(uint gpio, uint32_t events);

// Função de debounce(SERÁ ADICIONADA POSTERIORMENTE)

int main() {
  
  int clique = 0;                   // Variável para contar quantar número de cliques
  bool last_buttonA_state = true;   // Variável para guardar o estado do botão A
  bool last_buttonB_state = true;   // Variável para guardar o estado do botão B
  
  init_elements();
    
  // Configuração da interrupção com callback
  gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  
  while (true)
  { 
    
    bool buttonA_state = gpio_get(button_0);
    bool buttonB_state = gpio_get(button_1);

    if(buttonA_state == false && last_buttonA_state == true)
    {
     clique++;  // Incrementa a variável clique
    
     case_op(clique);
    }

    else if(buttonB_state == false && last_buttonB_state == true)
    {
    clique--;  // Decrementa a variável clique
    
    case_op(clique);
    }
    
    last_buttonA_state = buttonA_state;
    last_buttonB_state = buttonB_state;
    
    // Pisca o LED 5 vezes por segundo 
    gpio_put(ledR_pin, true);
    sleep_ms(100);
    gpio_put(ledR_pin, false);
    sleep_ms(100);
    
    }    
}    
  
// Função para inicializar as entradas e saídas. 
void init_elements(){
  stdio_init_all();
  gpio_init(ledR_pin);              // Inicializa o LED vermelho
  gpio_set_dir(ledR_pin, GPIO_OUT); // Define o LED vermelho como saída
  gpio_init(button_0);              // Inicializa o botão A
  gpio_set_dir(button_0, GPIO_IN);  // Define o botão A como uma entrada
  gpio_pull_up(button_0);           // Pull up do botão A
  gpio_init(button_1);              // Inicializa o botão B
  gpio_set_dir(button_1, GPIO_IN);  // Define o botão B como uma entrada
  gpio_pull_up(button_1);           // Pull up do botão B
  npInit(LED_PIN); // Inicializa matriz de LEDs NeoPixel.
}

// Função com o case de opções
void case_op(int clique){

switch (clique)
      {
      case 1:
            matrix_0();
      break;
      case 2:
            matrix_1();
      break;
      case 3:
            matrix_2();
      break;
      case 4:
            matrix_3();
      break;
      case 5:
            matrix_4();
      break;
      case 6:
            matrix_5();
      break;
      case 7:
            matrix_6();
      break;
      case 8:
            matrix_7();
      break;
      case 9:
            matrix_8();
      break;
      case 10:
            matrix_9();
      break;
      } 
}

// Matriz de LEDs para o número 0
void matrix_0(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 1
void matrix_1(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 2
void matrix_2(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 3
void matrix_3(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 4
void matrix_4(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 5
void matrix_5(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 6
void matrix_6(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 7
void matrix_7(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 8
void matrix_8(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Matriz de LEDs para o número 9
void matrix_9(){
int matriz[5][5][3] = {
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},   
    {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}, 
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}
};
  
  for(int linha = 0; linha < 5; linha++){
  for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
  npWrite();
  sleep_ms(2000);
}

// Inicializa a máquina PIO para controle da matriz de LEDs.
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

// Atribui uma cor RGB a um LED da matriz.
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

// Limpa o buffer de pixels.
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

// Escreve os dados do buffer nos LEDs.
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Converte as coordenadas (x, y) na matriz 5x5 para o índice da fila linear.
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

// Função para interrupção
void gpio_irq_handler(uint gpio, uint32_t events)
{
  sleep_ms(50);
  gpio_put(LED_PIN, !gpio_get(LED_PIN)); // Alterna o estado
}

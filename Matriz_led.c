#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "hardware/clocks.h"

#include "ws2818b.pio.h"

#include "matrizLED.h"

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs WS2818b.
 * 
 * @param pin Pino GPIO usado para controlar os LEDs WS2818b.
 * 
 * Este processo configura a máquina PIO e carrega o programa necessário 
 * para controlar a matriz de LEDs WS2818b. O PIO (Programmable I/O) é utilizado 
 * para gerar sinais de controle com precisão para os LEDs.
 */
void npInit(uint pin) {

  // Adiciona o programa PIO específico para o controle dos LEDs WS2818b no PIO0.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0; // Definindo que o PIO utilizado será o PIO0.

  // Reivindica uma máquina de estados (SM) do PIO0 para usar no controle dos LEDs.
  sm = pio_claim_unused_sm(np_pio, false); // Tenta obter uma máquina de estados não utilizada.
  if (sm < 0) {
    // Se não houver máquinas de estados livres no PIO0, tenta usar o PIO1.
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicializa o programa WS2818b na máquina de estados (SM) obtida no PIO selecionado.
  // O parâmetro '800000.f' refere-se à taxa de atualização (em Hz) dos LEDs WS2818b.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Inicializa a matriz de LEDs, passando o PIO e a máquina de estados como parâmetros.
  npMatrizInit(np_pio, sm);
}

int main()
{
    stdio_init_all();

    // Inicializa matriz de LEDs.
    npInit(LED_PIN);
    npClear(); // limpa a matriz
    npWrite(); // Escreve os dados nos LEDs (reset neste caso)


    while (true) {

      /*Exemplo 1: semáforo*/
        // sleep_ms(1000);
        // npClear();
        // for(int i = 0; i<5;i++)
        //     setLEDxy(3,i,255,0,0);
        // npWrite(); // Escreve os dados nos LEDs.
        // sleep_ms(1000);
        // npClear();
        // for(int i = 0; i<5;i++)
        //     setLEDxy(1,i,0,255,0);
        // npWrite(); // Escreve os dados nos LEDs.
        // sleep_ms(1000);
        // npClear();
        // for(int i = 0; i<5;i++)
        //     setLEDxy(2,i,255,255,0);
        // npWrite(); // Escreve os dados nos LEDs.
        // sleep_ms(1000);

        /*Exemplo 2: Contador de 9s com reset automático*/
        static int i = 9;
        setLEDnumber(i,100,0,0);
        if(i>0)
          i--;
        else
          i=9;
        sleep_ms(1000);
    }
}
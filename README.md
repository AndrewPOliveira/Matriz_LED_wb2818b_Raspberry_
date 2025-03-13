# Biblioteca de Controle da Matriz de LEDs - BitDogLab

Este projeto visa fornecer uma biblioteca simples e eficiente para controle da matriz de LEDs 5x5 disponível na placa **BitDogLab**. A biblioteca abstrai os detalhes da comunicação com a matriz de LEDs, permitindo ao usuário focar na criação de animações e no controle de LEDs individuais de maneira prática.

## Funcionalidades

- **Controle de LEDs Individuais**: A biblioteca permite controlar cada LED da matriz individualmente, definindo suas cores no formato RGB.
- **Controle de Padrões Numéricos**: Permite exibir números de 0 a 9 na matriz utilizando um template predefinido.
- **Função de Limpeza**: É possível apagar todos os LEDs da matriz com um simples comando.
- **Simples Interface de Programação**: A API foi projetada para ser fácil de usar, mesmo para iniciantes.

## Estrutura do Projeto

O projeto é dividido em três partes principais:

1. **`matrizLED.h`**: Define as funções de inicialização, controle e manipulação da matriz de LEDs.
2. **`matrizLED.c`**: Implementa as funções de controle, como definir LEDs, limpar a matriz e exibir números.
3. **`ws2818b.pio`**: Programa em PIO responsável pela comunicação com a matriz de LEDs (compatível com a biblioteca PIO do RP2040).

## Como Usar

### 1. Inicialização

A primeira etapa é inicializar a matriz de LEDs, configurando o pino de controle e a máquina PIO:

```c
npInit(LED_PIN);
```

### 2. Definir LEDs Individuais

Para definir a cor de um LED na posição específica, use a função `setLEDxy`:

```c
setLEDxy(x, y, r, g, b);
```

Onde `x` e `y` representam as coordenadas (0-4) da matriz e `r`, `g`, `b` são os valores de cor em formato RGB (0-255).

### 3. Exibir Números

A função `setLEDnumber` permite exibir números de 0 a 9 na matriz, com a seguinte sintaxe:

```c
setLEDnumber(number, r, g, b);
```

### 4. Limpar a Matriz

Para limpar todos os LEDs, basta chamar a função `npClear()`:

```c
npClear();
```

### 5. Atualizar a Matriz

Após modificar o estado dos LEDs, use `npWrite()` para atualizar a matriz com as novas configurações:

```c
npWrite();
```

## Dependências

- **Pico SDK**: A biblioteca foi desenvolvida para ser utilizada com o Raspberry Pi Pico, utilizando o SDK oficial da Raspberry Pi.
- **PIO (Programmable Input/Output)**: Utiliza o PIO do RP2040 para enviar dados de controle à matriz de LEDs.

## Exemplo Completo

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "hardware/clocks.h"

#include "ws2818b.pio.h"
#include "matrizLED.h"

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

  npMatrizInit(np_pio, sm);
}
int main() {
    stdio_init_all();

    // Inicializa a matriz de LEDs no pino 7
    npInit(LED_PIN);

    // Define as cores dos LEDs
    npClear(); //Limpa o buffer da matriz
    setLEDxy(0, 0, 255, 0, 0);  // LED na posição (0,0) em vermelho
    setLEDxy(1, 1, 0, 255, 0);  // LED na posição (1,1) em verde
    npWrite(); //Atualiza a matriz

    // Exibe o número 5 na matriz em azul automaticamente
    setLEDnumber(5, 0, 0, 255);

    while (1) {
        sleep_ms(1000);
    }
}
```

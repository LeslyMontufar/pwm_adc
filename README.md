# Controle do brilho do led usando porta ADC ![Language grade: C](https://img.shields.io/badge/language-C-blue)

Uso de conversor AD para mudar intensidade do led conforme a resistência de um potenciômetro.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Utilização de um ADC para a conversão de um único canal mas com o emprego de interrupção. O início da transferência será comandado pelo usuário, através de uma chamada a HAL_ADC_Start_IT(). A temporização da chamada também ficará a cargo do usuário, através do uso de um timer. Apenas para visualização, iremos condicionar o brilho dos LEDs da placa de desenvolvimento a valores lidos pelo ADC.

#### 1. No projeto STM32CubeIDE, configure-o da seguinte forma:
* Configure um temporizador como base de tempo de amostragem, de forma que ele gere
uma interrupção (use prioridade 3) com uma taxa de 10Hz. Isso irá garantir uma amostra
a cada 100ms.
* Configure o pino PB11 (era PC4) como canal para o ADC1. Use um tempo de amostragem de 28
ciclos. Como queremos apenas uma conversão por interrupção, o modo de scan deve estar
desligado, o modo contínuo desligado, requisição contínua de DMA desligada também. O
fim da conversão será indicado pela leitura de apenas um canal. Não configure nenhum
DMA.
* Habilite a interrupção do ADC1, usando dois como prioridade.
* Como feito no laboratório anterior, use o PWM do canal 1 do TIM1 (pino PA8), realizando
o jumper do PC13 para o PA8 e deixando o PC13 flutuante. Assim será possível ver o
brilho do led alterar de acordo com o PWM. Configure o TIM1 com base de tempo igual
a 1kHz com o canal de PWM com ciclo útil igual a zero.
 
#### 2. No código gerado:
* Inicialize timer e o canal PWM.
* Inicialize o temporizador de amostragem no modo de interrupção.
* Dentro do callback do timer usado (HAL_TIM_PeriodElapsedCallback), crie uma
chamada para início de conversão via interrupção com HAL_ADC_Start_IT().
* No callback de fim de conversão (HAL_ADC_ConvCpltCallback), use a função
HAL_ADC_GetValue() para obter o valor do ADC e configurar o canal de PWM de forma
proporcional (use as macros __HAL_TIM_SET_COMPARE e
__HAL_TIM_GET_COMPARE).
 
#### 3. Circuito na protoboard:
* Ligue um potenciômetro de até 10k Ohms no canal do ADC e observe se o programa está
funcionando adequadamente. Caso não tenha um potenciômetro, tente ver o funcionamento
jumpeando para terra e VDD.

## Desenvolvimento

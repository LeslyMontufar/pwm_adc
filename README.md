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
1. Configurei o led para iniciar apagado, pela configuração do duty cycle do pwm, que define a corrente disponível para o led.
```
void app_init(void){
	app_started = true;
	app_led_fade_percent(percent_fade);
	hw_timer_adc_start();
	hw_pwm_start();
}
```
```
void app_led_fade_percent(uint32_t percent){
	hw_set_duty(100-percent);
}
```
```
void hw_set_duty(uint16_t duty) {
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1)+1;
	uint16_t CCR = duty*arr/100;
	__HAL_TIM_SET_COMPARE(&htim1,PWM_CHN1, CCR-1*(CCR>0));
}
```
2. Com o timer do adc ligado e o timer do pwm iniciado, o led muda o estado de acordo com as amostars coletadas pelo adc. A interrupção do adc é iniciada a cada vez que COUNTER == ARR, na função `HAL_TIM_PeriodElapsedCallback`.
```
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim1) {
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
	else if(htim == &htim2)	{
		HAL_ADC_Start_IT(&hadc1);
		__HAL_TIM_SET_COUNTER(&htim2, 0);
	}
}
```
3. Quando o adc já possui o valor digital da amostra na memória, coleto esse valor.
```
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	tmp = HAL_ADC_GetValue(hadc);
	app_adc_it(tmp);
}
```
4. Uso para alterar o duty cycle do pwm e assim mudar a intensidade do led, a partir da corrente que recebe. Como o led PC13 internamente é uma saída do tipo push pull, então possui máximo brilho quando aterrado e mínimo quando recebe máxima tensão (3V3).
```
void app_adc_it(uint32_t tmp){
	percent_fade = (tmp*100)/4095;
	app_led_fade_percent(percent_fade);
}

void app_led_fade_percent(uint32_t percent){
	hw_set_duty(100-percent);
}
```

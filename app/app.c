/*
 * Lesly Montúfar
 * app.c
 *
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "app.h"
#include "hw.h"

volatile uint32_t percent_fade = 0;
bool app_started = false;

void app_adc_it(uint32_t tmp){
	percent_fade = tmp;
	app_led_fade_percent(percent_fade);
}

void app_led_fade_percent(uint32_t percent){
	hw_set_duty(100-percent); // o led fica em nível alto em PA8 = 0;
}

void app_led_off(void){
	percent_fade = 0;
	app_led_fade_percent(percent_fade);
}

//void app_button_interrupt(void){
//	if(!app_started)
//		return;
//
//	percent_fade += 10;
//	if(percent_fade==110)
//		percent_fade = 0;
//	app_led_fade_percent(percent_fade);
//}

void app_init(void){
	app_started = true;
	app_led_fade_percent(percent_fade);
	hw_timer_adc_start();
	hw_pwm_start();
}

void app_loop(void){
//	hw_cpu_sleep();
}

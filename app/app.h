/*
 * app.h
 *
 *  Created on: July 21, 2022
 *      Author: lesly
 */

#ifndef APP_H_
#define APP_H_

void app_apply_percent_fade(uint32_t tmp);
void app_led_fade_percent(uint32_t percent);
//void app_led_off(void);
//void app_button_interrupt(void);
void app_init(void);
void app_loop(void);

#endif /* APP_H_ */

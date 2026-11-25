// led-task.h — задача светодиода: выключен, горит или мигает
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    LED_STATE_OFF,
    LED_STATE_ON,
    LED_STATE_BLINK,
} led_state_t; //перечисляемый тип. Даёт имена набору целых значений

void led_task_init(void);
void led_task_handle(void);

void led_task_set_state(led_state_t state);
led_state_t led_task_get_state(void);

// false — период не принят
bool led_task_set_period_ms(uint32_t period_ms);


//Заголовок задачи разобран в т2.3.5. Возьмите его и добавьте функцию uint32_t led_task_get_period_ms(void).
uint32_t led_task_get_period_ms(void).


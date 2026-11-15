#include "led.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25;

static bool led_state = false;

void led_init(void)
{
    // инициализируем пин светодиода и настраиваем его на выход
    gpio_init(LED_PIN); //Инициализируем. Но не обязательно всё иниц. из того,что есть на плате
    gpio_set_dir(LED_PIN, GPIO_OUT);//25 пин работает на выход  (аут): подает ток - светодиол мигает
    led_set(false);
}

void led_set(bool on)
{
    led_state = on;
    gpio_put(LED_PIN, led_state);
}

void led_toggle(void)
{
    led_set(!led_state);
}

bool led_is_on(void)
{
    return led_state;
}

uint led_pin(void)
{
    return LED_PIN;
}


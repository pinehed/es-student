#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25; //создаем константу типа uint 

int main()
{
    // весь дальнейший код пишем здесь
    gpio_init(LED_PIN); //Инициализируем. Но не обязательно всё иниц. из того,что есть на плате
    gpio_set_dir(LED_PIN, GPIO_OUT);//25 пин работает на выход  (аут): подает ток - светодиол мигает
    while (1)
    {
        // код мигания пишем здесь
        gpio_put(LED_PIN, 1);//gpio_put - подаем напряжение. вкл - подаём 1 на светодиод
        sleep_ms(250); //ждём
        gpio_put(LED_PIN, 0); //выкл - подаём 0 на светодиод
        sleep_ms(1000);
    }
}

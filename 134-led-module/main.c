
// добавляем заголовочный файл функций ввода-вывода
#include "pico/stdlib.h"
// добавляем заголовочный файл функций работы с GPIO
#include "hardware/gpio.h"

#include <stdio.h>

#include "led.h"
#include "log.h"
#include "device.h"


const uint BUTTON_PIN = 15;  
const uint DEBOUNCE_MS = 20; //константа задержки


bool get_button_debounce(uint pin)
{
    // читаем вывод устойчиво к дребезгу
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'd')
    {
        led_set(false);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'v')
    {
        log_version(); //печатает строку версии. log 
    }
        else if (command == 'i')
    {
        device_info(); // <~ device
    }
    else
    {
        LOG_ERR("unknown command: %c\n", command);
    }

    
}

int main()
{
    // включаем стандартный ввод-вывод
    stdio_init_all();
    led_init();
  

    // инициализируем и настраиваем вывод кнопки
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);//`GPIO_IN`:микроконтроллер будет напряжение считывать, а не выдавать.
    gpio_pull_up(BUTTON_PIN); // `gpio_pull_up()` включает подтягивающий резистор, из-за которого отпущенная кнопка читается как `true`.

    // объявляем переменные
    bool led = false; // Светит ли светодиод. 
    bool previous = false;//что вывод показал на прошлом шаге цикла

    while (1)
    {
        // читаем состояние пина кнопки с задержкой
         bool current = get_button_debounce(BUTTON_PIN);      

        if (previous == true && current == false)
        {
            led_toggle();
            LOG_INF("led %s\n", led_is_on() ? "on" : "off");
        }

        // запоминаем текущее состояние пина кнопки, как предыдущее
        previous = current;
        
        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        LOG_DBG("got %c\n", command);
        handle_command(command);
    }
}
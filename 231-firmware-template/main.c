
// добавляем заголовочный файл функций ввода-вывода
#include "pico/stdlib.h"

#include <stdio.h>
#include <string.h>

//свои модули
#include "led.h"
#include "log.h"
#include "device.h"
#include "command.h"
#include "profiling.h"



//Мигать по часам  (2_2_2)
const uint BLINK_HALF_PERIOD_MS = 500; //константа полупериода, миллисек.
uint64_t last_toggle_us = 0;//метка прошлого переключения
void blink(void)
{
    uint64_t now_us = time_us_64();

    if (now_us - last_toggle_us >= BLINK_HALF_PERIOD_MS * 1000) //Полупериод задан в миллисекундах, а часы идут в микросекундах. Поэтому константа умножается на 1000.
    {
        last_toggle_us = now_us;
        led_toggle();
    }
}




int main()
{
    // инициализируем стандартный ввод-вывод и светодиод
    led_init();
     // запоминаем начальное показание часов для профилирования
     profiling_init();


    stdio_text_protocol_init();
    while (1)
    {   // учитываем прошлую итерацию
        profiling_iteration();
        // мигаем светодиодом
        blink();

        // принятая строка становится командой, команда — действием прибора
        const command_t *command = stdio_text_protocol_handle();
        if (command != NULL)
        {
            api_handle(command);
        }

        // принимаем команды
        read_line();
    }
}

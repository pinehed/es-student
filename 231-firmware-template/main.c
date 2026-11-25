
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



int main()
{
    // инициализируем стандартный ввод-вывод и светодиод
    led_init();
     // запоминаем начальное показание часов для профилирования
     profiling_init();

    stdio_text_protocol_init();
    led_task_init();

    while (1)
    {   // учитываем прошлую итерацию
        profiling_iteration();

        led_task_handle();

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

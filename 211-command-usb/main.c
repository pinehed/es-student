
// добавляем заголовочный файл функций ввода-вывода
#include "pico/stdlib.h"
// добавляем заголовочный файл функций работы с GPIO
#include "hardware/gpio.h"

#include <stdio.h>
#include <string.h>

//свои модули
#include "led.h"
#include "log.h"
#include "device.h"
#include "memory.h"
#include "command.h"



const uint BUTTON_PIN = 15;  
const uint DEBOUNCE_MS = 20; //константа задержки

//буфер строки
#define LINE_SIZE 32
char line[LINE_SIZE];
uint line_length = 0;

bool get_button_debounce(uint pin)
{
    // читаем вывод устойчиво к дребезгу
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

//опишем тип обработчика
typedef void (*command_handler_t)(void);

void cmd_enable(void)
{
    // включаем светодиод и сообщаем новое состояние
    led_set(true);
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_disable(void)
{
    // выключаем светодиод и сообщаем новое состояние
    led_set(false);
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_info(void)
{
    // печатаем паспорт устройства
    device_info();
    
}

void cmd_version(void)
{
    // печатаем строку журнала о версии прошивки
    log_version(); 
}

void cmd_ping(void)
{
    printf("pong\n");
}

void cmd_mem_info(void)
{
    // печатаем строку журнала о версии прошивки
    mem_info(); 
}

extern void fw_info(void);

void cmd_fw_info(void)
{
    // 2_1_4 память программы
   fw_info(); 
}
//Массив - таблица команд

void cmd_dev_info(void)
{
    // 2_1_4 память программы
   dev_info(); 
}


const struct command_t commands[] = {
    { "enable", cmd_enable },
    { "disable", cmd_disable },
    { "info", cmd_info },
    { "version", cmd_version },
    { "ping", cmd_ping },
    { "mem_info", cmd_mem_info },
    { "fw_info", cmd_fw_info },
    { "dev_info", cmd_dev_info }
};

//command_count  - cчитаем длину 
const uint command_count = sizeof(commands) / sizeof(commands[0]);

void handle_command(const char *command)
{
    for (uint i = 0; i < command_count; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        {
            if (commands[i].handler != NULL) //важно! Что бы не было: ядро передаст управление в начало Boot ROM (адрес )
            {
                commands[i].handler();//- вызов по указателю: скобки после указателя означают «передать управление по этому адресу». 
            }

            return;
        }
    }

    LOG_ERR("unknown command: %s\n", command);
}

//функция: сбор строки из символов
void read_line(void)
{
    int symbol = getchar_timeout_us(0);

    if (symbol == PICO_ERROR_TIMEOUT)
    {
        return;
    }

    if (symbol == '\r' || symbol == '\n')
    {
        putchar('\n');
        line[line_length] = '\0';

        if (line_length > 0)
        {
            LOG_DBG("got %s\n", line);
            handle_command(line);
        }

        line_length = 0;
        return;
    }

    if (line_length + 1 < LINE_SIZE)
    {
        line[line_length] = (char)symbol;
        line_length = line_length + 1;
        putchar(symbol);
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
        
        read_line();
    }
}
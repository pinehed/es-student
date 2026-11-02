// добавляем заголовочный файл функций ввода-вывода
#include "pico/stdlib.h"
// добавляем заголовочный файл функций работы с GPIO
#include "hardware/gpio.h"

#include <stdio.h>



// объявляем константы


const uint LED_PIN = 25; //создаем константу ВЫВЫДА?! типа uint 
const uint BUTTON_PIN = 15;  // объявляем константу вывода светодиода
const uint DEBOUNCE_MS = 20; //константа задержки

//функцию, которая читает вывод устойчиво: 
//«уровень считается высоким, только если он высок сейчас и всё ещё высок через двадцать миллисекунд». В любом другом случае функция вернёт `false`.

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

void set_led(bool on)
{
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}

//Функция получает пришедший символ и текущее состояние светодиода, а возвращает новое. Если команда была — изменённое, если нет — то же самое. 
bool handle_command(int command, bool led)
{
    if (command == 'e')
    {
        led = true;
        set_led(led);
    }
    else if (command == 'd')
    {
        led = false;
        set_led(led);
    }
    else
    {
        printf("unknown command: %c\n", command);
    }

    return led;
}



int main()
{   // включаем стандартный ввод-вывод
    stdio_init_all();

    // инициализируем пин светодиода
    // настраиваем пин светодиода на выход
    gpio_init(LED_PIN); //Инициализируем. Но не обязательно всё иниц. из того,что есть на плате
    gpio_set_dir(LED_PIN, GPIO_OUT);//25 пин работает на выход  (аут): подает ток - светодиол мигает
    
      // инициализируем и настраиваем пин кнопки
     // подтягиваем кнопку к питанию
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);//`GPIO_IN`:микроконтроллер будет напряжение считывать, а не выдавать.
    gpio_pull_up(BUTTON_PIN); // `gpio_pull_up()` включает подтягивающий резистор, из-за которого отпущенная кнопка читается как `true`.


    
    //задать 2переменных типа булевая (Да\нет).
    bool led = false; // Светит ли светодиод. 
    bool previous = false;//что вывод показал на прошлом шаге цикла
    
    while (1)
    {  // читаем состояние пина кнопки с задержкой
        bool current = get_button_debounce(BUTTON_PIN);      // читаем состояние пина кнопки с задержкой
        
        // если состояние сменилось — переключаем светодиод вызовом set_led
        if (previous == true && current == false)
        {
            led = !led;
            set_led(led);

        }
        
        // запоминаем текущее состояние пина кнопки как предыдущее.
        // Стоит здесь - перед command. Иначе кнопка не работает, а только терминал
        previous = current;

        //прочесть символ - команду с терминала:
        int command = getchar_timeout_us(0); //Ноль в аргументе функции означает не ждать вовсе. Если символ пришёл, функция возвращает его код, если нет — значение `PICO_ERROR_TIMEOUT`
        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        led = handle_command(command, led);
 



    }
    
}

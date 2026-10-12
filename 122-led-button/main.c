// добавляем заголовочный файл функций ввода-вывода
#include "pico/stdlib.h"
// добавляем заголовочный файл функций работы с GPIO
#include "hardware/gpio.h"



const uint LED_PIN = 25; //создаем константу ВЫВЫДА?! типа uint 
const uint BUTTON_PIN = 15;  // объявляем константу вывода светодиода

int main()
{   // инициализируем пин светодиода
    // настраиваем пин светодиода на выход
    gpio_init(LED_PIN); //Инициализируем. Но не обязательно всё иниц. из того,что есть на плате
    gpio_set_dir(LED_PIN, GPIO_OUT);//25 пин работает на выход  (аут): подает ток - светодиол мигает
    
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);//`GPIO_IN`:микроконтроллер будет напряжение считывать, а не выдавать.
    gpio_pull_up(BUTTON_PIN); // `gpio_pull_up()` включает подтягивающий резистор, из-за которого отпущенная кнопка читается как `true`.


    //два указателя на регистры и маску вывода светодиода:
    //volatile - для компилятора: за этим адресом не обычная переменная, а регистр устройства.
    volatile uint32_t *gpio_out_set = (uint32_t *)(SIO_BASE + SIO_GPIO_OUT_SET_OFFSET);
    volatile uint32_t *gpio_out_clr = (uint32_t *)(SIO_BASE + SIO_GPIO_OUT_CLR_OFFSET);
    //Маска `1u << LED_PIN` ставит единицу в тот разряд, который отвечает за вывод `25`, и нули во все остальные: именно так регистр понимает, о каком выводе идёт речь.
    const uint32_t led_mask = 1u << LED_PIN;

    //задать 2переменных типа булевая (Да\нет).
    bool led = false; // Светит ли светодиод. 
    bool previous = false;//что вывод показал на прошлом шаге цикла
    
    while (1)
    {
        bool current = gpio_get(BUTTON_PIN);

        if (previous == true && current == false)
        {
            led = !led;
            gpio_put(LED_PIN, led);
        }

        previous = current;
    }
    
}

#include "pico/stdlib.h"
#include "hardware/gpio.h"
// подключаем 2 загоовочных файла с адресами 
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"


const uint LED_PIN = 25; //создаем константу типа uint 

int main()
{
    // весь дальнейший код пишем здесь
    gpio_init(LED_PIN); //Инициализируем. Но не обязательно всё иниц. из того,что есть на плате
    gpio_set_dir(LED_PIN, GPIO_OUT);//25 пин работает на выход  (аут): подает ток - светодиол мигает
    
    //два указателя на регистры и маску вывода светодиода:
    //volatile - для компилятора: за этим адресом не обычная переменная, а регистр устройства.
    volatile uint32_t *gpio_out_set = (uint32_t *)(SIO_BASE + SIO_GPIO_OUT_SET_OFFSET);
    volatile uint32_t *gpio_out_clr = (uint32_t *)(SIO_BASE + SIO_GPIO_OUT_CLR_OFFSET);
    //Маска `1u << LED_PIN` ставит единицу в тот разряд, который отвечает за вывод `25`, и нули во все остальные: именно так регистр понимает, о каком выводе идёт речь.
    const uint32_t led_mask = 1u << LED_PIN;
    
    
    while (1)
    {
        // код мигания 
        *gpio_out_set = led_mask;
        sleep_ms(250);
        *gpio_out_clr = led_mask;
        sleep_ms(1000);

     
    }
}

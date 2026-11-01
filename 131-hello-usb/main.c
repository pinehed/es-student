

#include <stdio.h>
#include "pico/stdlib.h" //инициализирует функции



int main()
{   stdio_init_all(); //Включите канал до первого обращения к нему:
//инициализируем железо, отвечающее за отправку данных везде (находится в "черной коробочке")
    
 
    stdio_init_all();

    while (1)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }

}  
    

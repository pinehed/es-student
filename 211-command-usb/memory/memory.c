#include <stdio.h> // ради printf(),
#include <stdint.h>  // ради типа uintptr_t
#include "pico/stdlib.h" //размер флеш памяти конкретной платы.
#include "hardware/regs/addressmap.h" //базовых адресов флеш, ОЗУ и ПЗУ

//Обьявляем символы линкера
//каждое как внешнюю переменную и берите адрес, а не значение:
extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;


// memory.c

static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void)
{
    // шапка таблицы: область, начало, конец, размер
    printf("%-15s %-15s %-15s %-15s\n",
            "area","start","end","size");

    // flash — XIP_BASE и PICO_FLASH_SIZE_BYTES
    row("flash", (uintptr_t)XIP_BASE, (uintptr_t)(XIP_BASE+PICO_FLASH_SIZE_BYTES));
    // sram — базовый адрес из SDK, размер из datasheet
    row("sram", (uintptr_t)SRAM_BASE, (uintptr_t)SRAM_END);
    // rom — базовый адрес из SDK, размер из datasheet
    row("row", (uintptr_t)ROM_BASE, (uintptr_t)(ROM_BASE+0x4000));

    // image — от __flash_binary_start до __flash_binary_end
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    // free  — от __flash_binary_end до конца флеш-памяти
    row("free", (uintptr_t)&__flash_binary_end, (uintptr_t)(XIP_BASE+PICO_FLASH_SIZE_BYTES)); //!!!
    // boot2 — от __boot2_start__ до __boot2_end__
    row("boot2", (uintptr_t)& __boot2_start__, (uintptr_t)&__boot2_end__);
    // text  — от __boot2_end__ до __etext: код и константы
    row("text", (uintptr_t)&__boot2_end__ , (uintptr_t)&__etext);

    // data flash — хранение .data, от __etext, длиной с .data
    // data ram   — работа .data, от __data_start__ до __data_end__
    row("data ram", (uintptr_t)&__data_start__,(uintptr_t)&__data_end__);
    // bss        — от __bss_start__ до __bss_end__
    row("bss", (uintptr_t)&__bss_start__,(uintptr_t)&__bss_end__);
    // heap       — от __bss_end__ до __HeapLimit
    row("heap", (uintptr_t)&__bss_end__,(uintptr_t)&__HeapLimit);
    // stack      — от __StackBottom до __StackTop
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

    // итог: образ во флеш и из чего он сложился
    printf("%s %8u", "flash image",   (unsigned)((uintptr_t)&__boot2_end__-(uintptr_t)& __boot2_start__)+
                             (unsigned)((uintptr_t)&__etext- (uintptr_t)&__boot2_end__ )+
                             (unsigned)((uintptr_t)&__data_end__- (uintptr_t)&__data_start__ ) );

    printf("%s %8u",  " = boot2",  (unsigned)((uintptr_t)&__boot2_end__-(uintptr_t)& __boot2_start__));
    printf("%s %8u", "+ text",  (unsigned)((uintptr_t)&__etext- (uintptr_t)&__boot2_end__ )) ;
    printf("%s %8u\n", "+ data",  (unsigned)((uintptr_t)&__data_end__- (uintptr_t)&__data_start__ ));  
    

    // итог: свободно во флеш-памяти из всего её объёма
     printf("%s %8u", "flash free",   (unsigned)PICO_FLASH_SIZE_BYTES - 
                                    ((unsigned)((uintptr_t)&__boot2_end__-(uintptr_t)& __boot2_start__)+
                             (unsigned)((uintptr_t)&__etext- (uintptr_t)&__boot2_end__ )+
                             (unsigned)((uintptr_t)&__data_end__- (uintptr_t)&__data_start__ ) ));

    printf("%s %8u\n", " of",  (unsigned)PICO_FLASH_SIZE_BYTES);  
    

    // итог: занято в ОЗУ — .data и .bss
    printf("%s %8u", "ram used",  (unsigned)((uintptr_t)&__bss_end__-(uintptr_t)&__bss_start__ )+
                              (unsigned)((uintptr_t)&__data_end__- (uintptr_t)&__data_start__ ) );

    printf("%s %8u",  " = bss",  (unsigned)((uintptr_t)&__bss_end__-(uintptr_t)&__bss_start__ ));
    printf("%s %8u\n", "+ data",  (unsigned)((uintptr_t)&__data_end__- (uintptr_t)&__data_start__ ));  


    // итог: свободно в ОЗУ — под кучу и под стек
    printf("%s %8u %s %8u %s\n ", "ram free",  
            (unsigned)((uintptr_t)&__HeapLimit - (uintptr_t)&__bss_end__ )," for heap and", 
            (unsigned)((uintptr_t)&__StackTop - (uintptr_t)&__StackBottom ),"for stack");
}

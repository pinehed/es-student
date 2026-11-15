#pragma once

#include "pico/types.h"  //ради типа uint: uint — не язык C, а имя из Pico SDK.
 
typedef void (*command_handler_t)(void);

struct command_t
{
    const char *name;
    command_handler_t handler;
};

// extern -  «она где-то есть», не отводя памяти.   
//Без него const uint command_count; в заголовке — не объявление, а определение: переменная заводится в каждом файле, который заголовок включил, и компоновщик остановится на multiple definition. 
extern const struct command_t commands[];
extern const uint command_count;
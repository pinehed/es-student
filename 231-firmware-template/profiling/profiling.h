#pragma once

#include "pico/types.h"

void profiling_init(void); // запоминает начальное показание часов
void profiling_iteration(void);//вызывается в начале каждой итерации и учитывает прошлую. 
//три функции отдают результат и сбрасывают максимум:
float profiling_avg_us(void);
uint32_t profiling_max_us(void);
void profiling_reset_max(void);



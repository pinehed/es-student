#include <stdio.h> // ради printf(),
#include <stdint.h>  // типы SDK, 
#include "hardware/clocks.h" //ради функций работы с тактовыми сигналами.

//uint32_t sys_hz = clock_get_hz(clk_sys);
//uint32_t sys_khz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);


//. Печать строки
static void row(const char *name, uint32_t set_khz, uint32_t measured_khz)
{
    printf("%-8s %9u %12u\n", name, (unsigned)set_khz, (unsigned)measured_khz);
}
static void row_rosc(const char *name, const char *name1, uint32_t measured_khz)
{
    printf("%-8s %8s %12u\n", name, name1, (unsigned)measured_khz);
}


void clk_info(void)
{
    // шапка таблицы: сигнал, настроенная частота, измеренная частота
          printf("%-10s %-10s %-10s\n",    
            "signal","set_khz","measured_khz");

    // clk_ref, clk_sys, clk_peri, clk_usb, clk_adc — clock_get_hz() в кГц и frequency_count_khz()
row("clk_ref", clock_get_hz(clk_ref) / 1000, frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_REF));
row("clk_sys", clock_get_hz(clk_sys) / 1000, frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
row("clk_peri", clock_get_hz(clk_peri) / 1000, frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI));
row("clk_usb", clock_get_hz(clk_usb) / 1000, frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB));
row("clk_adc", clock_get_hz(clk_adc) / 1000, frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC));
    // rosc — прочерк вместо настроенной частоты и измеренная частота
row_rosc("rosc", "-", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC));
}

extern uint64_t time_us_64();

//Функция, печатает показание счёта в миллисекундах. 
//time_us_64() - Монотонное время прибора ведёт таймер. Он прибавляет единицу раз в микросекунду, начиная с момента сброса. 
void uptime(void)
{
    printf("uptime: %llu ms\n", time_us_64() / 1000);
}
#include <stdio.h> // ради printf(),
#include <stdint.h>  // типы SDK, 
#include "hardware/clocks.h" //ради функций работы с тактовыми сигналами.
#include "log.h" //Для LOG_INF() и LOG_ERR() 
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


//константа пониженной частоты
const uint32_t CLK_SYS_LOW_KHZ = 62500;
//функцию, которая ставит частоту и сообщает результат:
static void clk_sys_set(uint32_t khz)  // остаётся внутри модуля, поэтому объявлена со словом static.
{
    if (set_sys_clock_khz(khz, false))  //Частоту ядра меняет функция SDK set_sys_clock_khz()
    {
        LOG_INF("clk_sys %u kHz\n", (unsigned)khz);
    }
    else
    {
        LOG_ERR("clk_sys %u kHz is not set\n", (unsigned)khz);
    }
}

void clk_sys_low(void)
{
    clk_sys_set(CLK_SYS_LOW_KHZ);  
}

void clk_sys_default(void)
{
    clk_sys_set(SYS_CLK_KHZ);//Макрос SYS_CLK_KHZ задан в Pico SDK и равен рабочей частоте 125 000 кГц
}
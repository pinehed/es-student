#include "led-task.h"

#include "pico/stdlib.h"

// Состояние задачи: внутреннее связывание, только для этого файла
static led_state_t led_state;
static uint64_t half_period_us;
static uint64_t last_toggle_us;

// Вспомогательная функция: из других файлов её не вызвать
static void led_set(bool on)
{
    gpio_put(PICO_DEFAULT_LED_PIN, on);
}

bool led_task_set_period_ms(uint32_t period_ms)
{
    // С нулевым полупериодом светодиод переключался бы
    // на каждой итерации: такой период не принимается
    if (period_ms == 0) {
        return false;
    }

    half_period_us = (uint64_t)period_ms * 1000 / 2;
    return true;
}


//Проверка периода  - НАДО? СЮДА ЛИ?
bool period_elapsed(uint64_t *last_us, uint64_t period_us)
{
    uint64_t now_us = time_us_64();

    if (now_us - *last_us < period_us) {
        return false;
    }

    // Следующее срабатывание отсчитывается от прошлого, а не от «сейчас»
    *last_us += period_us;

    // Отстали больше чем на период: не догонять, а начать отсчёт заново
    if (now_us - *last_us >= period_us) {
        *last_us = now_us;
    }

    return true;
}


void led_task_init(void)
{
    // 1 настраиваем вывод светодиода, период 1000 мс, состояние «мигает»
}

void led_task_set_state(led_state_t state)
{
    //2-1 запоминаем состояние и выполняем действие при входе:
    // 2 -2 «выключен» — гасим, «горит» — зажигаем, «мигает» — первое переключение сразу
  led_state = state;

    // Действие при входе: выполняется один раз, в момент перехода
    switch (led_state) {
    case LED_STATE_OFF:
        gpio_put(PICO_DEFAULT_LED_PIN, false);
        break;
    case LED_STATE_ON:
        gpio_put(PICO_DEFAULT_LED_PIN, true);
        break;
    case LED_STATE_BLINK:
        // Первое переключение — на ближайшей итерации
        last_toggle_us = 0;
        break;
    }
}



void led_task_handle(void)
{
    switch (led_state)
    {
    // 3 -1  «выключен» и «горит» — ничего не делаем, светодиод установлен при входе
    // «3 - 2 мигает» — каждые полпериода переключаем светодиод
    case LED_STATE_OFF:
    case LED_STATE_ON:
        // Светодиод установлен при входе в состояние, делать нечего
        break;
    case LED_STATE_BLINK:
        if (period_elapsed(&last_toggle_us, 500000)) {
            gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
        }
        break;

    }
}
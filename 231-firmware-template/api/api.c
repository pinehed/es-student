// обработчик команды: получает имя и аргументы
typedef void (*api_callback_t)(const command_t *command);

// команда прибора: имя, справка для help и обработчик
typedef struct
{
    const char *name;
    const char *help;
    api_callback_t callback;
} api_command_t;


void cmd_info(void)
{
    // печатаем паспорт устройства
    device_info();
}


//Массив - таблица команд
void cmd_uptime(void)
{
    // 2_1_5 чтение по адресу
   uptime(); 
}


//число пи методом Лейбница
// прикидка: за член ряда 4 операции с double, 175 + 110 + 190 + 110 = 585 тактов;
// 1 000 000 членов по 585 тактов при 125 МГц — около 4,7 с
const uint CALC_PI_TERMS = 1000000;
double calc_pi(uint terms)
{
    // сумма ряда и знак очередного члена, оба double
    double sum = 0.0;
    double sign = 1.0;

    // для k от 0 до terms: прибавить к сумме sign / (2k + 1) и сменить знак
        for (int k = 0; k < terms; k++) {
        // Прибавить очередной член ряда: ±1 / (2k + 1)
        sum += sign / (2.0 * k + 1.0);
        sign = -sign;
    }

    // вернуть сумму, умноженную на 4
    // Сумма ряда равна π/4
    return 4.0 * sum;
}

volatile double pi_result;//обращения через volatile компилятор не выбрасывает и не переставляет друг относительно друга. Без этого слова компилятор вправе перенести расчёт за второе чтение часов. Результат от этого не изменится, но замер измерит пустое место.

void cmd_calc_pi(void)
{
    uint64_t start_us = time_us_64();
    pi_result = calc_pi(CALC_PI_TERMS);
    uint64_t spent_us = time_us_64() - start_us;

    printf("pi: %.8f\n", pi_result);
    printf("time: %llu ms\n", spent_us / 1000);
}

void cmd_main_time_exec(void)
{ //печатает среднее и максимум 
   printf("iteration avg %.2f us, max %u us\n", profiling_avg_us(), (unsigned)profiling_max_us());
}

void cmd_main_time_reset(void)
{ 
   profiling_reset_max();
   printf("max reset\n");
}


const struct api_command_t commands[] = {
    { "help", "list of commands", cmd_info },
    { "info", "device passport", cmd_info },
    { "uptime", "uptime", cmd_uptime},
    { "calc_pi",  "calc_pi",cmd_calc_pi },
    { "main_time_reset", "main_time_reset", cmd_main_time_reset},
    { "main_time_exec", "main_time_exec",cmd_main_time_exec },

};
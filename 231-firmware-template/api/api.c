// обработчик команды: получает имя и аргументы
typedef void (*api_callback_t)(const command_t *command);

// команда прибора: имя, справка для help и обработчик
typedef struct
{
    const char *name;
    const char *help;
    api_callback_t callback;
} api_command_t;


void cmd_info(const command_t *command)
{
    // печатаем паспорт устройства
    device_info();
}


//Массив - таблица команд
void cmd_uptime(const command_t *command)
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

void cmd_calc_pi(const command_t *command)
{
    uint64_t start_us = time_us_64();
    pi_result = calc_pi(CALC_PI_TERMS);
    uint64_t spent_us = time_us_64() - start_us;

    printf("pi: %.8f\n", pi_result);
    printf("time: %llu ms\n", spent_us / 1000);
}

void cmd_main_time_exec(const command_t *command)
{ //печатает среднее и максимум 
   printf("iteration avg %.2f us, max %u us\n", profiling_avg_us(), (unsigned)profiling_max_us());
}

void cmd_main_time_reset(const command_t *command)
{ 
   profiling_reset_max();
   printf("max reset\n");
}


const struct api_command_t commands[] = {
    { "info", "device passport", cmd_info },
    { "uptime", "uptime", cmd_uptime},
    { "calc_pi",  "calc_pi",cmd_calc_pi },
    { "main_time_reset", "main_time_reset", cmd_main_time_reset},
    { "main_time_exec", "main_time_exec",cmd_main_time_exec },

};




//command_count  - cчитаем длину 
const uint command_count = sizeof(commands) / sizeof(commands[0]);

//функция для help в api_handle
void api_handle_help (void)
{
    printf("%-15s %s\n","help", "list of commands");
    for (uint i = 0; i < command_count; i++)
    {
         printf("%-15s %s\n",commands[i].name, commands[i].help);
    }
}

void api_handle(const command_t *command)
{// 1 Команду с пометкой truncated она не выполняет и отвечает error: command longer than 63 characters or 4 arguments.
    if (command.truncated)
    {
        printf ("error: command longer than 63 characters or 4 arguments");
        return; //// return - выполнить и выкинуть из всего цикла api_handle.
    }

     // 2 Команду help она выполняет сама. Печатает строку о себе, а за ней имена и справку всех команд таблицы.
    if (strcmp(command.name, "help") == 0)
    {
         api_handle_help();
         return;
    }
 
// 3  -1 Если имя есть в таблице, вызывает его обработчик.
    for (uint i = 0; i < command_count; i++)
    {
        if (strcmp(command.name, commands[i].name) == 0)
        {
            if (commands[i].callback != NULL) //важно! Что бы не было: ядро передаст управление в начало Boot ROM (адрес )
            {
                commands[i].callback(command);//- вызов по указателю: скобки после указателя означают «передать управление по этому адресу». 
            }

            return;
        }
    }
//3 -2  Если имени в таблице нет, отвечает error: unknown command '<имя>', try help.
// return - выкидывали из всего цикла api_handle.  Если не выкинуло, то дошли сюда
   printf ("error: unknown command '%s', try help\n", command.name);
    
}



//Разбор числового аргумента = т2.3.8
// Число без знака из десятичной записи. false — в записи не только цифры
// или число не помещается в uint32_t.
static bool parse_u32(const char *text, uint32_t *value)
{
    // strtoul приняла бы и знак: из "-1" получилось бы 4294967295
    if (!isdigit((unsigned char)text[0]))
    {
        return false;
    }

    char *end;
    errno = 0;
    unsigned long number = strtoul(text, &end, 10);

    // после числа остались символы или число не поместилось
    if (*end != '\0' || errno == ERANGE || number > UINT32_MAX)
    {
        return false;
    }

    *value = (uint32_t)number;
    return true;
}
static void cmd_led_period(const command_t *command)
{
    uint32_t period_ms;

    if (command->argc != 1 || !parse_u32(command->argv[0], &period_ms))
    {
        printf("error: usage led_period <period_ms>\n");
        return;
    }

    // годится ли такой период, решает задача светодиода
    if (!led_task_set_period_ms(period_ms))
    {
        printf("error: period_ms must be greater than 0\n");
        return;
    }

    // ответ: состояние и период светодиода
    print_led_state();
}





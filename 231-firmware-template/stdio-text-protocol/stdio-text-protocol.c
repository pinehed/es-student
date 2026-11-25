const command_t *stdio_text_protocol_handle(void)
{
    //буфер строки
    #define LINE_SIZE 32
    char line[LINE_SIZE];
    uint line_length = 0;

    // 1 читаем символ без ожидания; нет символа — возвращаем NULL
    // ~ getchar_timeout_us(0)
    int c = getchar_timeout_us(0);
    if (c == PICO_ERROR_TIMEOUT) 
    {
        return NULL;
    }
    else {
        handle_char(c);
    }
    

    // 2  конец строки: пустую строку пропускаем, иначе переводим строку в терминале,
    // 3 делим строку на слова и возвращаем команду
    // команда, принятая из одной строки

    
        // принятая строка становится командой, команда — действием прибора

    
    // 4 Backspace (0x08 или 0x7f): убираем последний символ и стираем его на экране "\b \b"
   if (symbol == '0x08' || symbol == '0x7F')
    {
        
    }
    // 5  печатный символ: кладём в буфер и повторяем в терминал;
    // 6 буфер полон — помечаем строку как переполненную

        // строка длиннее буфера или аргументов больше, чем помещается в argv:
    // команда принята не целиком, выполнять её нельзя

}




//Из маин
//функция: сбор строки из символов
void read_line(void)
{
    int symbol = getchar_timeout_us(0);

    if (symbol == PICO_ERROR_TIMEOUT)
    {
        return;
    }

    if (symbol == '\r' || symbol == '\n')
    {
        putchar('\n');
        line[line_length] = '\0';

        if (line_length > 0)
        {
            LOG_DBG("got %s\n", line);
            handle_command(line);
        }

        line_length = 0;
        return;
    }

    if (line_length + 1 < LINE_SIZE)
    {
        line[line_length] = (char)symbol;
        line_length = line_length + 1;
        putchar(symbol);
    }
}
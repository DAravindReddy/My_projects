

#include"main.h"

unsigned char clock_reg[3];
unsigned char time[7];
unsigned char log[11];
unsigned char log_pos = 0, sec;
unsigned char return_time;
unsigned char *menu[] = {"View log", "Clear log", "Download log", "Change passwd", "Set time"};
unsigned char menu_pos;
unsigned char clear_flag = 1;
unsigned char current_screen;




static void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD

    /* To store the time in HH:MM:SS format */
    //char time[9];  // "HH:MM:SS"
    // HH ->
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';

    //time[2] = ':';
    // MM
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';

    //time[4] = ':';
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';

    //clcd_print(time, LINE2(4)); // HH:MM:SS
}

void display_time()
{
    get_time();
    //HH:MM:SS
    clcd_putch(time[0], LINE2(1));
    clcd_putch(time[1], LINE2(2));
    clcd_putch(':', LINE2(3));
    clcd_putch(time[2], LINE2(4));
    clcd_putch(time[3], LINE2(5));
    clcd_putch(':', LINE2(6));
    clcd_putch(time[4], LINE2(7));
    clcd_putch(time[5], LINE2(8));
}

void display_dashboard(unsigned char event[] , unsigned char speed)
{
    //display title message
    clcd_print(" TIME     E  SP", LINE1(0));

    //Display the TIME
    display_time();

    //display event
    clcd_print(event, LINE2(10));

    //display speed
    clcd_putch((speed / 10) + '0', LINE2(13));
    clcd_putch((speed % 10) + '0', LINE2(14));

}

void store_event()
{
    char addr;
    if(log_pos == 10)
    {
        log_pos = 0;
    }
    addr = 0x05 + log_pos * 10;
    ext_eeprom_24C02_str_write(addr, log);
    log_pos++;
}

void log_event(unsigned char event[], unsigned char speed)
{
    get_time();
    strncpy(log, time, 6);
    strncpy(&log[6], event, 2);
    log[8] = speed/10 + '0';
    log[9] = speed%10 + '0';
    log[10] = '\0';

    store_event();
}

unsigned char login_screen(unsigned char key, unsigned char reset_flag)
{
    static char user_passwd[4];
    static unsigned char i;
    static unsigned char attempt_left;


    if(reset_flag == RESET_PASSWORD)
    {
        i = 0;
        attempt_left = 3;
        user_passwd[0] = '\0';
        user_passwd[1] = '\0';
        user_passwd[2] = '\0';
        user_passwd[3] = '\0';

        key = ALL_RELEASED;
        return_time = 5;
    }
    if(return_time == 0)
    {
        return RETURN_BACK;
    }
    //SW4 = 1, SW = 0
    if(key == SW4 && i < 4)
    {
        clcd_putch('*', LINE2(4+i));
        user_passwd[i] = '1';
        i++;
        return_time = 5;
    }
    else if(key == SW5 && i < 4)
    {
        clcd_putch('*', LINE2(4+i));
        user_passwd[i] = '0';
        i++;
        return_time = 5;
    }
    if(i == 4)
    {
        char s_passwd[4];
        for(int j = 0 ; j < 4; j++)
        {
            s_passwd[j] = ext_eeprom_24C02_read(j);
        }
        //compare both passd
        if(strncmp(user_passwd, s_passwd, 4) == 0)
        {
            clear_display();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            clcd_print("Login Success", LINE1(1));
            __delay_ms(3000);

            //change to the main menu
            return LOGIN_SUCCESS;
        }
        else
        {
            attempt_left--;
            if(attempt_left == 0)
            {
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("you are blocked", LINE1(0));
                clcd_print("wait for ", LINE2(0));
                clcd_print("secs", LINE2(12));
                sec = 60;
                while(sec)
                {
                    clcd_putch(sec/10 + '0', LINE2(9));
                    clcd_putch(sec%10 + '0', LINE2(10));
                }
                attempt_left = 3;
            }
            else
            {
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("Wrong password", LINE1(0));
                clcd_print("attempt left", LINE2(0));
                clcd_putch(attempt_left + '0', LINE2(13));
                __delay_ms(3000);
            }
            clear_display();
            clcd_print("Enter password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            i = 0;
            return_time = 5;
        }
    }
}

unsigned char menu_screen(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_MENU)
    {
        menu_pos = 0;
        return_time = 5;
    }
    if(key == SW4 && menu_pos > 0)
    {
        menu_pos--;
        clear_display();
    }
    else if(key == SW5 && menu_pos < 4)
    {
        menu_pos++;
        clear_display();
    }
    if(menu_pos == 4)
    {
        clcd_putch('*', LINE2(0));
        clcd_print(menu[menu_pos - 1], LINE1(2));
        clcd_print(menu[menu_pos], LINE2(2));
    }
    else{
    clcd_putch('*', LINE1(0));
    clcd_print(menu[menu_pos], LINE1(2));
    clcd_print(menu[menu_pos + 1], LINE2(2));
    }
    return menu_pos;
}


void clear_display()
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

void view_log()
{
    static unsigned char index = 0; // Current log index to display
    unsigned char key = ALL_RELEASED;
    unsigned char eeprom_addr;
    char clcd_buffer[17];
    static unsigned short delay_sw4 = 0, delay_sw5 = 0;
    int i;

    if (clear_flag == 1)
    {
        key = read_digital_keypad(LEVEL);

        // Display header
        clcd_print("# TIME     E  SP", LINE1(0));

        // Handle SW4 (Scroll up / Main menu)
        if (key == SW4)
        {
            delay_sw4++;

            if (delay_sw4 > LONG_PRESS_THRESHOLD)
            {
                delay_sw4 = 0;
                current_screen = MAIN_MENU_SCREEN; // Long press action
            }
        }
        else if (delay_sw4 > 0 && delay_sw4 < LONG_PRESS_THRESHOLD)
        {
            if (index > 0)
                index--;
            delay_sw4 = 0;
        }

        // Handle SW5 (Scroll down / Dashboard)
        if (key == SW5)
        {
            delay_sw5++;

            if (delay_sw5 > LONG_PRESS_THRESHOLD)
            {
                delay_sw5 = 0;
                current_screen = DASHBOARD_SCREEN; // Long press action
            }
        }
        else if (delay_sw5 > 0 && delay_sw5 < LONG_PRESS_THRESHOLD)
        {
            if (index < (log_pos - 1) && index < (MAX_LOGS - 1))
                index++;
            delay_sw5 = 0;
        }
        if (index < log_pos) // Make sure index is valid
        {
            eeprom_addr = 0x05 + (index * LOG_SIZE);

            // Read 11 bytes of the log from EEPROM
            for (i = 0; i < LOG_SIZE; i++)
            {
                log[i] = ext_eeprom_24C02_read(eeprom_addr + i);
            }

            // Format log line for line 2
            clcd_buffer[0]  = index + '0';  // Log number starting at '1'
            clcd_buffer[1]  = ' ';
            clcd_buffer[2]  = log[0];
            clcd_buffer[3]  = log[1];
            clcd_buffer[4]  = ':';
            clcd_buffer[5]  = log[2];
            clcd_buffer[6]  = log[3];
            clcd_buffer[7]  = ':';
            clcd_buffer[8]  = log[4];
            clcd_buffer[9]  = log[5];
            clcd_buffer[10] = ' ';
            clcd_buffer[11] = log[6];
            clcd_buffer[12] = log[7];
            clcd_buffer[13] = ' ';
            clcd_buffer[14] = log[8];
            clcd_buffer[15] = log[9];
            clcd_buffer[16] = '\0';


        }
        clcd_print(clcd_buffer, LINE2(0));
    }
    else
    {
        clear_display();
        clcd_print("No logs found", LINE1(0));
    }
}


void clear_log()
{
    unsigned char key = ALL_RELEASED;
    static unsigned short delay_sw4 = 0, delay_sw5 = 0;

    if (clear_flag == 1)
    {
        // Clear log logic
        log_pos = 0;          // Reset the log count
        clear_flag = 0;       // Mark logs as cleared

        // Feedback to user
        clear_display();
        clcd_print("Logs cleared", LINE1(2));
        clcd_print("successfully", LINE2(2));
    }

    // Keypad input
    key = read_digital_keypad(LEVEL);

    // Long press SW4 ? Main Menu
    if (key == SW4)
    {
        delay_sw4++;
        if (delay_sw4 > 200)
        {
            current_screen = MAIN_MENU_SCREEN;
            delay_sw4 = 0;
        }
    }
    else
    {
        delay_sw4 = 0;
    }

    // Long press SW5 ? Dashboard
    if (key == SW5)
    {
        delay_sw5++;
        if (delay_sw5 > 200)
        {
            current_screen = DASHBOARD_SCREEN;
            delay_sw5 = 0;
        }
    }
    else
    {
        delay_sw5 = 0;
    }
}

void download_log()
{
    unsigned char key = ALL_RELEASED;
    static unsigned short delay_sw4 = 0, delay_sw5 = 0;
    unsigned char eeprom_addr;
    unsigned char log[11];
    char uart_buffer[25];
    int i;

    if (clear_flag == 1)
    {
        for (i = 0; i < log_pos; i++)
        {
            eeprom_addr = 0x05 + (i * 11);

            // Read 11-byte log from EEPROM
            for (int j = 0; j < 11; j++)
            {
                log[j] = ext_eeprom_24C02_read(eeprom_addr++);
            }

            // Format log: "<log_no> HH:MM:SS E SP\r\n"
            uart_buffer[0] = i + '1';  // log number (1,2,3...)
            uart_buffer[1] = ' ';
            uart_buffer[2] = log[0];   // HH
            uart_buffer[3] = log[1];
            uart_buffer[4] = ':';
            uart_buffer[5] = log[2];   // MM
            uart_buffer[6] = log[3];
            uart_buffer[7] = ':';
            uart_buffer[8] = log[4];   // SS
            uart_buffer[9] = log[5];
            uart_buffer[10] = ' ';
            uart_buffer[11] = log[6];  // Event
            uart_buffer[12] = log[7];
            uart_buffer[13] = ' ';
            uart_buffer[14] = log[8];  // Speed tens
            uart_buffer[15] = log[9];  // Speed ones
            uart_buffer[16] = '\r';
            uart_buffer[17] = '\n';
            uart_buffer[18] = '\0';

            puts(uart_buffer);  // Send formatted log via UART
        }

        clcd_print("Logs downloading", LINE1(0));
        clcd_print("Check Serial Out", LINE2(0));
    }
    else
    {
        clear_display();
        clcd_print("No logs found", LINE1(2));
    }

    // Handle DKP for screen navigation
    key = read_digital_keypad(LEVEL);

    if (key == SW4)
    {
        delay_sw4++;
        if (delay_sw4 > 200)
        {
            current_screen = MAIN_MENU_SCREEN;
            delay_sw4 = 0;
        }
    }
    else
    {
        delay_sw4 = 0;
    }

    if (key == SW5)
    {
        delay_sw5++;
        if (delay_sw5 > 200)
        {
            current_screen = DASHBOARD_SCREEN;
            delay_sw5 = 0;
        }
    }
    else
    {
        delay_sw5 = 0;
    }
}


void change_passwd()
{
    char new_passwd1[5] = "", new_passwd2[5] = "";
    unsigned char i = 0, key;
    char clcd_buffer[17];

    clear_display();
    clcd_print("Enter New Passwd", LINE1(0));

    // Step 1: Read first password
    while (i < 4)
    {
        key = read_digital_keypad(STATE);
        if (key == SW4)
        {
            new_passwd1[i] = '1';
            clcd_putch('*', LINE2(i));  // Show '*' for entered digit
            i++;
            __delay_ms(300); // Debounce delay
        }
        if(key == SW5)
        {
            new_passwd1[i] = '0';
            clcd_putch('*', LINE2(i));  // Show '*' for entered digit
            i++;
            __delay_ms(300); // Debounce delay
        }
    }
    new_passwd1[4] = '\0';

    __delay_ms(500);
    clear_display();
    clcd_print("Re-enter Passwd", LINE1(0));

    // Step 2: Read second password
    i = 0;
    while (i < 4)
    {
        key = read_digital_keypad(STATE);
        if (key == SW4)
        {
            new_passwd2[i] = '1';
            clcd_putch('*', LINE2(i));
            i++;
            __delay_ms(300);
        }
        if (key == SW5)
        {
            new_passwd2[i] = '0';
            clcd_putch('*', LINE2(i));
            i++;
            __delay_ms(300);
        }
    }
    new_passwd2[4] = '\0';

    __delay_ms(500);
    clear_display();

    // Step 3: Compare passwords
    if (strcmp(new_passwd1, new_passwd2) == 0)
    {
        // Step 4: Save new password to EEPROM
        for (i = 0; i < 4; i++)
        {
            ext_eeprom_24C02_byte_write(0x00 + i, new_passwd1[i]);
            __delay_ms(5); // EEPROM write delay
        }

        clcd_print("Password Change", LINE1(0));
        clcd_print("Success", LINE2(4));

    }
    else
    {
        clcd_print("Password Change", LINE1(0));
        clcd_print("Failure", LINE2(4));
    }

    // Wait 1.5 seconds and return to main menu
    __delay_ms(1500);

}
              // formatted time string

void set_time()
{
    unsigned char hour = 0, min = 0, sec = 0;
    unsigned char field = 2; // Start from SS
    unsigned char key;
    unsigned short delay_sw4 = 0, delay_sw5 = 0;
    static unsigned char blink = 1;
    static unsigned short blink_timer = 0;

    // Load current time from RTC
    get_time();

    // Convert string back to int
    hour = (time[0] - '0') * 10 + (time[1] - '0');
    min  = (time[2] - '0') * 10 + (time[3] - '0');
    sec  = (time[4] - '0') * 10 + (time[5] - '0');
    clear_display();
    clcd_print("Set Time", LINE1(4));

    while (1)
    {
        // Time rendering with blinking
        clcd_putch((field == 0 && !blink) ? ' ' : (hour / 10) + '0', LINE2(1));
        clcd_putch((field == 0 && !blink) ? ' ' : (hour % 10) + '0', LINE2(2));
        clcd_putch(':', LINE2(3));
        clcd_putch((field == 1 && !blink) ? ' ' : (min / 10) + '0', LINE2(4));
        clcd_putch((field == 1 && !blink) ? ' ' : (min % 10) + '0', LINE2(5));
        clcd_putch(':', LINE2(6));
        clcd_putch((field == 2 && !blink) ? ' ' : (sec / 10) + '0', LINE2(7));
        clcd_putch((field == 2 && !blink) ? ' ' : (sec % 10) + '0', LINE2(8));

        // Blinking logic
        blink_timer++;
        if (blink_timer > 50)
        {
            blink_timer = 0;
            blink = !blink;
        }

        key = read_digital_keypad(LEVEL);

        // Long press SW4: Save time
        if (key == SW4)
        {
            delay_sw4++;
            if (delay_sw4 > 200)
            {
                // Convert to BCD before writing
                clock_reg[0] = ((hour / 10) << 4) | (hour % 10); // HH
                clock_reg[1] = ((min / 10) << 4) | (min % 10);   // MM
                clock_reg[2] = ((sec / 10) << 4) | (sec % 10);   // SS

                write_ds1307(HOUR_ADDR, clock_reg[0]);
                write_ds1307(MIN_ADDR, clock_reg[1]);
                write_ds1307(SEC_ADDR, clock_reg[2]);

                clear_display();
                clcd_print("Time Set", LINE1(4));
                clcd_print("Success", LINE2(4));
                __delay_ms(1500);
                current_screen = MAIN_MENU_SCREEN;
                return;
            }
        }
        else if (delay_sw4 > 0 && delay_sw4 < 200) // Short press SW4 = Increment
        {
            switch (field)
            {
                case 0: hour = (hour + 1) % 24; break;
                case 1: min = (min + 1) % 60; break;
                case 2: sec = (sec + 1) % 60; break;
            }
            delay_sw4 = 0;
        }

        // Long press SW5: Exit without saving
        if (key == SW5)
        {
            delay_sw5++;
            if (delay_sw5 > 200)
            {
                current_screen = DASHBOARD_SCREEN;
                return;
            }
        }
        else if (delay_sw5 > 0 && delay_sw5 < 200) // Short press SW5 = Change field
        {
            field = (field + 1) % 3;
            delay_sw5 = 0;
        }

        __delay_ms(20);
    }
}


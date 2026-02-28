

/*---------------------------PROJECT-1-----------------------------------------
Name: Aravind
Date: 20/5/2025
Description: Car_black_box project
Peripherals : PIC16F877A, RTC, CLCD, EEPROM, DKP, POTENTIOMETER, I2C, UART, ADC
 * ---------------------------------------------------------------------------*/
#include"main.h"


#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)

void init_config()
{
    init_clcd();
    init_digital_keypad();
    init_adc();
    init_i2c(100000);
    init_ds1307();
    init_timer2();
    PEIE = 1;
    GIE = 1;
}

void main()
{
    init_config();
    unsigned char reset_flag;
    unsigned char control_flag = DASHBOARD_SCREEN;
    unsigned char event[3] = "ON";
    unsigned char speed = 0, key;
    unsigned char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};
    unsigned char gr = 0, menu_pos;

    ext_eeprom_24C02_str_write(0x00, "1010");
    log_event(event, speed);
    while(1)
    {
        speed = read_adc()/10.3;//0-1023 => 0 - 90

        key = read_digital_keypad(STATE);
        for(unsigned int i = 300; i--;);//delay

        if(key == SW1)
        {
            //display the collision
            strcpy(event, "CO");
            log_event(event, speed);
        }
        else if(key == SW2 && gr < 6)
        {
            //increase the gear
            strcpy(event, gear[gr]);
            gr++;
            log_event(event, speed);
        }
        else if(key == SW3 && gr > 0)
        {
            gr--;
            //decrease the gear
            strcpy(event, gear[gr]);
            log_event(event, speed);
        }
        else if((key == SW4 || key == SW5) && control_flag == DASHBOARD_SCREEN)
        {
            control_flag = LOGIN_SCREEN;
            clear_display();
            clcd_print("Enter password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            reset_flag = RESET_PASSWORD;
            TMR2ON = 1;
        }
        else if(control_flag == MAIN_MENU_SCREEN && key ==  SW4)
        {
            switch(menu_pos)
            {
                case 0:
                    control_flag = VIEW_LOG_SCREEN;
                    break;
                case 1:
                    control_flag = CLEAR_LOG_SCREEN;
                    break;
                case 2:
                    control_flag = DOWNLOAD_LOG_SCREEN;
                    break;
                case 3:
                    control_flag = CHANGE_PASSWORD_SCREEN;
                    break;
                case 4:
                    control_flag = SET_TIME_SCREEN;
                    break;
            }
        }
        switch(control_flag)
        {
            case DASHBOARD_SCREEN:
                display_dashboard(event, speed);
                break;
            case LOGIN_SCREEN:
                switch(login_screen(key, reset_flag))
                {
                    case RETURN_BACK:
                        control_flag = DASHBOARD_SCREEN;
                        clear_display();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    case LOGIN_SUCCESS:
                        control_flag = MAIN_MENU_SCREEN;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        clear_display();
                        reset_flag = RESET_MENU;
                        continue;
                }
                break;
            case MAIN_MENU_SCREEN:
                menu_pos = menu_screen(key, reset_flag);
                break;
                case VIEW_LOG_SCREEN:
                    view_log();
                    break;
                case CLEAR_LOG_SCREEN:
                    clear_log();
                    break;
                case DOWNLOAD_LOG_SCREEN:
                    download_log();
                    break;
                case CHANGE_PASSWORD_SCREEN:
                    change_passwd();
                    break;
                case SET_TIME_SCREEN:
                    set_time();
                    break;
        }
        reset_flag = RESET_NOTHING;
    }
}

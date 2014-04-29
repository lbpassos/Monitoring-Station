void displayMenu_voltage();
void displayMenu_corrente();
void displayMenu_temperatura();
void displayMenu_humidade();
void displayMenu_consumo();
void display_erase();
void display_title();

void display_voltage(float v);
void display_current(float v);
void display_temperatura(char t_int, unsigned char t_quo);
void display_humidade(unsigned char h_int, unsigned char h_quo);		
void display_consumo(float v);
void display_pFactor( float v );
//void display_data(unsigned char ano_mes, unsigned char dia, unsigned char hora, unsigned char min);

int displayNumber(int c, unsigned char pos_x, unsigned char pos_y);

void erase_voltage();
void erase_current();
void erase_temperatura();
void erase_humidade();
void erase_consumo();



	
//void acerto_rtc(rtclock *rtc_clock);
//void start_managerDisplay();
//void button_lcd(void);

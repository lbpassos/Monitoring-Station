#include "manager_display.h"
#include "button.h"
#include "nokia_6610.h"
#include "lpc2106.h"
#include "rtc.h"
#include "date.h"

#define menuItem	5 // Num de menus
#define	DIGIT_SIZE		8
#define GRAU_POS_STRING	2 // Pos do simbolo grau na string na temperatura
#define MES_POS_STRING	1 // Pos do simbolo e com chapeu

#define INIT				0
#define START				1
#define BUTTON_PRESSED		2
#define FINNISH				3
#define SHOW_MENU			4
#define BUTTON_RELEASED		5
#define SHOW_VALUE			6

#define U_X				30
#define U_Y				0	
#define I_X				50
#define I_Y				0
#define P_X				70
#define P_Y				0
#define T_X				90
#define T_Y				0
#define HUM_X			110
#define HUM_Y			0


#define UM_S				1000
#define DEZ_S				10*1000
#define TRINTA_S			30 //30s com pclk=1ms
#define SESSENTA_S			60


enum Color {	WHITE = 0xFFF, BLACK = 0x000, RED = 0xF00,	GREEN = 0x0F0, 
				BLUE = 0x00F,  CYAN = 0x0FF,  MAGENTA = 0xF0F,
				YELLOW = 0xFF0, BROWN = 0xB22, ORANGE = 0xFA0, PINK = 0xF6A };
				

typedef struct _menu{
	unsigned char *ptr;
	unsigned char value;
	int (*ptr_func)(unsigned char );
} Menu;


static const char id[] = "P109 - 22164";
static const char tensao[] = "U[V] = ";
static const char corrente[] = "I[A] = ";
static const char consumo[] = "E[Wh] = ";
static const char temperatura[] = "T[ C] = ";
static const unsigned char grau = 128; // Simbolo de grau
static const char humidade[] = "H[%] = ";
//static const char p_factor[] = "PF = ";


static unsigned char button_lcd_state;
static unsigned char acerto_status = SHOW_MENU;
static unsigned long int initial; //tempo
static unsigned char segundo;



//static Menu menu_rtc[menuItem]; 

/*
//Iniciar  
void init_managerButton(void){
	
	menu_rtc[0].ptr = (char *)ano;
	menu_rtc[0].value = year_Date();
	menu_rtc[0].ptr_func = inc_Year;
	
	menu_rtc[1].ptr = (char *)mes;
	menu_rtc[1].value = month_Date();
	menu_rtc[1].ptr_func = inc_Month;
	
	menu_rtc[2].ptr = (char *)dia;
	menu_rtc[2].value = day_Date();
	menu_rtc[2].ptr_func = inc_Day;
	
	menu_rtc[3].ptr = (char *)hora;
	menu_rtc[3].value = 0;
	menu_rtc[3].ptr_func = inc_Hour;
	
	menu_rtc[4].ptr = (char *)minuto;
	menu_rtc[4].value = 47;
	menu_rtc[4].ptr_func = inc_Min;

}
	
*/
// Iniciar
//void start_managerDisplay(){
//	button_lcd_state = START;
//}

//maquina de estados
/*void button_lcd(void){
	
	char aux_button;
	
	switch( button_lcd_state ){
		case INIT:
				break;
		case START:
				lcd_backlight(1); //Acender display
				initial = chrono_start();
				button_lcd_state = BUTTON_PRESSED;
				break;
		case BUTTON_PRESSED:
				if( button_pressed() ){
					if( chrono_timeout(initial, TRINTA_S) ){
						button_lcd_state = INIT;
						acerto_rtc();
						return;
					}
				}
				else button_lcd_state = BUTTON_RELEASED;
				break;
		case BUTTON_RELEASED:
				if( chrono_timeout(initial, SESSENTA_S) ){
						lcd_backlight(0);
						button_lcd_state = INIT;
						return;
				}
				else
					if( button_pressed() ){
						//button_read();
						button_lcd_state = START;
					}
				break;
	}
	return;
}*/
				
/*				
void acerto_rtc(rtclock *rtc_clock){
	
	Menu *menu_rover = menu_rtc;
	char *buf = "00"; 
	char *ptr_temp;
	int i;
	
	
	lcd_fill_rectangle(0, 15, 132, 132, WHITE); //Apagar linha
	LCDPutStr("Modo Acerto", 20, 20, BLACK, WHITE);
	
	while(1){
		switch( acerto_status ){
			case SHOW_MENU:
				lcd_fill_rectangle(0, 60, 131, 65, WHITE); //Apagar linha
				LCDPutStr( (*menu_rover).ptr, 60, 0, BLACK, WHITE); // Titulo
				acerto_status = SHOW_VALUE;
				break;
				
			case SHOW_VALUE:
				i = 0;
				
				//LCDPutStr( (*menu_rover).ptr, 60, 0, BLACK, WHITE); // Titulo
				
				//displayNumber((*menu_rover).value, 60, 0);
				
				
				//toString( (*menu_rover).value, buf, 2);
				
				ptr_temp = (*menu_rover).ptr;
				while( *ptr_temp++ != 0x00 ) // Dim da string
					++i;
				displayNumber((*menu_rover).value, 60, DIGIT_SIZE*i);
				
				//LCDPutStr( buf, 60, DIGIT_SIZE*i, BLACK, WHITE);
				acerto_status = BUTTON_RELEASED;
				break;
			case BUTTON_RELEASED:
				if( !button_pressed() ){ //se liberto
					initial = chrono_start();
					acerto_status = BUTTON_PRESSED;
				}
				break;
			case BUTTON_PRESSED:
				if( button_pressed() ){
					//++((*menu_rover).initial_value);
					LCDPutStr( "  ", 60, DIGIT_SIZE*i, BLACK, WHITE);
					
					i = (*menu_rover).value;
					i = (*menu_rover).ptr_func(i);
					(*menu_rover).value = i;
					acerto_status = SHOW_VALUE;
				}
				else
					if( chrono_timeout(initial, UM_S) ){ //conta 1 segundo
						 initial = chrono_start(); // reload
						 ++segundo;
					}
					if( segundo == SESSENTA_S ){ //passaram 60 seg
						segundo = 0;	
						++menu_rover;
						acerto_status = SHOW_MENU;
						if( menu_rover == &menu_rtc[menuItem] ){
							lcd_fill_rectangle(0, 15, 131, 131, WHITE); // Apagar lcd
							LCDPutStr("A Iniciar ...", 20, 20, BLACK, WHITE);
							clockData_Set_sec(rtc_clock, 10);
							clockData_Set_min(rtc_clock, menu_rtc[4].value+1);//+1min tempo que esteve pressionado o botao
							clockData_Set_hour(rtc_clock, menu_rtc[3].value);
							clockData_Set_day(rtc_clock, menu_rtc[2].value);
							clockData_Set_month(rtc_clock, menu_rtc[1].value);
							clockData_Set_year(rtc_clock, menu_rtc[0].value + 2000);
							set_rtc(rtc_clock);
							chrono_delay(DEZ_S);
							lcd_fill_rectangle(0, 15, 131, 131, WHITE); // Apagar lcd
							acerto_status = SHOW_MENU;
							return;
						}
					}
					break;
		}
	}
}	*/	
							
// Converter para character e mostrar no display
int displayNumber(int c, unsigned char pos_x, unsigned char pos_y){

	unsigned int i = 1;
	unsigned int index = 0;
	unsigned int DIV = 0;
	unsigned int RES;
	
	if( c<0 ){ // por o sinal negativo
		LCDPutChar( '-', pos_x, pos_y+DIGIT_SIZE*index, BLACK, WHITE);
		++index;
		c *= -1;
	}
	
	for( DIV=c/10; DIV>0; ++i)
		DIV = DIV / 10;
	
	DIV = index;
	while( i ){ //Escrever
		RES = c%10;
		c /= 10;
		LCDPutChar( RES+'0', pos_x, pos_y+DIGIT_SIZE*(i+index-1), BLACK, WHITE);
		--i;
		++DIV;		
	}
	
	return DIV; // retorna o num de caracteres escritos
}
		






		
/*  *********** Mostrar variaveis no LCD *************** 
 *  ****************************************************
 */

void display_erase(){
	lcd_fill_rectangle(0, 0, 132, 132, WHITE);
	
}

void display_title(){
	//Escrever Titulo
	LCDPutStr( (char *)id, 5, 20, BLACK, WHITE);
	lcd_fill_rectangle(20, DIGIT_SIZE+5, DIGIT_SIZE*(sizeof(id)-1), 2, BLACK);//____
}


// ************************** Etiqueta
void displayMenu_voltage(){
	LCDPutStr( (char *)tensao, U_X, U_Y, BLACK, WHITE);
}

void displayMenu_corrente(){
	LCDPutStr( (char *)corrente, I_X, I_Y, BLACK, WHITE);
}

/*void displayMenu_pFactor(){
	LCDPutStr( (char *)p_factor, PF_X, PF_Y, BLACK, WHITE);
}*/

void displayMenu_consumo(){
	LCDPutStr( (char *)consumo, P_X, P_Y, BLACK, WHITE);
}

void displayMenu_temperatura(){
	LCDPutStr( (char *)temperatura, T_X, T_Y, BLACK, WHITE);
	LCDPutChar( grau, T_X, GRAU_POS_STRING * DIGIT_SIZE, BLACK, WHITE);	
}

void displayMenu_humidade(){
	LCDPutStr( (char *)humidade, HUM_X, HUM_Y, BLACK, WHITE);	
}



// ______________________________-  Apagar
void erase_voltage(){
	//lcd_fill_rectangle(U_Y+DIGIT_SIZE*( sizeof(tensao)-1 ) , U_X, 132, 132, WHITE);
	LCDPutStr( "      ", U_X, U_Y+DIGIT_SIZE*( sizeof(tensao)-1 ), BLACK, WHITE);
}

void erase_current(){
	//lcd_fill_rectangle(P_Y+DIGIT_SIZE*( sizeof(temperatura)-1 ) , P_X, 132, 132, WHITE);
	LCDPutStr( "      ", I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 ), BLACK, WHITE);
}

/*void erase_pFactor(){
	//lcd_fill_rectangle(P_Y+DIGIT_SIZE*( sizeof(temperatura)-1 ) , P_X, 132, 132, WHITE);
	LCDPutStr( "      ", PF_X, PF_Y+DIGIT_SIZE*( sizeof(p_factor)-1 ), BLACK, WHITE);
}*/

void erase_consumo(){
	//lcd_fill_rectangle(P_Y+DIGIT_SIZE*( sizeof(temperatura)-1 ) , P_X, 132, 132, WHITE);
	LCDPutStr( "      ", P_X, P_Y+DIGIT_SIZE*( sizeof(consumo)-1 ), BLACK, WHITE);
}

void erase_temperatura(){
	//lcd_fill_rectangle(T_Y+DIGIT_SIZE*( sizeof(temperatura)-1 ) , T_X, 132, T_X+DIGIT_SIZE, WHITE);
	LCDPutStr( "     ", T_X, T_Y+DIGIT_SIZE*( sizeof(temperatura)-1 ), BLACK, WHITE);
}

void erase_humidade(){
	//lcd_fill_rectangle(HUM_Y+DIGIT_SIZE*( sizeof(humidade)-1 ) , HUM_X, 132, 132, WHITE);
	LCDPutStr( "     ", HUM_X, HUM_Y+DIGIT_SIZE*( sizeof(humidade)-1 ), BLACK, WHITE);
}




// ____________________________ Escrever Valor	
void display_voltage(float v){
	
	int v_int, v_quo;
	
	v_int = v; //inteiro
	//v_quo = v*10;
	//v_quo %= 10;
	
	int num_pos = displayNumber(v_int, U_X, U_Y+DIGIT_SIZE*( sizeof(tensao)-1 ));
	/*if( v_quo ){
		LCDPutChar( '.', U_X, U_Y+DIGIT_SIZE*( sizeof(tensao)-1 ) + DIGIT_SIZE*(num_pos++), BLACK, WHITE);
		displayNumber(v_quo, U_X, U_Y+DIGIT_SIZE*( sizeof(tensao)-1 )+DIGIT_SIZE*( num_pos ));
	}*/
}

void display_current(float v){
	
	int v_int, v_quo;
	
	v_int = v; //inteiro
	int num_pos = displayNumber(v_int, I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 ));
	LCDPutChar( '.', I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 ) + DIGIT_SIZE*(num_pos), BLACK, WHITE);
	++num_pos;
	v -= v_int;
	v_quo = v * 100; //-inteiro
	if( v_quo>0 && v_quo<10 )
		displayNumber(0, I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 )+DIGIT_SIZE*( num_pos++ ));
	displayNumber(v_quo, I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 )+DIGIT_SIZE*( num_pos ));
	
	//v -= v_quo;
	//v_quo = v * 10;
	//v_quo *= 10;
	//v_quo %= 100;
	//displayNumber(v_quo, I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 )+DIGIT_SIZE*( num_pos+2 ));
	
	/*if( v_quo ){
		LCDPutChar( '.', I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 ) + DIGIT_SIZE*(num_pos), BLACK, WHITE);
		displayNumber(v_quo, I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 )+DIGIT_SIZE*( num_pos+1 ));
	}*/
}

/*void display_pFactor(float v){
	
	int v_int, v_quo;
	
	v_int = v; //inteiro
	int num_pos = displayNumber(v_int, PF_X, PF_Y+DIGIT_SIZE*( sizeof(p_factor)-1 ));
	LCDPutChar( '.', PF_X, PF_Y+DIGIT_SIZE*( sizeof(p_factor)-1 ) + DIGIT_SIZE*(num_pos), BLACK, WHITE);
	++num_pos;
	v -= v_int;
	v_quo = v * 10; //-inteiro
	if( v_quo>0 && v_quo<10 )
		displayNumber(0, PF_X, PF_Y+DIGIT_SIZE*( sizeof(p_factor)-1 )+DIGIT_SIZE*( num_pos++ ));
	displayNumber(v_quo, PF_X, PF_Y+DIGIT_SIZE*( sizeof(p_factor)-1 )+DIGIT_SIZE*( num_pos ));
	
	//v -= v_quo;
	//v_quo = v * 10;
	//v_quo *= 10;
	//v_quo %= 100;
	//displayNumber(v_quo, I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 )+DIGIT_SIZE*( num_pos+2 ));
	
	if( v_quo ){
		LCDPutChar( '.', I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 ) + DIGIT_SIZE*(num_pos), BLACK, WHITE);
		displayNumber(v_quo, I_X, I_Y+DIGIT_SIZE*( sizeof(corrente)-1 )+DIGIT_SIZE*( num_pos+1 ));
	}
}*/

//Escrever temperatura	
void display_temperatura(char t_int, unsigned char t_quo){

	int num_pos = displayNumber(t_int, T_X, T_Y+DIGIT_SIZE*( sizeof(temperatura)-1 ) );
	if( t_quo ){
		LCDPutChar( '.', T_X, T_Y+DIGIT_SIZE*( sizeof(temperatura)-1 )+DIGIT_SIZE*(num_pos), BLACK, WHITE);
		displayNumber(t_quo, T_X, T_Y+DIGIT_SIZE*( sizeof(temperatura)-1 )+DIGIT_SIZE*( num_pos+1 ));
	}
}


//Humidade
void display_humidade(unsigned char h_int, unsigned char h_quo){
	
	int num_pos = displayNumber(h_int, HUM_X, HUM_Y+DIGIT_SIZE*( sizeof(humidade)-1 ) );
	if( h_quo ){
		LCDPutChar( '.', HUM_X, HUM_Y+DIGIT_SIZE*( sizeof(humidade)-1 )+DIGIT_SIZE*(num_pos), BLACK, WHITE);
		displayNumber(h_quo, HUM_X, HUM_Y+DIGIT_SIZE*( sizeof(humidade)-1)+DIGIT_SIZE*( num_pos+1 ));
	}
}
		
//Consumo
void display_consumo(float v){
	
	int v_int, v_quo;
	
	v_int = v; //inteiro
	
	int num_pos = displayNumber(v_int, P_X, P_Y+DIGIT_SIZE*( sizeof(consumo)-1 ));
	LCDPutChar( '.', P_X, P_Y+DIGIT_SIZE*( sizeof(consumo)-1 ) + DIGIT_SIZE*(num_pos), BLACK, WHITE);
	v -= v_int;
	v_quo = v * 10; //-inteiro
	displayNumber(v_quo, P_X, P_Y+DIGIT_SIZE*( sizeof(consumo)-1 )+DIGIT_SIZE*( num_pos+1 ));
	/*v_quo = v*100;
	v_quo %= 100;
	
	int num_pos = displayNumber(v_int, P_X, P_Y+DIGIT_SIZE*( sizeof(consumo)-1 ));
	if( v_quo ){
		LCDPutChar( '.', P_X, P_Y+DIGIT_SIZE*( sizeof(consumo)-1 ) + DIGIT_SIZE*(num_pos), BLACK, WHITE);
		displayNumber(v_quo, P_X, P_Y+DIGIT_SIZE*( sizeof(consumo)-1 )+DIGIT_SIZE*( num_pos+1 ));
	}*/
}	



	
//Data
/*void aux_display_data(char *ptr, unsigned char value){
	unsigned char DIV, RES;
	
	DIV = value/10;
	RES = value%10;
	*ptr++ = DIV+'0';
	*ptr = RES + '0';
}*/

/*
void display_data(unsigned char ano_mes, unsigned char dia, unsigned char hora, unsigned char min){
	char dsp[]="DD/MM/AA  HH:MM";
	//int aux;
	
		aux_display_data( dsp, dia);
		aux_display_data( dsp+3, ano_mes&0x0F);
		aux_display_data( dsp+6, ano_mes>>4);
		aux_display_data( dsp+10, hora);
		aux_display_data( dsp+13, min);
		
		LCDPutStr( dsp, DATA_X, DATA_Y, BLACK, WHITE);
	
		/*aux=displayNumber(dia, DATA_X, DATA_Y);
		LCDPutChar( '/', DATA_X, DATA_Y+DIGIT_SIZE*aux, BLACK, WHITE);
		++aux;
		aux += displayNumber(ano_mes&0x0F, DATA_X, DATA_Y+DIGIT_SIZE*aux);
		LCDPutChar( '/', DATA_X, DATA_Y+DIGIT_SIZE*aux, BLACK, WHITE);
		++aux;
		aux += displayNumber(ano_mes>>4, DATA_X, DATA_Y+DIGIT_SIZE*aux);
		LCDPutChar( ' ', DATA_X, DATA_Y+DIGIT_SIZE*aux, BLACK, WHITE);
		++aux;
		LCDPutChar( ' ', DATA_X, DATA_Y+DIGIT_SIZE*aux, BLACK, WHITE);
		++aux;
	//aux += 2; //Escrever espaco
		aux += displayNumber(hora, DATA_X, DATA_Y+DIGIT_SIZE*aux);
		LCDPutChar( ':', DATA_X, DATA_Y+DIGIT_SIZE*aux, BLACK, WHITE);
		++aux;
		displayNumber(min, DATA_X, DATA_Y+DIGIT_SIZE*aux);*/
//}
	
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		

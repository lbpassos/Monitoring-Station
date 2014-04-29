#include "button.h"
#include "lpc2106.h"
#include "buffer.h"

#define maxButton	1 // Num maximo de botoes
#define debounceTime	20 //ms
//#define maskPort_Button	0x03 // botoes nos portos P01 e P02

#define dim_Fifo	1// num de teclas armazenadas

#define PRESSED			1	
#define RELEASED		0

#define testButton_Pressed	0
#define pressedDebounce		1
#define testButton_Released	2
#define releaseDebounce		3

#define button_decode		0
#define	wait_for_button		1
#define	wait_for_release	2


/* Definicao dos portos a utilizar */
#define button_15	15

typedef struct _button{
	unsigned int port; // porto GPIO
	//unsigned int status; // PRESSED ou RELEASED
	unsigned int id; // nome do botao
} Button;
/* **************************************** */


//static int button[maxButton];//Numero de botoes: Premido ou Liberto
static Button buttons[maxButton];

static unsigned char buttonStatus_Pressed;//controlador da maquina de estados
static unsigned char buttonStatus_Read;//controlador da maquina de estados

static unsigned int initial;//tempo
static int actualButton_Pressed;
static FifoNode fifo; //Fifo
static int buffer_Fifo[dim_Fifo];//buffer para as teclas


void button_init(){
	Button b1;
	b1.port	= button_15;
	b1.id = 1;
	buttons[0] = b1;
	

	newBuffer(&fifo, dim_Fifo, buffer_Fifo);
	//PINSEL0 = 0x00; // selecionar P0.0 -> P0.15
	int i; 
	uint32_t  pinsel = PINSEL0;
	uint32_t  iodir = IODIR;
	for(i=0; i<maxButton; ++i){
		pinsel &= ~( 3<<(buttons[i].port*2) );
		iodir &= ~( 1<<(buttons[i].port) );
	}
	PINSEL0 = pinsel;
	IODIR = iodir;
}
	
	
/* Retorna o estado de um determinado botão (button_code)  */
int button_state(int button_code){
	uint32_t tmp;
	tmp = IOPIN;
	return ( ~(tmp>>buttons[button_code].port) & 0x01 );
}

/* Pesquisa de tecla */
int button_pressed(){
	int i;
	switch(buttonStatus_Pressed){
		case testButton_Pressed: // Testar se existe alguma tecla premida
						for(i=0; i<maxButton; ++i)
							if( button_state(i) ){
								buttonStatus_Pressed = pressedDebounce;
								initial = chrono_start();
								actualButton_Pressed = i;
								break;
							}
						break;
		case pressedDebounce: // Esperar tempo (debounce)
						if(chrono_timeout(initial, debounceTime)){
							if( button_state(actualButton_Pressed) ){
								write(&fifo, buttons[actualButton_Pressed].id);// escreve no fifobuffer
								buttonStatus_Pressed = testButton_Released;
								return 1; //botao premido
							}
							buttonStatus_Pressed = testButton_Pressed; // Nao ha tecla premida
						}
						break;						
		case testButton_Released: // verificar se botao continua pressionado
						if( !button_state(actualButton_Pressed) ){
							initial = chrono_start();
							buttonStatus_Pressed = releaseDebounce;
						}
						//return 1; //botao continua pressionado
						break;
		case releaseDebounce: // Debounce (botao largado)
						if(chrono_timeout(initial, debounceTime) ){
							if( button_state(actualButton_Pressed) ){
								buttonStatus_Pressed = testButton_Released; // continua premido
								//return 1;
							}
							else buttonStatus_Pressed = testButton_Pressed;
						}
						//else return 1;
						break;
		default:
						return 0;
	}
	return 0;
}

/* Ler tecla do buffer */
int button_read(){
	int tecla=read(&fifo);
	if(tecla) return tecla;// existe tecla
	while( !button_pressed() ); // espera por tecla
	
	while( buttonStatus_Pressed != testButton_Pressed ) button_pressed();
	tecla = read(&fifo);
	return tecla; 
}

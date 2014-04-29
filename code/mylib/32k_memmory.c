#include "32k_memmory.h"
#include "i2c.h"
#include "lpc2106.h"

#define NUM_ofMODULES		1
#define MODULE_SIZE			32*1024 
#define PAGE_SIZE			64

#define INIT		0
#define WAIT		1
#define DELAY_5ms	2


static unsigned int mem_size; //tamanho da memoria
static long int start_timer; // valor do timer
static unsigned char module[NUM_ofMODULES]; // end. de cada modulo
static unsigned char buffer[3]; //add_H  add_L  data
static unsigned char write_status; // maquina de estados de escrita
static unsigned char read_status; //maquina de estados leitura



/* Iniciar modulo
 * Parametros:
 * 				ENTRADA: -
 * 				  SAIDA: -
 */ 
void init_memmory(){
	int i = 0;
	while( i<NUM_ofMODULES ){
		module[i] = 0x50 | i; // end. do modulo de memoria
		mem_size += MODULE_SIZE; // dim total
		++i;
	}
	mem_size -= PAGE_SIZE; // Tirar 64 bytes para guardar os ptrs
	chrono_delay(1);
}

/* Dimensao da memoria
 * Parametros:
 * 				ENTRADA: -
 * 				  SAIDA: Tamanho da memoria
 */ 
unsigned int size_memmory(void){ return mem_size; }
	

/* Escrita
 * Parametros:
 * 				ENTRADA: addr - end.
 * 						 data - 8 bits
 * 				  SAIDA: 0 - Em processamento
 * 						 1 - Terminado
 * 						-1 - Erro
 */ 
int write_memmory(unsigned int addr, unsigned char data){
	int real_address;
	unsigned char *module_ptr;
	
	if( addr>=0 && addr<PAGE_SIZE ) 
		return -1; // Nao gravar no end.
	
	switch(write_status){
		case INIT:
				real_address = addr;
				module_ptr = module;
				while( real_address >= MODULE_SIZE ){
					real_address -= MODULE_SIZE;
					++module_ptr;
				}
				/* construcao da trama de escrita */	
				buffer[0] = real_address>>8;
				buffer[1] = real_address&0xFF;
				buffer[2] = data;
				i2c_set( *module_ptr, 0);
				i2c_comm(buffer, 3, 0x00, 0);
				write_status = WAIT;
				break;
		case WAIT:
				if( i2c_status() ){
					start_timer = chrono_start();
					write_status = DELAY_5ms; // Aguardar tempo de escrita 5ms
				}
				break;
		case DELAY_5ms:
				if( chrono_timeout( start_timer, 5) ){
					write_status = INIT;
					return 1; //Escrita terminada
				}
				break;
		default:
				return -1;
	}
	return 0;
}


/* Escreve dados numa pagina de 64 bytes (Modulo 1)
 * Parametros:
 * 				ENTRADA: data - dois primeiros bytes devem estar
 * 								inicializados a 0: 0 0 dados
 * 						 dim - dimensao de data
 * 				  SAIDA: -
 */ 
void writePage_memmory(unsigned char page, unsigned char addr, unsigned char *data, unsigned char dim){

	*data++ = page>>2;
	*data-- = (page&0x03)<<6 | addr&0x3F;
	i2c_set( module[0], 0); // Seleccionar modulo 1
	i2c_comm(data, dim, 0x00, 0);
	while( !i2c_status() );
	chrono_delay(5);

}

					
/* Leitura de dados
 * Parametros:
 * 				ENTRADA: addr - end.
 * 						 data - dados
 * 						 dim  - dimensao de dados
 * 				  SAIDA: 0 - Em processamento
 * 						 1 - Terminado
 */ 							
int read_memmory(unsigned int addr, unsigned char *data, int dim){

	unsigned int real_address;
	unsigned char *module_ptr;
	
	switch(read_status){
		case INIT:
				real_address = addr;
				module_ptr = module;
				while( real_address >= MODULE_SIZE ){
					real_address -= MODULE_SIZE;
					++module_ptr;
				}
				buffer[0] = real_address>>8;
				buffer[1] = real_address&0xFF;
				i2c_set( *module_ptr, 0x02);
				i2c_comm(buffer, 2, data, dim);
				read_status = WAIT;
				break;
		case WAIT:
				if( i2c_status() ){
					read_status = INIT;
					return 1;
				}
				break;
		default:
				return -1;
	}
	return 0;
}
				

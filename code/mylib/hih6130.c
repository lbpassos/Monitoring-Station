#include "hih6130.h"
#include "i2c.h"
#include "lpc2106.h"

#define DIM_DATA	4

#define RESOLUTION	16383 //2^14 -1


#define IDDLE			0
#define INIT			1
#define TEST_INIT		2
#define INIT_MEASURE	3
#define MEASURE_READY	4
#define	COMPLETE		5

static unsigned char hih_state = IDDLE;
static unsigned char data[DIM_DATA];
static unsigned int start_timer;



void initMeasure_hih6130(){
	hih_state = INIT;
}

int status_hih30(){
	if( hih_state==COMPLETE )
		return 1;
	return 0;
}

void endMeasure_hih6130(){
	hih_state = IDDLE;
}

int hum_hih6130(char *humidity){
	int tmp, aux;
	if( hih_state==COMPLETE ){
		tmp = data[0]&0x3F;
		tmp <<= 8;
		tmp |= data[1];
		
		*(humidity++) = (tmp*100)/RESOLUTION;
		aux = (tmp*100)%RESOLUTION;
		aux *=10;
		aux /= RESOLUTION;
		
		*humidity = aux; 
		
		return 1;
	}
	return 0;
}

int temp_hih6130(char *temp){
	int tmp, tmp_1;
	
	if( hih_state==COMPLETE ){
		tmp = data[2];
		tmp <<= 8;
		tmp |= data[3];
		tmp >>= 2;
				
		*(temp++) = (tmp*165)/RESOLUTION-40;
		tmp_1 = (tmp*165)%RESOLUTION;
		tmp_1 *= 10; // Uma casa decimal de precisao
		tmp_1 /= RESOLUTION;
		*temp = tmp_1;
		
		return 1;
	}
	return 0;
}
	
/* Maquina de Estados do sensor */
void hih6130(){
	switch(hih_state){
		case IDDLE:
				break;
		case INIT:
				i2c_set(0x27, 0); //end do sensor
				i2c_comm(0x00, 0, 0x00, 0);
				hih_state = TEST_INIT;
				break;
		case TEST_INIT:
				if( i2c_status() ){
					start_timer = chrono_start();
					hih_state = INIT_MEASURE;
				}
				break;
		case INIT_MEASURE:
				if( chrono_timeout(start_timer, 40) ){
					i2c_set(0x27, 1);
					i2c_comm(0x00, 0, data, DIM_DATA);
					hih_state = MEASURE_READY;
				}
				break;
		case MEASURE_READY:
				if( i2c_status() )
					hih_state = COMPLETE;
				break;
		case COMPLETE:
				break;
		default:
				break;
	}
}
				
				
				
				
				
				
				
				
				

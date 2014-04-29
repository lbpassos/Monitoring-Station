#include "spi.h"
#include "lpc2106.h"

#define SCK_PIN		4
#define MISO_PIN	5
#define MOSI_PIN	6

void spi_transaction_begin(Spi_device *spi)__attribute__((section(".text.fastcode")));
void spi_transaction_end(Spi_device * spi)__attribute__((section(".text.fastcode")));
void spi_transaction_transfer(Spi_device *device, unsigned int size, unsigned char *tx_data, unsigned char *rx_data)__attribute__((section(".text.fastcode")));


static inline void CLK(int value){
	if(value)
		IOSET = 1 << SCK_PIN;
	else
		IOCLR = 1 << SCK_PIN;
}

static inline void MOSI(unsigned long int data){
	if(data & 1)
		IOSET = 1 << MOSI_PIN;
	else
		IOCLR = 1 << MOSI_PIN;
}

static inline int MISO(){
	return (IOPIN >> MISO_PIN) & 1;
}

void spi_init(){
	unsigned long int pinsel = PINSEL0;
	//Selecionar GPIO: P0.4  P0.5   P0.6
	PINSEL0 = pinsel & (~(0x3 << (SCK_PIN<<1))) & (~(0x3 << (MISO_PIN<<1))) & (~(0x3 << (MOSI_PIN<<1)));
	pinsel = IODIR;
	//Selecionar como OUT: P0.4 e P0.6         IN: P0.5
	IODIR = (pinsel & (~(1<<MISO_PIN))) | (1<<SCK_PIN) | (1<<MOSI_PIN);
}

void spi_transaction_begin(Spi_device *spi){
	CLK(spi->cpol);
	spi->cs(1);
}

void spi_transaction_end(Spi_device * spi){
	spi->cs(0);
}


void spi_transaction_transfer(Spi_device *device, unsigned int size, unsigned char *tx_data, unsigned char *rx_data){
	if(size == 0 ) return;
	
	int nbits = device->cbits;
	int step = (device->cbits+7)/8;
	int clock_ph0 = device->cpha != device->cpol;
	int clock_ph1 = !clock_ph0;
	while(size-- > 0){
		register unsigned long data_out, data_in;
		data_out = data_in = 0;
		if(tx_data != 0){
			if(step == 2)
				data_out = *(unsigned short*)tx_data;
				else 
					if(step > 2)
						data_out = *(unsigned long*)tx_data;
						else
							data_out = *(unsigned char*)tx_data;
		}
		if(device->lsbf){
			int i;
			for(i=0; i<nbits; ++i){
				CLK(clock_ph0);
				MOSI(data_out);
				CLK(clock_ph1);
				data_in |= MISO() << i;
				data_out >>= 1;
			}
		}
		else {
			int i;
			for(i=0; i<nbits; ++i){
				CLK(clock_ph0);
				MOSI(data_out >> (nbits -1 -i));
				CLK(clock_ph1);
				data_in = (data_in << 1) | MISO();
			}
		}
		if(rx_data != 0){
			if(step == 2)
				*(unsigned short*)rx_data = data_in;
				else 
					if(step > 2)
						*(unsigned long*)rx_data = data_in;
						else
							*(unsigned char*)rx_data = data_in;
				rx_data += step;
		}
		if(tx_data != 0)
			tx_data += step;
	}
	CLK(device->cpol);
}
						
					
					
					
	
	
	 

#include "wiznet.h"
#include "spi.h"
#include "lpc2106.h"


#define RESET_WIZpin	0
#define	INT_WIZpin		16 //no PINSEL1 porto 16
#define	CS_WIZpin		13

static Spi_device spi;


static void cs_set(int value){
	if(value)
		IOCLR = 1<<CS_WIZpin;
	else
		IOSET = 1<<CS_WIZpin;
}


void wiznet_init() {
	/* direcionar os pinos de CS, RESET e IRQ para o GPIO */
	uint32_t pinsel = PINSEL0;
	uint32_t pinsel_1 = PINSEL1;
	
	PINSEL0 = pinsel & ~(3 << CS_WIZpin * 2) & ~(3 << RESET_WIZpin * 2);
	PINSEL1 = pinsel_1 & ~(3 << (INT_WIZpin-16) * 2);
	
	/* desactivar os sinais; são todos active low */
	IOSET = 1 << CS_WIZpin | 1 << RESET_WIZpin;
	/* e programá-los como saídas excepto IRQ*/
	
	uint32_t iodir = IODIR;
	IODIR = iodir | 1 << CS_WIZpin | 1 << RESET_WIZpin & ~(1 << INT_WIZpin);

	
	//spi.clock = 10;
	
	spi.cpha = 0;
	spi.cpol = 0;
	spi.cbits = 8;
	spi.lsbf = 0; // MSB first
	spi.cs = cs_set;
	
	/*spi = {
		10,					//SCLK
		0,					//CPHA
		0,					//CPOL
		9,					//Numero de bits por palavra
		0,					//LSBF
		cs_set				
	};*/
	
}


//Escrever comando
void write_wiznet(unsigned int addr, unsigned char data){
	uint8_t out_buffer[4];
	
	out_buffer[0] = 0xF0;
	out_buffer[1] = addr>>8;
	out_buffer[2] = addr&0xFF;
	out_buffer[3] = data;
	
	spi_transaction_begin(&spi);
	spi_transaction_transfer(&spi, 4, out_buffer, 0x00);
	spi_transaction_end(&spi);

}

//Ler do modulo
unsigned char read_wiznet(unsigned int addr){
	uint8_t out_buffer[4];
	uint8_t in_buffer[4];
	
	out_buffer[0] = 0x0F;
	out_buffer[1] = addr>>8;
	out_buffer[2] = addr&0xFF;
	out_buffer[3] = 0x00;

	spi_transaction_begin(&spi);
	spi_transaction_transfer(&spi, 4, out_buffer, in_buffer);
	spi_transaction_end(&spi);

	return in_buffer[3];
}

//fazer RESET
void reset_wiznet(){
	
	IOCLR = 1<<RESET_WIZpin;
	chrono_delay(1);
	IOSET = 1<<RESET_WIZpin;
	chrono_delay(10);
}

/* ******************************* REDE
 * 
 */
 
//Definir MAC_add
void setMAC_wiznet(unsigned char *add){

	int i;
	for( i=0; i<6; ++i)
		 write_wiznet( SHAR0+i, *(add+i) );
}

//Definir IP add
void setIP_wiznet(unsigned int add){
	int i;
	
	for(i=0; i<4; ++i)
		write_wiznet( SIPR0+i, (add>>(24-8*i))&0xFF );
}

//Definir mask
void setMASK_wiznet(unsigned int mask){
	int i;
	
	for(i=0; i<4; ++i)
		write_wiznet( SUBR0+i, (mask>>(24-8*i))&0xFF );
}

//definir ip gateway	
void setGATEWAY_wiznet(unsigned int add){
	int i;
	
	for(i=0; i<4; ++i)
		write_wiznet( GAR0+i, (add>>(24-8*i))&0xFF );
}

/* ******************************* INTERRUPTS
 * 
 */

//Permitir Interrupts
void unmaskIRQ_wiznet(unsigned char value){
	unsigned char tmp;
	
	tmp = read_wiznet( IMR );
	tmp |= 1<<value;
	write_wiznet( IMR, tmp );
}

//Inibir Interrupts
void maskIRQ_wiznet(unsigned char value){
	unsigned char tmp;
	
	if( value == IM_Disable ) // Desinibir todos
		tmp = 0x00;
	else{
		tmp = read_wiznet( IMR );
		tmp &= 0<<value;
	}
	write_wiznet( IMR, tmp );
}

//Verificar causa dos interrupts
unsigned char causeIRQ_wiznet(){
	unsigned char tmp, i;
	
	tmp = read_wiznet( IR );
	i=0;
	do{
		if(tmp&0x01)
			break;
		tmp >>= 1;
		++i;
	}
	while(i<8);
	
	//write_wiznet( IR, 1<<i ); // Limpar interrupt
	return i;
}
			
//Verificar se existe interrupt
unsigned char IRQ_wiznet(){
	unsigned int pin_ = IOPIN;
	
	if( (pin_ >> INT_WIZpin)&0x01 ) return 0;
	return 1;
}

// Tipo de Interrupt no socket
unsigned char SnIRQ_wiznet(Socket s, unsigned char type_irq){
	
	unsigned char irq_tmp;
	
	irq_tmp = read_wiznet( S0_IR+0x100*s );
	write_wiznet( S0_IR+0x100*s, irq_tmp ); //Limpar irq
	if( (irq_tmp>>type_irq)&0x01 )
		irq_tmp = 1;
	else
		irq_tmp = 0;

	return irq_tmp;
}


/* *************************** SOCKETS
 * 
 */
 
//Status do socket
unsigned char getSnStatus_wiznet(Socket s){
	return read_wiznet( S0_SR+0x100*s );
}
 
//Tamanho do buffer do Socket_TX
unsigned int getSn_TXsize_wiznet(Socket s){
	unsigned int sz;
	sz = read_wiznet( S0_TX_FSR+0x100*s );
	sz <<= 8;
	sz |= read_wiznet( S0_TX_FSR+0x100*s+1 );
	return sz;
}

// Obter Tamanho dos dados do socket_RX
unsigned int getSn_RXsize_wiznet(Socket s){
	unsigned int sz;
	sz = read_wiznet( S0_RX_RSR+0x100*s );
	sz <<= 8;
	sz |= read_wiznet( S0_RX_RSR+0x100*s+1 );
	return sz;
}

// Definir MAC de destino
void setSn_DestMAC_wiznet(Socket s, unsigned char *addr){
	int i;
	for( i=0; i<6; ++i)
		 write_wiznet( S0_DHAR+i+0x100*s, *(addr+i) );
}

// Definir IP de destino
void setSn_DestIP_wiznet(Socket s, unsigned int addr){
	int i;
	for( i=0; i<4; ++i)
		 write_wiznet( S0_DIPR+i+0x100*s, (addr>>(24-8*i))&0xFF );
}

// Definir PORTO de destino
void setSn_DestPORT_wiznet(Socket s, unsigned int addr){
	int i;
	for( i=0; i<2; ++i)
		 write_wiznet( S0_DPORT+i+0x100*s, (addr>>(8-8*i))&0xFF );
}

//Definir protocolo do socket
void setSnType_wiznet(Socket s, unsigned char val){
	 write_wiznet( S0_MR+0x100*s, val );
}	

//Escrever no buffer de TRX
void sendData_wiznet(Socket s, unsigned char *data, unsigned int len){
	
	unsigned int ptr_data, realPtr_data, i, j;
	
	ptr_data = read_wiznet(S0_TX_WR+0x100*s);
	ptr_data <<= 8;
	ptr_data |= read_wiznet(S0_TX_WR+0x100*s+1);
	//ptr_data &= MASK_S0;
	
	realPtr_data = (ptr_data&MASK_S0) + BASE_ADD_TX_MEMMORY; 
	
	if( realPtr_data+len > (BASE_ADD_TX_MEMMORY+SIZE_TX_RX_Sn*s+SIZE_TX_RX_Sn-1) ){ // buffer overflow
		for( i=0; i<(BASE_ADD_TX_MEMMORY+SIZE_TX_RX_Sn*s+SIZE_TX_RX_Sn-realPtr_data); ++i )
			write_wiznet( realPtr_data+i, *(data++) );
		--i;
		for( j=0; j<(len-i); ++j)
			write_wiznet( BASE_ADD_TX_MEMMORY+j, *(data++) );
	}
	else
		for( i=0; i<len; ++i)
			write_wiznet(realPtr_data+i, *(data++) );

	ptr_data += len; //Incrementar Sn_TX_WR

	/*if( ptr_data >= SIZE_TX_RX_Sn )
		ptr_data -= SIZE_TX_RX_Sn-1;
	*/
	write_wiznet( S0_TX_WR+0x100*s, (unsigned char)(ptr_data>>8) );
	write_wiznet( S0_TX_WR+0x100*s+1, (unsigned char)(ptr_data) );
}

// Ler dados recebidos
void recvData_wiznet(Socket s, unsigned char *data, unsigned int len){
	
	unsigned int ptr_data, realPtr_data, i, j;
	
	ptr_data = read_wiznet(S0_RX_RD+0x100*s);
	ptr_data <<= 8;
	ptr_data |= read_wiznet(S0_RX_RD+0x100*s+1);
	
	//ptr_data &= MASK_S0;
	realPtr_data = (ptr_data&MASK_S0) + BASE_ADD_RX_MEMMORY; 
	
	if( realPtr_data+len > (BASE_ADD_RX_MEMMORY+SIZE_TX_RX_Sn*s+SIZE_TX_RX_Sn-1) ){ // buffer overflow
		for( i=0; i<(BASE_ADD_RX_MEMMORY+SIZE_TX_RX_Sn*s+SIZE_TX_RX_Sn-realPtr_data); ++i )
			*(data++) = read_wiznet(realPtr_data+i);
		--i;
		for( j=0; j<(len-i); ++j)
			*(data++) = read_wiznet(BASE_ADD_RX_MEMMORY+j);
	}
	else
		for( i=0; i<len; ++i)
			*(data++) = read_wiznet(realPtr_data+i);

	ptr_data += len; //Incrementar Sn_RX_RD
	/*if( ptr_data >= SIZE_TX_RX_Sn )
		ptr_data -= SIZE_TX_RX_Sn-1;
	*/	
	write_wiznet( S0_RX_RD+0x100*s, (unsigned char)(ptr_data>>8) );
	write_wiznet( S0_RX_RD+0x100*s+1, (unsigned char)(ptr_data) );
	write_wiznet( S0_CR+0x100*s, Sn_CR_RECV ); //Enviar net
}

/* **************************** UDP *********************
 * 
 */
 
// Abrir um socket UDP
void createUDP_socket(Socket s, unsigned int port){
	
	setSnType_wiznet( s, UDP ); // protocolo UDP
	write_wiznet( S0_PORT+0x100*s, (unsigned char)(port>>8) ); //porto
	write_wiznet( S0_PORT+0x100*s+1, (unsigned char)port );
	write_wiznet( S0_CR+0x100*s, Sn_CR_OPEN ); //Abrir	
}
	
// Fechar socket
void close_socket(Socket s){
	write_wiznet( S0_CR+0x100*s, Sn_CR_CLOSE );
}

//Enviar dados para o socket
void sendtoUDP_socket(Socket s, unsigned char *src_buf, unsigned int len, unsigned int addr, unsigned int port){
	
	setSn_DestIP_wiznet( s, addr ); // IP destino
	setSn_DestPORT_wiznet( s, port ); // PORTO destino
	sendData_wiznet( s, src_buf, len); //Enviar para o buffer
	write_wiznet( S0_CR+0x100*s, Sn_CR_SEND ); //Enviar net
	
}




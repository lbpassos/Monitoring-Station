// posicoes de memoria
#define BASE_ADD_TX_MEMMORY			0x4000	//End base da memoria de TX
#define BASE_ADD_RX_MEMMORY			0x6000	//End base da memoria de RX
#define SIZE_TX_RX_Sn				0x0800	//2K para cada Sn (TX e RX)
#define	MASK_S0						0x07FF	//Mascara para bloco 2K

// 	registos
#define GAR0		0x0001	// ip gateway address
#define SUBR0		0x0005	// Subnet mask
#define	SHAR0		0x0009	// MAC_ADDRESS REGISTER
#define SIPR0		0x000F	// IP_ADDRESS Register
#define IR			0x0015	// Interrupt Register
#define IMR			0x0016	// Interrupt Mask Register
#define S0_MR		0x0400	// Socket protocol
#define S1_MR		0x0500	// Socket protocol
#define S2_MR		0x0600	// Socket protocol
#define	S3_MR		0x0700	// Socket protocol
#define S0_CR		0x0401	// Socket 
#define S1_CR		0x0501	// Socket
#define S2_CR		0x0601	// Socket
#define	S3_CR		0x0701	// Socket
#define S0_PORT		0x0404	// PORTO Local do socket 0
#define S1_PORT		0x0504	// PORTO Local do socket 1
#define S2_PORT		0x0604	// PORTO Local do socket 2
#define	S3_PORT		0x0704	// PORTO Local do socket 3
#define S0_IR		0x0402	// Interrupts do Socket 0
#define S1_IR		0x0502	// Interrupts do Socket 1
#define	S2_IR		0x0602	// Interrupts do Socket 2
#define	S3_IR		0x0702	// Interrupts do Socket 3
#define S0_SR		0x0403	// Estado do Socket 0
#define S1_SR		0x0503	// Estado do Socket 1
#define	S2_SR		0x0603	// Estado do Socket 2
#define	S3_SR		0x0703	// Estado do Socket 3
#define	S0_TX_FSR	0x0420	// Tamanho do buffer de TX do socket 0 
#define	S1_TX_FSR	0x0520	// Tamanho do buffer de TX do socket 1 
#define	S2_TX_FSR	0x0620	// Tamanho do buffer de TX do socket 2 
#define	S3_TX_FSR	0x0720	// Tamanho do buffer de TX do socket 3 
#define	S0_RX_RSR	0x0426	// Tamanho do buffer de RX do socket 0 
#define	S1_RX_RSR	0x0526	// Tamanho do buffer de RX do socket 1 
#define	S2_RX_RSR	0x0626	// Tamanho do buffer de RX do socket 2 
#define	S3_RX_RSR	0x0726	// Tamanho do buffer de RX do socket 3 
#define	S0_DHAR		0x0406	// MAC de destino do socket 0 
#define	S1_DHAR		0x0506	// MAC de destino do socket 1 
#define	S2_DHAR		0x0606	// MAC de destino do socket 2 
#define	S3_DHAR		0x0706	// MAC de destino do socket 3 
#define	S0_DIPR		0x040C	// IP de destino do socket 0 
#define	S1_DIPR		0x050C	// IP de destino do socket 1 
#define	S2_DIPR		0x060C	// IP de destino do socket 2 
#define	S3_DIPR		0x070C	// IP de destino do socket 3 
#define	S0_DPORT	0x0410	// PORTO de destino do socket 0 
#define	S1_DPORT	0x0510	// PORTO de destino do socket 1 
#define	S2_DPORT	0x0610	// PORTO de destino do socket 2 
#define	S3_DPORT	0x0710	// PORTO de destino do socket 3 
#define	S0_RX_RD	0x0428	// Localizacao dos dados para leitura do socket 0 
#define	S1_RX_RD	0x0528	// Localizacao dos dados para leitura do socket 1 
#define	S2_RX_RD	0x0628	// Localizacao dos dados para leitura do socket 2 
#define	S3_RX_RD	0x0728	// Localizacao dos dados para leitura do socket 3 
#define	S0_TX_WR	0x0424	// Localizacao dos dados para ESCRITA do socket 0 
#define	S1_TX_WR	0x0524	// Localizacao dos dados para ESCRITA do socket 1 
#define	S2_TX_WR	0x0624	// Localizacao dos dados para ESCRITA do socket 2 
#define	S3_TX_WR	0x0724	// Localizacao dos dados para ESCRITA do socket 3


//Tipos de Interrupts Globais
#define	IM_IR0		0	// Socket 0
#define IM_IR1		1	// Socket 1
#define	IM_IR2		2	// Socket 2
#define IM_IR3		3	// Socket 3
#define IM_IR5		5	// PPPoE
#define IM_IR6		6	// Destination Unreachable
#define IM_IR7		7	// IP conflict Enable
#define IM_Disable	8	// Disable all

//Tipos de Interrupts no SOCKET
#define SOCK_IRQ_CON		0
#define SOCK_IRQ_DISCON		1
#define SOCK_IRQ_RECV		2
#define SOCK_IRQ_TIMEOUT	3
#define SOCK_IRQ_SEND_OK	4


//Estados dos Sockets
#define SOCK_CLOSED			0x00
#define SOCK_INIT			0x13
#define SOCK_LISTEN			0x14
#define SOCK_ESTABLISHED	0x17
#define SOCK_CLOSE_WAIT		0x1C
#define SOCK_UDP			0x22
#define	SOCK_IPRAW			0x32
#define	SOCK_MACRAW			0x42
#define	SOCK_PPPOE			0x5F
#define	SOCK_SYNSENT		0x15
#define	SOCK_SYNRECV		0x16
#define SOCK_FIN_WAIT		0x18
#define SOCK_CLOSING		0x1A
#define SOCK_TIME_WAIT		0x1B
#define	SOCK_LAST_ACK		0x1D
#define SOCK_ARP			0x11

//Valores de Sn_CR: controlador do socket
#define Sn_CR_OPEN			0x01		
#define Sn_CR_LISTEN		0x02		
#define Sn_CR_CONNECT		0x04		
#define Sn_CR_DISCON		0x08		
#define Sn_CR_CLOSE			0x10		
#define Sn_CR_SEND			0x20		
#define Sn_CR_SEND_MAC		0x21		
#define Sn_CR_SEND_KEEP		0x22
#define Sn_CR_RECV			0x40	



#define BASE_ADD_RX_MEMMORY			0x6000	//End base da memoria de RX
#define BASE_ADD_TX_MEMMORY			0x4000	//End base da memoria de TX

//Protocolo
#define UDP					0x02

typedef unsigned char Socket;



void wiznet_init(); //Init modulo
void reset_wiznet(); //Reset
void write_wiznet(unsigned int addr, unsigned char data); //Escrever comando
unsigned char read_wiznet(unsigned int addr); //Ler 


// Network
void setMAC_wiznet(unsigned char *ptr); //definir mac_address
void setIP_wiznet(unsigned int add); //definir ip_address
void setMASK_wiznet(unsigned int mask); //definir mask_address
void setGATEWAY_wiznet(unsigned int add); // definir ip gateway

// Interrupts
void unmaskIRQ_wiznet(unsigned char value); // permitir interrupts
void maskIRQ_wiznet(unsigned char value); // Nao permitir interrupts
unsigned char causeIRQ_wiznet(); // Verificar causa do interrupt
unsigned char IRQ_wiznet(); // Verificar se existe interrupt
unsigned char SnIRQ_wiznet(Socket s, unsigned char type); // Tipo de interrupt no socket


//SOCKETS
void setSnType_wiznet(Socket s, unsigned char val); //Definir protocolo do socket
unsigned char getSnStatus_wiznet(Socket s); // Socket status
unsigned int getSn_TXsize_wiznet(Socket s); // Tamanho do buffer do socket_TX
unsigned int getSn_RXsize_wiznet(Socket s); // Tamanho do buffer do socket_RX
void setSn_DestMAC_wiznet(Socket s, unsigned char *addr); // MAC de destino
void setSn_DestIP_wiznet(Socket s, unsigned int addr); // IP destino
void setSn_DestPORT_wiznet(Socket s, unsigned int addr); // PORTO destino
void sendData_wiznet(Socket s, unsigned char *data, unsigned int len); //Escrever dados para TRX
void recvData_wiznet(Socket s, unsigned char *data, unsigned int len); // Ler dados recebidos

//UDP
void createUDP_socket(Socket s, unsigned int port); // Abrir um socket UDP
void close_socket(Socket s); // Fechar socket
void sendtoUDP_socket(Socket s, unsigned char *src_buf, unsigned int len, unsigned int addr, unsigned int port); // Enviar data (UDP/IP RAW)
//unsigned int recvfromUDP_socket(Socket s, unsigned char *dst_buf, unsigned int *addr, unsigned int *port ); // Receber data (UDP/IP RAW)




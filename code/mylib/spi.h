typedef struct {
	int clock;	//ritmo de clk
	int cpha;	// fase
	int cpol;	// polaridade
	int cbits;	// numero de bits de uma palavra
	int lsbf;	//ordem de TRX dos bits (LSB -> MSB) ou (MSB -> LSB)
	void (*cs)(int); // apontador para a funcao que selecciona o slave
} Spi_device;

void spi_init(); // Inicializa SPI
void spi_transaction_begin(Spi_device *); //selecciona periferico
void spi_transaction_end(Spi_device *); // termina seleccao do periferico
void spi_transaction_transfer(Spi_device *device, unsigned int size, unsigned char *tx_buffer, unsigned char *rx_buffer); // transfere dados

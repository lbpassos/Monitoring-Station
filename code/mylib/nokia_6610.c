#include "nokia_6610.h"
#include "lpc2106.h"
#include "spi.h"


#define CS_PIN			7
#define BACKLIGHT_PIN	8
#define	RESET_PIN		9

void LCDPutChar(char c, int x, int y, int fColor, int bColor)__attribute__((section(".text.fastcode")));



const unsigned char FONT8x8[99][8] = {
0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00, // columns, rows, num_bytes_per_char
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00, // !
0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00, // "
0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00, // #
0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00, // $
0x00,0x63,0x66,0x0C,0x18,0x33,0x63,0x00, // %
0x1C,0x36,0x1C,0x3B,0x6E,0x66,0x3B,0x00, // &
0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00, // '
0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00, // (
0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00, // )
0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00, // *
0x00,0x30,0x30,0xFC,0x30,0x30,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30, // ,
0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00, // .
0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00, // / (forward slash)
0x3E,0x63,0x63,0x6B,0x63,0x63,0x3E,0x00, // 0     0x30
0x18,0x38,0x58,0x18,0x18,0x18,0x7E,0x00, // 1
0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00, // 2
0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00, // 3
0x0E,0x1E,0x36,0x66,0x7F,0x06,0x0F,0x00, // 4
0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00, // 5
0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00, // 6
0x7E,0x66,0x06,0x0C,0x18,0x18,0x18,0x00, // 7
0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00, // 8
0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00, // 9
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00, // :
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30, // ;
0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00, // <
0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00, // =
0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00, // >
0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00, // ?
0x3E,0x63,0x6F,0x69,0x6F,0x60,0x3E,0x00, // @   0x40
0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00, // A
0x7E,0x33,0x33,0x3E,0x33,0x33,0x7E,0x00, // B
0x1E,0x33,0x60,0x60,0x60,0x33,0x1E,0x00, // C
0x7C,0x36,0x33,0x33,0x33,0x36,0x7C,0x00, // D
0x7F,0x31,0x34,0x3C,0x34,0x31,0x7F,0x00, // E
0x7F,0x31,0x34,0x3C,0x34,0x30,0x78,0x00, // F
0x1E,0x33,0x60,0x60,0x67,0x33,0x1F,0x00, // G
0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00, // H
0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // I
0x0F,0x06,0x06,0x06,0x66,0x66,0x3C,0x00, // J
0x73,0x33,0x36,0x3C,0x36,0x33,0x73,0x00, // K
0x78,0x30,0x30,0x30,0x31,0x33,0x7F,0x00, // L
0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00, // M
0x63,0x73,0x7B,0x6F,0x67,0x63,0x63,0x00, // N
0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00, // O
0x7E,0x33,0x33,0x3E,0x30,0x30,0x78,0x00, // P
0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00, // Q
0x7E,0x33,0x33,0x3E,0x36,0x33,0x73,0x00, // R
0x3C,0x66,0x30,0x18,0x0C,0x66,0x3C,0x00, // S
0x7E,0x5A,0x18,0x18,0x18,0x18,0x3C,0x00, // T
0x66,0x66,0x66,0x66,0x66,0x66,0x7E,0x00, // U
0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00, // V
0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00, // W
0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00, // X
0x66,0x66,0x66,0x3C,0x18,0x18,0x3C,0x00, // Y
0x7F,0x63,0x46,0x0C,0x19,0x33,0x7F,0x00, // Z
0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00, // [
0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00, // \ (back slash)
0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00, // ]
0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF, // _
0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00, // `
0x00,0x00,0x3C,0x06,0x3E,0x66,0x3B,0x00, // a
0x70,0x30,0x3E,0x33,0x33,0x33,0x6E,0x00, // b 
0x00,0x00,0x3C,0x66,0x60,0x66,0x3C,0x00, // c 
0x0E,0x06,0x3E,0x66,0x66,0x66,0x3B,0x00, // d
0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00, // e
0x1C,0x36,0x30,0x78,0x30,0x30,0x78,0x00, // f
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x7C, // g
0x70,0x30,0x36,0x3B,0x33,0x33,0x73,0x00, // h
0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00, // i
0x06,0x00,0x06,0x06,0x06,0x66,0x66,0x3C, // j
0x70,0x30,0x33,0x36,0x3C,0x36,0x73,0x00, // k
0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // l
0x00,0x00,0x66,0x7F,0x7F,0x6B,0x63,0x00, // m
0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00, // n
0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00, // o
0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78, // p
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F, // q
0x00,0x00,0x6E,0x3B,0x33,0x30,0x78,0x00, // r
0x00,0x00,0x3E,0x60,0x3C,0x06,0x7C,0x00, // s
0x08,0x18,0x3E,0x18,0x18,0x1A,0x0C,0x00, // t
0x00,0x00,0x66,0x66,0x66,0x66,0x3B,0x00, // u
0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00, // v
0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00, // w
0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00, // x
0x00,0x00,0x66,0x66,0x66,0x3E,0x06,0x7C, // y
0x00,0x00,0x7E,0x4C,0x18,0x32,0x7E,0x00, // z
0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00, // {
0x0C,0x0C,0x0C,0x00,0x0C,0x0C,0x0C,0x00, // |
0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00, // }
0x3B,0x6E,0x00,0x00,0x00,0x00,0x00,0x00, // ~
0x1C,0x36,0x36,0x1C,0x00,0x00,0x00,0x00, // DEL
0x0C,0x12,0x12,0x0C,0x00,0x00,0x00,0x00, // Graus: 128 
0x10,0x28,0x3C,0x66,0x7E,0x60,0x3C,0x00 };// ê



static Spi_device spi;
static uint8_t contrast;

static void cs_set(int value){
	if(value)
		IOCLR = 1<<CS_PIN;
	else
		IOSET = 1<<CS_PIN;
}

void lcd_io_init() {
	/* direcionar os pinos de CS, RESET E BACKLIGHT para o GPIO */
	uint32_t pinsel = PINSEL0;
	PINSEL0 = pinsel & ~(3 << CS_PIN * 2) & ~(3 << BACKLIGHT_PIN * 2) & ~(3 << RESET_PIN * 2);
	/* desactivar os sinais; são todos active low */
	IOSET = 1 << CS_PIN | 1 << RESET_PIN;// 1 << BACKLIGHT_PIN;
	IOCLR = 1 << BACKLIGHT_PIN;
	/* e programá-los como saídas */
	uint32_t iodir = IODIR;
	IODIR = iodir | 1 << CS_PIN | 1 << BACKLIGHT_PIN | 1 << RESET_PIN;
	
	spi.clock = 10;
	spi.cbits = 9;
	//spi.lsbf = 1;
	spi.cs = cs_set;
	contrast = 45;
	/*spi = {
		10,					//SCLK
		0,					//CPHA
		0,					//CPOL
		9,					//Numero de bits por palavra
		0,					//LSBF
		cs_set				
	};*/
	spi_transaction_begin(&spi); 
	write_command(DISCTL);	//Display control
	write_data(0x00);		//P1=0x00 - 2 divisions, switching period=8 (default)
	write_data(32);			//P2=nlines/4 - 1 = 132/4 - 1 = 32)
	write_data(00);			//P3=0x00 - no inversely highlighted lines
	write_command(COMSCN);	//COM scan
	write_data(1);			//P1=0x01 - Scan 1->80, 160<-81
	write_command(OSCON);	//Internal oscilator ON
	write_command(SLPOUT);	//sleep out
	write_command(VOLCTR);	//Voltage control (contrast setting)
	write_data(contrast);	//P1 = 32
	write_data(3);			//P2 = 3 resistance ratio (only value that works)
	write_command(PWRCTR);	//Power control
	write_data(0x0f);		//reference voltage regulator on, circuit voltage follower on, BOOST ON
		
	write_command(DATCTL);	//Data control
	write_data(0x00);		//P1=0x00 - page address normal, column address normal, address scan in column direction
	write_data(0x00);		//P2=0x00 - RGB sequence (default value)
	write_data(0x02);		//P3=0x02-Grayscale->16 (select 12-bit color, type A)
	
	//lcd_fill_rectangle(0, 0, 132, 132, 0xFFF);
	
	chrono_delay(1000);
	write_command(DISON);	//Display on
	spi_transaction_end(&spi);
}

void lcd_backlight(int on) {
	if (on)
		IOSET = 1 << BACKLIGHT_PIN;
	else
		IOCLR = 1 << BACKLIGHT_PIN;
}

static inline void write_command(uint8_t command) {
	uint16_t out_buffer[] = {~0x100 & command};
	//uint16_t in_buffer[sizeof(out_buffer)];
	spi_transaction_transfer(&spi, /*sizeof(out_buffer)*/1, (uint8_t*)out_buffer, /*(uint8_t*)in_buffer*/0x00);
}

static inline void write_data(uint8_t data) {
	uint16_t out_buffer[] = {0x100 | data};
	//uint16_t in_buffer[sizeof(out_buffer)];
	spi_transaction_transfer(&spi, /*sizeof(out_buffer)*/1, (uint8_t*)out_buffer, /*(uint8_t*)in_buffer*/0x00);
}


void lcd_draw_point(int x, int y, int color) {
	spi_transaction_begin(&spi);
	write_command(CASET);	/* Column address set (command 0x2A) */
	write_data(x);
	write_data(x);
	write_command(PASET);	/* Page address set (command 0x2B) */
	write_data(y);
	write_data(y);
	write_command(RAMWR);	/* write memory */
	write_data(color >> 4);
	write_data((color << 4) & 0xf0);
	write_data(0);
	write_command(NOP);
	spi_transaction_end(&spi);
}

void lcd_draw_hor_line(int x, int y, int dx, int color) {
	int i;
	spi_transaction_begin(&spi);
	write_command(CASET);
	write_data(x);
	write_data(x + dx - 1);
	write_command(PASET);
	write_data(y);
	write_data(y);
	write_command(RAMWR);
	for (i = (dx + 1) / 2; i > 0; --i) {
		write_data(color >> 4);
		write_data(((color << 4) & 0xf0) | ((color >> 8) & 0xf));
		write_data(color);
	}
	write_command(NOP);
	spi_transaction_end(&spi);
}

void lcd_draw_ver_line(int x, int y, int dy, int color) {
	int i;
	spi_transaction_begin(&spi);
	write_command(CASET);
	write_data(x);
	write_data(x);
	write_command(PASET);
	write_data(y);
	write_data(y + dy - 1);
	write_command(RAMWR);
	for (i = (dy + 1) / 2; i > 0; --i) {
		write_data(color >> 4);
		write_data(((color << 4) & 0xf0) | ((color >> 8) & 0xf));
		write_data(color);
	}
	write_command(NOP);
	spi_transaction_end(&spi);
}

void lcd_fill_rectangle(int x, int y, int dx, int dy, int color) {
	int i;
	spi_transaction_begin(&spi);
	write_command(CASET);
	write_data(x);
	write_data(x + dx - 1);
	write_command(PASET);
	write_data(y);
	write_data(y + dy - 1);
	write_command(RAMWR);
	for (i = (dx * dy + 1) / 2 + 1; i > 0; --i) {
		write_data(color >> 4);
		write_data(((color << 4) & 0xf0) | ((color >> 8) & 0xf));
		write_data(color);
	}
	write_command(NOP);
	spi_transaction_end(&spi);
}

void lcd_copy_rectangle(int x, int y, int dx, int dy, uint8_t * bitmap) {
	int i;
	spi_transaction_begin(&spi);
	write_command(CASET);
	write_data(x);
	write_data(x + dx - 1);
	write_command(PASET);
	write_data(y);
	write_data(y + dy - 1);
	write_command(RAMWR);
	for (i = (dx * dy + 1) / 2 + 1; i > 0; --i) {
		write_data(*bitmap++);
	}
	write_command(NOP);
	spi_transaction_end(&spi);
}

/*
void lcd_inc_contrast(){
	//++contrast;
	spi_transaction_begin(&spi);
	write_command(VOLCTR);	//Voltage control (contrast setting)
	write_data(++contrast);	//P1 = 32
	write_data(3);			//P2 = 3 resistance ratio (only value that works)
	spi_transaction_end(&spi);
}

void lcd_dec_contrast(){
	//--contrast;
	spi_transaction_begin(&spi);
	write_command(VOLCTR);	//Voltage control (contrast setting)
	write_data(--contrast);	//P1 = 32
	write_data(3);	
	spi_transaction_end(&spi);
}
*/

void lcd_reset(){
	IOCLR = 1 << RESET_PIN;
	chrono_delay(1000);
	IOSET = 1 << RESET_PIN;
	chrono_delay(1000);
	
}

void LCDPutChar(char c, int x, int y, /*int size,*/ int fColor, int bColor) {
	//extern const unsigned char FONT6x8[97][8];
	//extern const unsigned char FONT8x8[97][8];
	//extern const unsigned char FONT8x16[97][16];
	int i,j;
	unsigned int nCols;
	unsigned int nRows;
	unsigned int nBytes;
	unsigned char PixelRow;
	unsigned char Mask;
	unsigned int Word0;
	unsigned int Word1;
	unsigned char *pFont;// = font_5_7();
	unsigned char *pChar;
	//unsigned char *FontTable[] = {(unsigned char *)FONT6x8, (unsigned char *)FONT8x8,
		//						(unsigned char *)FONT8x16};
	// get pointer to the beginning of the selected font table
	pFont = (unsigned char *)FONT8x8;
	// get the nColumns, nRows and nBytes
	nCols = *pFont;
	nRows = *(pFont + 1);
	nBytes = *(pFont + 2);

	// get pointer to the last byte of the desired character
	pChar = pFont + (nBytes * (c - 0x1F)); //+ nBytes - 1;

	// Row address set (command 0x2B)
	spi_transaction_begin(&spi);
	
	write_command(PASET);
	write_data(x);
	write_data(x + nRows - 1);
	// Column address set (command 0x2A)
	write_command(CASET);
	write_data(y);
	write_data(y + nCols - 1);

	// WRITE MEMORY
	write_command(RAMWR);
	// loop on each row, working backwards from the bottom to the top
	for (i = nRows - 1; i >= 0; i--) {
		// copy pixel row from font table and then decrement row
		PixelRow = *pChar++;
		// loop on each pixel in the row (left to right)
		// Note: we do two pixels each loop
		Mask = 0x80;
		for (j = 0; j < nCols; j += 2) {
			// if pixel bit set, use foreground color; else use the background color
			// now get the pixel color for two successive pixels
			if ((PixelRow & Mask) == 0)
				Word0 = bColor;
				else
					Word0 = fColor;
			Mask = Mask >> 1;
			if ((PixelRow & Mask) == 0)
				Word1 = bColor;
				else
					Word1 = fColor;
			Mask = Mask >> 1;
			// use this information to output three data bytes
			write_data((Word0 >> 4) & 0xFF);
			write_data(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
			write_data(Word1 & 0xFF);
		}
	}
	// terminate the Write Memory command
	write_command(NOP);
	spi_transaction_end(&spi);
}

void LCDPutStr(char *pString, int x, int y, int fColor, int bColor) {
	// loop until null-terminator is seen
	while (*pString != 0x00) {
		// draw the character
		LCDPutChar(*pString++, x, y, fColor, bColor);
		// advance the y position
		
		y = y + 8;
				
		if (y > 131) break;
	}
}


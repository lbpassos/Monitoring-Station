#define I2CONSET        (*((volatile unsigned long*) 0xE001C000))
#define I2STAT        (*((volatile unsigned long*) 0xE001C004))
#define I2DAT        (*((volatile unsigned long*) 0xE001C008))
#define I2ADR        (*((volatile unsigned long*) 0xE001C00C))
#define I2SCLH        (*((volatile unsigned long*) 0xE001C010))
#define I2SCLL        (*((volatile unsigned long*) 0xE001C014))
#define I2CONCLR        (*((volatile unsigned long*) 0xE001C018))



#define TRANSMIT_MODE		0x00
#define RECEIVER_MODE		0x01
#define TRANSCEIVER_MODE	0x02

void init_i2c(void);

void i2c_isrHandle(void);
void i2c_set(unsigned char slave_add, unsigned char mode);
void i2c_comm(unsigned char *ptr_datatx, int dimtx, unsigned char *ptr_datarx, int dimrx);
int i2c_status(void);

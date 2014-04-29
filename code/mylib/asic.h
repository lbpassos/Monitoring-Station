#define AEHF		0
#define	SAG			1
#define	CYCEND		2
#define	WSMP		3
#define	ZX			4
#define	TEMP		5
#define	RESET		6
#define	AEOF		7
#define	PKV			8
#define	PKI			9
#define	VAEHF		10
#define	VAEOF		11
#define	ZXTO		12
#define	PPOS		13
#define	PNEG		14

#define	CYCMODE		7 

#define SETTLING_TIME	900

//rms status
#define INIT		0
#define START		1
#define MEASURE		2
#define WAIT_ISR	3
#define RMS			4
#define COMPLETE	5
#define ENERGY		6

#define ON	1
#define OFF	0

#define INTEGRATION_TIME	20000

void asic_init();
void write_asic(unsigned char *command);
void read_asic(unsigned char *command, unsigned char dim);

unsigned int irq_asic();
void reset_asic();
void enableIsr_asic( unsigned char command );
void disableIsr_asic( unsigned char command );
unsigned int isrStatus_asic();

float Urms_asic();
float Irms_asic();
float ActiveE_value();
float PowerFactor();

unsigned int ActiveEnergy_asic();
float Urms_value();
float Irms_value();
void startRMS_measure_asic();
unsigned int measureRMS_complete_asic();	
void endRMS_measure_asic();
unsigned int isWattInc();

void asic_rms();
unsigned int CaliberActiveEnergy_asic();

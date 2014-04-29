/* Watchdog */
#define WDMOD		   (*((volatile unsigned char *) 0xE0000000))
#define WDTC           (*((volatile unsigned long *) 0xE0000004))
#define WDFEED         (*((volatile unsigned char *) 0xE0000008))
#define WDTV           (*((volatile unsigned long *) 0xE000000C))
/* Memory Accelerator Module (MAM) */
#define MAMCR          (*((volatile unsigned char *) 0xE01FC000))
#define MAMTIM         (*((volatile unsigned char *) 0xE01FC004))
/* Phase Locked Loop (PLL) */
#define PLLCON         (*((volatile unsigned char *) 0xE01FC080))
#define PLLCFG         (*((volatile unsigned char *) 0xE01FC084))
#define PLLSTAT        (*((volatile unsigned short*) 0xE01FC088))
#define PLLFEED        (*((volatile unsigned char *) 0xE01FC08C))
/* VPB Divider */
#define VPBDIV         (*((volatile unsigned char *) 0xE01FC100))


/*TIMER0  - CONFIG & STATUS REGISTERS*/
#define TIMER0_IR        (*((volatile unsigned long*) 0xE0004000))
#define TIMER0_TCR       (*((volatile unsigned long*) 0xE0004004))
#define TIMER0_TC        (*((volatile unsigned long*) 0xE0004008))
#define TIMER0_PR        (*((volatile unsigned long*) 0xE000400C))
#define TIMER0_PC        (*((volatile unsigned long*) 0xE0004010))

/*TIMER0  - MATCH REGISTERS */ 
#define TIMER0_MCR       (*((volatile unsigned long*) 0xE0004014))
#define TIMER0_MR0       (*((volatile unsigned long*) 0xE0004018))
#define TIMER0_MR1       (*((volatile unsigned long*) 0xE000401C))
#define TIMER0_MR2       (*((volatile unsigned long*) 0xE0004020))
#define TIMER0_MR3       (*((volatile unsigned long*) 0xE0004024))

/*TIMER0  - CAPTURE REGISTERS */

#define TIMER0_CCR       (*((volatile unsigned long*) 0xE0004028))
#define TIMER0_CR0       (*((volatile unsigned long*) 0xE000402C))
#define TIMER0_CR1       (*((volatile unsigned long*) 0xE0004030))
#define TIMER0_CR2       (*((volatile unsigned long*) 0xE0004034))
#define TIMER0_CR3       (*((volatile unsigned long*) 0xE0004038))
#define TIMER0_EMR       (*((volatile unsigned long*) 0xE000403C))

#define PLOCK 		0x400



#define	FOSC	14745600
#define	PLL_M	4
#define PLL_P	2
#define CCLK	(FOSC * PLL_M) // 58.982.400 = 58Mhz
#define PCLK	(CCLK / 4) // 14.745.600 = 14Mhz


#define IOPIN          (*((volatile unsigned long *) 0xE0028000))
#define IOSET          (*((volatile unsigned long *) 0xE0028004))
#define IODIR          (*((volatile unsigned long *) 0xE0028008))
#define IOCLR          (*((volatile unsigned long *) 0xE002800C))
#define PINSEL0          (*((volatile unsigned long *) 0xE002C000))
#define PINSEL1        (*((volatile unsigned long *) 0xE002c004))

/*         Real Time Clock   */
#define ILR			(*((volatile unsigned long *) 0xE0024000))
#define CTC			(*((volatile unsigned long *) 0xE0024004))
#define CCR			(*((volatile unsigned long *) 0xE0024008))
#define CIIR		(*((volatile unsigned long *) 0xE002400C))
#define AMR			(*((volatile unsigned long *) 0xE0024010))
#define CTIME_0		(*((volatile unsigned long *) 0xE0024014))
#define CTIME_1		(*((volatile unsigned long *) 0xE0024018))
#define CTIME_2 	(*((volatile unsigned long *) 0xE002401C))
#define SEC    		(*((volatile unsigned long *) 0xE0024020))
#define MIN         (*((volatile unsigned long *) 0xE0024024))
#define HOUR        (*((volatile unsigned long *) 0xE0024028))
#define DOM        	(*((volatile unsigned long *) 0xE002402C))
#define DOW         (*((volatile unsigned long *) 0xE0024030))
#define DOY         (*((volatile unsigned long *) 0xE0024034))
#define MONTH       (*((volatile unsigned long *) 0xE0024038))
#define YEAR        (*((volatile unsigned long *) 0xE002403C))
#define ALSEC       (*((volatile unsigned long *) 0xE0024060))
#define ALMIN       (*((volatile unsigned long *) 0xE0024064))
#define ALHOUR      (*((volatile unsigned long *) 0xE0024068))
#define ALDOM       (*((volatile unsigned long *) 0xE002406C))
#define ALDOW       (*((volatile unsigned long *) 0xE0024070))
#define ALDOY       (*((volatile unsigned long *) 0xE0024074))
#define ALMON       (*((volatile unsigned long *) 0xE0024078))
#define ALYEAR      (*((volatile unsigned long *) 0xE002407C))
#define PREINT      (*((volatile unsigned long *) 0xE0024080))
#define PREFRAC     (*((volatile unsigned long *) 0xE0024084))

/*=============================================================================
	Vectored Interrupt Controller - VIC
*/

#define MEMMAP	(*((volatile unsigned short*) 0xE01FC040))

typedef volatile struct {
	unsigned long 
		IRQStatus,
		FIQStatus,
		RawIntr,
		IntSelect,
		IntEnable,
		IntEnClear,
		SoftInt,
		SoftIntClear,
		Protection,
		VectAddr,
		DefVectAddr,
		gap0[(0x100 - 0x38) / 4],
		VectorAddr[16],
		gap1[(0x200 - 0x13D) / 4],
		VectCntl[16];
} LPC2XXX_VIC;

#define  VIC ((LPC2XXX_VIC*) 0xFFFFF000)

#define	VIC_SOURCE_WDT			0
#define VIC_SOURCE_Dbg_CommRx	2
#define VIC_SOURCE_Dbg_CommTx	3
#define VIC_SOURCE_TIMER0		4
#define VIC_SOURCE_TIMER1		5
#define VIC_SOURCE_UART0		6
#define VIC_SOURCE_UART1		7
#define VIC_SOURCE_PWM			8
#define VIC_SOURCE_I2C			9
#define VIC_SOURCE_SPI			10
#define VIC_SOURCE_PLL			12
#define VIC_SOURCE_RTC			13
#define VIC_SOURCE_EINT0		14
#define VIC_SOURCE_EINT1		15
#define VIC_SOURCE_EINT2		16





typedef unsigned long uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

void counter_init();
void chrono_delay(unsigned int);

unsigned int chrono_start();
unsigned int chrono_timeout(unsigned int initial, unsigned int delta);
//unsigned long int chrono_restart(unsigned long int initial, unsigned long int delta);

void watchdog_init();
void watchdog_reload();


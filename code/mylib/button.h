#define IOPIN          (*((volatile unsigned long *) 0xE0028000))
#define IOSET          (*((volatile unsigned long *) 0xE0028004))
#define IODIR          (*((volatile unsigned long *) 0xE0028008))
#define IOCLR          (*((volatile unsigned long *) 0xE002800C))
#define PINSEL0          (*((volatile unsigned long *) 0xE002C000))
#define PINSEL1        (*((volatile unsigned long *) 0xE002c004))


void button_init(); // Iniciar modulo
int button_state(int button_code); //Ver estado do botao (pressionado ou liberto)
int button_pressed(); // Pesquisar botão
int button_read(); // Ler codigo do botao

#define	EVENT_KEY_PRESSED		1
#define	EVENT_NOKIA_TIMEOUT		2
#define EVENT_CHECK_INFRA		3
#define EVENT_READ_INFRA		4
#define EVENT_CHECK_U_I			5
#define EVENT_READ_U_I			6
#define EVENT_CHECK_ENERGY		7
#define EVENT_SNMP				8

#define MAX_SEC_TIMERS			4
#define FIRST_SEC_TIMER			0
#define SEC_SEC_TIMER			1
#define THIRD_SEC_TIMER			2
#define FOURTH_SEC_TIMER		3

typedef struct _event{
	unsigned int type;
	unsigned int value;
}event_t;

typedef struct _milisecond{
	unsigned int initial;
	unsigned int delta;
}Milisecond_t;

void event_init();
void event_read(event_t *ptr);

void event_timer_start_SEC(unsigned int timer_def, unsigned int s);
int timeOut_event(unsigned char timer_def);



/*
void event_timer_start_SECOND(unsigned int ms, unsigned int mul);
void event_couter_minutes(unsigned char min);
*/

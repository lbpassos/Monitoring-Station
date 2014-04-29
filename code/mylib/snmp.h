
#define SNMP_PORT			    	161 // Porto de RX
#define SNMP_TRAP_PORT				162
#define SNMP_VERSION_1	    		0	// Versao suportada
#define SNMP_MAX_LEN				128 // Maxima msg SNMP suportada

#define NumberNodes					8 // Num_max de NOS
#define	NumberGroups				2 // Num_max de grupos
#define NumberContents_PRODUTO		1
#define NumberContents_CONTROLO		5

// Mensagens suportadas
#define MSG_GET_REQUEST				0
#define MSG_GET_RESPONSE			2


//Erros SNMP
#define SNMP_ERR_NOERROR    		0
#define SNMP_ERR_TOOBIG	    		1
#define SNMP_ERR_NOSUCHNAME 		2
#define SNMP_ERR_BADVALUE   		3
#define SNMP_ERR_READONLY   		4
#define SNMP_ERR_GENERR	    		5

//MIB
#define MIB_ISO						1
#define MIB_ORG						3
#define MIB_DOD						6
#define MIB_INTERNET				1
#define MIB_PRIVATE					4
#define MIB_ENTERPRISE				1
 
// IANA-IPL = 16884
#define IANA_IPL_UP				0x81 // UP(16884)/128
#define IANA_IPL_MID			0X83 //	Lower(16884)
#define IANA_IPL_LOW			0x74 

//MIB Isel
#define MIB_ISEL_IPL_PRODUTO		1
#define MIB_ISEL_IPL_CONTROLE		2

#define MIB_ISEL_IPL_PRODUTO_NOME	1

#define MIB_ISEL_IPL_CONTROLE_U		1
#define MIB_ISEL_IPL_CONTROLE_I		2
#define MIB_ISEL_IPL_CONTROLE_E		3
#define MIB_ISEL_IPL_CONTROLE_T		4
#define MIB_ISEL_IPL_CONTROLE_H		5

//ASN.1
#define ASN_BOOLEAN	    	1 // 1 ou 0
#define ASN_INTEGER	    	2 // char
#define ASN_BIT_STR	    	3 // 
#define ASN_OCTET_STR	    4
#define ASN_NULL	    	5
#define ASN_OBJECT_ID	    6
#define ASN_SEQUENCE	    16
#define ASN_SET		    	17
#define ASN_NUMERIC_STRING	18

#define ASN_UNIVERSAL	    0x00 //Class
#define ASN_APPLICATION     0x40 //
#define ASN_CONTEXT	    	0x80
#define ASN_PRIVATE	    	0xC0

#define ASN_FORMAT			0x20 //Dados complexos

#define RONLY		0
#define WONLY		1
#define RWRITE		2
#define NOT_FOUND	0
#define FOUND		1

//Variaveis
typedef struct _var{
	unsigned char id; //OID variavel ID
	unsigned char access; //READ, WRITE RWRITE
	unsigned char type; //ASN.1 
}OID_VAR;

typedef struct _varGroup{
	unsigned char oid; //OID do grupo
	unsigned char numberContents; // Numero de conteudos
	//OID_VAR *group; //Variaveis no grupo
}GROUP_MIB;


typedef struct _OID{ // OID recebido
	unsigned char id[NumberNodes+1+1+1];//Nodes+Grupo+Variavel+0
	//unsigned char found; // se existir 1, se nao 0
}OID;


//Funcoes
void init_agent_snmp(unsigned char *RX_buf, unsigned char *TX_buf, unsigned char RX_size, unsigned char TX_size); //Iniciar o snmp 
void initVariables_snmp(float *U, float *I, float *E, char *t, char *h);

void proc_agent_snmp(); //Executar agente

unsigned char printVar(unsigned char num, unsigned char *ptr);
unsigned char float_toChar(unsigned char *dest, float *ptr, unsigned char precision);
unsigned char char_toString(unsigned char *dest, unsigned char *ptr);

typedef struct _memManager{
	unsigned int start_address; // end. de inicio
	unsigned int size; // tamanho do modulo
	unsigned int write; // prox. end de escrita
	unsigned int read; // end. de leitura
	unsigned int count; // num de elementos na memoria
} memManager;

void init_memManager(memManager *Mem, unsigned int add, unsigned int size);
int insert_memManager(memManager *Mem, unsigned char *data, unsigned char dim);
int read_memManager(memManager *Mem, unsigned char *dest_data, unsigned int size);
int update_readManager(memManager *Mem, unsigned int dim);
int isSizeAvailable_forInsert(memManager *Mem, unsigned char dim);
int isSizeAvailable_forRead(memManager *Mem, unsigned char dim);


	

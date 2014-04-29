
typedef struct fifoBuffer{
	int *buffer;//buffer
	int dimBuffer;//dimensao do fifo
	int read;//leitura
	int write;//escrita
	int count;// num de teclas no buffer
} FifoNode;

void newBuffer(FifoNode *, int, int *); // Criar fifobuffer
int write(FifoNode *, int); // Escrever
int read(FifoNode *); // Ler

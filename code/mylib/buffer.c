#include "buffer.h"

/* Criar fifonode
 * Parametros de Entrada: Estrutura fifo
 * 					      Dimensao do buffer
 * 						  buffer (array de char)
 */
void newBuffer(FifoNode *fifo, int dim, int *ptr){
	fifo->buffer = ptr;
	fifo->dimBuffer = dim;
	fifo->read = 0;
	fifo->write = 0;
	fifo->count = 0;
}

/* Escrever no fifonode
 * Parametros de Entrada:	Estrutura fifo
 * 							tecla
 * Parametro de Saida:		0 -> se não for possivel inserir
 * 							tecla
 */
int write(FifoNode *fifo, int tecla){
	if( fifo->count == fifo->dimBuffer ) return 0; // buffer cheio
	fifo->buffer[fifo->write] = tecla; // Inserir
	++(fifo->count); // incrementar numero teclas presentes no buffer
	if( ++(fifo->write) == fifo->dimBuffer ) fifo->write = 0;
	return tecla;
}

/*	Ler do fifonode	
 * Parametros de Entrada:	fifo
 * Parametros de saida:		0 -> buffer vazio
 * 							tecla
 */
int read(FifoNode *fifo){
	int tmp;
	if ( !(fifo->count) ) return 0;//vazio
	--(fifo->count); // decrementar numero teclas presentes no buffer
	tmp = fifo->buffer[(fifo->read)++];// Ler
	if( fifo->read == fifo->dimBuffer ) fifo->read = 0;
	return tmp;
}

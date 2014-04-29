#include "mem_manager.h"
#include "32k_memmory.h"

#define INIT		0
#define WRITE		1
#define READ		2

static unsigned char status_insert;
static unsigned char status_read;
static unsigned char *ptr_data;
static unsigned char dim_rover;

void init_memManager(memManager *Mem, unsigned int add, unsigned int size){
	Mem->start_address = add;
	Mem->size = size;
	Mem->write = add;
	Mem->read = add; 
	Mem->count = 0x00;
}

int insert_memManager(memManager *Mem, unsigned char *data, unsigned char dim){
	
	switch(status_insert){
		case INIT:
				ptr_data = data;
				dim_rover = dim;
				status_insert = WRITE;
				break;
		case WRITE:
				if( write_memmory( Mem->write, *ptr_data) ){
					if( ++Mem->write == (Mem->start_address+Mem->size) )
						Mem->write = Mem->start_address;
					++Mem->count;
					--dim_rover;
					++ptr_data;
					if( !dim_rover ){
						status_insert = INIT;
						return Mem->count;
					}
				}
				break;
		default:
				break;
	}
	return 0;
}
						

int read_memManager(memManager *Mem, unsigned char *dest_data, unsigned int size){
	int i;
	switch(status_read){
		case INIT:
				status_read = READ;
				break;
		case READ:
				if( read_memmory(Mem->read, dest_data, size) ){
					status_read = INIT;
					return size;	
				}
				break;		
		default:
				break;
	}
	return 0;
}

int update_readManager(memManager *Mem, unsigned int dim){
	int i;
	for(i=0; i<dim; ++i){
		++Mem->read;
		--Mem->count;
		if( Mem->read == (Mem->start_address+Mem->size) )
			Mem->read = Mem->start_address;
	}
	return dim;
}

int isSizeAvailable_forInsert(memManager *Mem, unsigned char dim){
	if( (Mem->size-Mem->count) >= dim ) return 1;
	return 0;
}

int isSizeAvailable_forRead(memManager *Mem, unsigned char dim){
	if( Mem->count >= dim ) return 1;
	return 0;
}

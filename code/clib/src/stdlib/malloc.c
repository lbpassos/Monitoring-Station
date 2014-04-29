#include <stdlib.h>
#include <string.h>
#include <assert.h>

#undef	DEBUG
#undef	CHECK

/*	
 *	Este módulo implementa as funções normalizadas "malloc" e "free".
 *	A memória utilizável está compreendida entre "heap_bottom" e "heap_top",
 *	estas variáveis são definidas noutro módulo.
 *
 *	Algoritmo baseado num exemplo do livro:
 *		"The C Programming Language", B. Kernighan and D. Ritchie
 */

/*
 *	Definicao do tipo de dados utilizado para o cabecalho de um bloco
 *	de memoria, quando este se encontra na lista dos blocos livres
 */

typedef struct header {
	struct header *next;	/* proximo elemento da lista */
	size_t size;			/* dimensao do bloco em multiplos da
							   da dimensao do cabecalho */
} * header_t;

//==============================================================================
//	Verificação

#ifdef CHECK

#define lengthof(x) (sizeof(x)/sizeof(x[0]))

static void * checkArray[100];
static size_t checkCount = 0;

static void checkInsert(void * p) {
	size_t i;
	for (i = 0; i < checkCount; ++i)
		if (p < checkArray[i])
			break;
	memmove(&checkArray[i + 1], &checkArray[i], (checkCount - i) * sizeof(void *));
	checkArray[i] = p;
	checkCount++;
#ifdef	DEBUG
	cprintf("allocate= %08x checkCount= %d\n\r", p, checkCount);
#endif
	assert(checkCount < lengthof(checkArray));
}

static void checkRemove(void * p) {
	size_t i;
	for (i = 0; i < checkCount; ++i)
		if (p <= checkArray[i])
			break;
	assert(i < checkCount && p == checkArray[i]);
	memmove(&checkArray[i], &checkArray[i + 1], (checkCount - i) * sizeof(void *));
	checkCount--;
#ifdef	DEBUG
	cprintf("deallocate= %08x checkCount= %d\n\r", p, checkCount);
#endif
}

static void checkPrint() {
	size_t i;
	for (i = 0; i < checkCount; ++i) {
		header_t p = (header_t) checkArray[i];
		cprintf("%4d start= %08x size= %d\n\r", i, p, p->size * sizeof(struct header));
	}
}

#endif

//==============================================================================

/*
 *	Variaveis globais ao gestor de memoria
 */

static struct header base;	/* elemento sentinela */
static header_t freep;		/* entrada para a lista livre */
static size_t allocated;	/* dimensão total da memória allocada */

void * x;

/*
 *	malloc: reservar um bloco de memoria e preenche-lo com zeros
 */

void * malloc(size_t nbytes) {
	header_t p, prevp;
	unsigned nunits;

	/* calcular a dimensao do bloco a reservar, em multiplos da dimensao do
	   cabechalho */
	nunits = (nbytes + sizeof(struct header) - 1) / sizeof(struct header) + 1;
	/* pesquisar a lista dos blocos livres ate encontrar um com a dimensao
	   suficiente */
	for (prevp = freep, p = prevp->next; ; prevp = p, p = p->next) {
		if (p->size >= nunits) {	/* dimensao suficiente */
			if (p->size == nunits)	/* exactamente igual */
				prevp->next = p->next;
			else {		/* reservar no fim do bloco livre */
				p->size -= nunits;
				p += p->size;
				/* memorizar a dimensao do bloco reservado */
				p->size = nunits;
			}
			freep = prevp;
			/* iniciar o bloco com zeros */
			memset(p + 1, 0, nbytes);
			/* incrementa a dimensão de memória alocada */
			allocated += p->size;
			/* marcar o bloco */
			p->next = (header_t) 0x87651234;
#ifdef	CHECK
			checkInsert(p);
#endif
			/* devolver o endereco de inicio do bloco */
			return (void *)(p + 1);
		}
		if (p == freep)
			/* Terminado o percurso completo da lista dos blocos livres,
			   sem que o pedido tenha sido satisfeito.
			   Memoria esgotada */
			return NULL;
	}
}

/*
 *	free: libertar um bloco de memoria
 */

void free(void *ap) {
	header_t bp, p;
	
	/* obter o apontador para o cabecalho do bloco */
	bp = (header_t)ap - 1;
	assert(bp->next == (header_t) 0x87651234);
#ifdef	CHECK
	checkRemove((void *)bp);
#endif
	/* decrementa a dimensão da memória alocada */
	allocated -= bp->size;
	/* percorrer a lista dos blocos livres, que esta' ordenada por ordem
	   crescente dos enderecos, para determinar a posicao de insercao do
	   bloco a libertar */
	for (p = freep; !(bp > p && bp < p->next);	p = p->next)
		if (p >= p->next && (bp > p || bp < p->next))
			break;	/* bloco posicionado no fim ou no inicio da lista */
	/* verificar se o bloco libertado compacta com o seguinte */
	if (bp + bp->size == p->next) {
		/* realizar a compactacao */
		bp->size += p->next->size;
		bp->next = p->next->next;
	} else
		bp->next = p->next;
	/* verificar se o bloco resultante do processamento anterior
	   pode compactar com o bloco anterior */
	if (p + p->size == bp) {
		/* compactar com o anterior */
		p->size += bp->size;
		p->next = bp->next;
	} else
		p->next = bp;
	freep = p;
}

/*
 *	Iniciar o gestor de memoria
 */
 
extern char __heap_start__;
extern char __heap_end__;

static int pool_size;

void malloc_init() {
	header_t up;
	
	/* iniciar a lista dos blocos livres, inserindo um bloco com
	   dimensao zero, que funciona como sentinela */
	base.next = freep = &base;
	base.size = 0;

	/* determinar a dimensao do bloco de memoria a atribuir ao gestor */

	up = (header_t)&__heap_start__; 
	pool_size = &__heap_end__ - &__heap_start__;
	up->size = pool_size / sizeof(struct header);
	up->next = (header_t) 0x87651234;
	allocated = up->size;
#ifdef	CHECK
	checkInsert(up);
#endif
	/* colocar o bloco reservado na lista dos blocos livres */
	free((void *)(up + 1));
}

#ifdef __BORLANDC__
#pragma startup malloc_init 2
#endif

/*
 *	Alocar um bloco de memória inicializada a zero
 */

void * calloc(size_t nelem, size_t size) {
	register unsigned i;
	register void * p;

	i = nelem * size;
	p = malloc(i);
	if ( p != NULL )
		memset(p, 0, i);
	return p;
}

static size_t memFree() {
	size_t free = 0;
	struct header * p;
	/*	Percorrer a lista dos blocos livres */
	for (p = base.next; p != &base; p = p->next)
		free += p->size;
	return free * sizeof(struct header);
}

static size_t memLarger() {
	size_t max = 0;
	struct header * p;
	for (p = base.next; p != &base; p = p->next)
		if (p->size > max)
			max = p->size;
	return max * sizeof(struct header); 
}

#ifdef	CHECK
void memPrint() {
	struct header * p;
	int i;
	cprintf("begin= %08x end= %d free= %d larger= %d allocated= %d used= %d%%\n\r",
		&heap_bottom, &heap_bottom + pool_size, memFree(), memLarger(), allocated, allocated * 100 / pool_size);
	cprintf("free list\n\r");
	for (i = 0, p = base.next; p != &base; p = p->next, ++i)
		cprintf("%4d start= %08x size= %d\n\r", i, p, p->size * sizeof(struct header));
	cprintf("allocated list\n\r");
	checkPrint();
}
#endif

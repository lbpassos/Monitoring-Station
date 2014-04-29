#include "snmp.h"
#include "wiznet.h"

#define SYSTEM_ID_SIZE	18

#define COMMUNITY_SIZE	6
#define OID_MAX_REQ		5

unsigned char searchOID_MIB(unsigned char sz)__attribute__((section(".text.fastcode")));
void send_ERROR( unsigned char error, unsigned char index )__attribute__((section(".text.fastcode")));
void buildLength( unsigned char *src, unsigned int value)__attribute__((section(".text.fastcode")));
unsigned int length_value(unsigned char *src)__attribute__((section(".text.fastcode")));
void sendResponse( unsigned char ans )__attribute__((section(".text.fastcode")));
void proc_agent_snmp()__attribute__((section(".text.fastcode")));



//static int inc_;

static const unsigned char systemID[SYSTEM_ID_SIZE+1] = "P109 - Agente SNMP";

static const unsigned char system_mib[NumberNodes] = { 43, MIB_DOD, MIB_INTERNET, MIB_PRIVATE, MIB_ENTERPRISE, IANA_IPL_UP, IANA_IPL_MID, IANA_IPL_LOW };
static GROUP_MIB enterprise_class[NumberGroups];


static const OID_VAR enterprise_Produto[NumberContents_PRODUTO] = {
				{1, RONLY, ASN_OCTET_STR}
			};
static const OID_VAR enterprise_Control[NumberContents_CONTROLO]  = {
				{1, RONLY, ASN_OCTET_STR},
				{2, RONLY, ASN_OCTET_STR},
				{3, RONLY, ASN_OCTET_STR},
				{4, RONLY, ASN_OCTET_STR},
				{5, RONLY, ASN_OCTET_STR}
			};
		

static const unsigned char community[COMMUNITY_SIZE+1] = "public";
static const Socket s;

static unsigned char *RX_ptr;
static unsigned char *TX_ptr;
static unsigned char RX_sz;
static unsigned char TX_sz;
static unsigned char length_pdu; //tamanho do PDU

static unsigned char *PDU_type; //tipo de pdu
static unsigned char *LENGTH_Build_ptr[4]; //primeiro posicao depois do OID
static unsigned int size_msg;
static unsigned int IP_dest;
static unsigned int PORT_dest;
static unsigned int SIZE_SNMP;

static OID oid[OID_MAX_REQ];

static float *u, *irms, *e; //variaveis
static char *temp, *humi;

//Iniciar modulo SNMP
void init_agent_snmp(unsigned char *RX_buf, unsigned char *TX_buf, unsigned char RX_size, unsigned char TX_size){

// Inicializar oid	
	enterprise_class[0].oid = MIB_ISEL_IPL_PRODUTO;
	enterprise_class[0].numberContents = NumberContents_PRODUTO;
	//enterprise_class[0].group = &enterprise_Produto;
	
	enterprise_class[1].oid = MIB_ISEL_IPL_CONTROLE;
	enterprise_class[1].numberContents = NumberContents_CONTROLO;
	//enterprise_class[1].group = &enterprise_Control;
		
	createUDP_socket( s, SNMP_PORT ); // Abrir um socket UDP
	
	RX_ptr = RX_buf;
	TX_ptr = TX_buf;
	RX_sz = RX_size; //tamanho da mensagem SNMP
	TX_sz = TX_size;
}

/* *********************** Iniciar variaveis ****
 * 
 */
void initVariables_snmp(float *U, float *I, float *E, char *t, char *h){
	u = U;
	irms = I;
	e = E;
	temp = t;
	humi = h;
}

//So imprimir
unsigned char printVar(unsigned char num, unsigned char *ptr){
	unsigned int i = 10;
	unsigned char j = 0;
	
	if(num){
		while( num>i )
			i *= 10;
		while( num ){
			i /= 10;
			*ptr++ = num/i + '0';
			num %= i;
			++j;
		}
		j -= 1;
	}
	else
		*ptr = '0';
	return j+1;
}
	
// Converter para string para envio
unsigned char float_toChar(unsigned char *dest, float *ptr, unsigned char precision){
	
	unsigned int index = 0;
	int v_int;
	
	v_int = *ptr;
	if( v_int<0 ){ // por o sinal negativo
		*dest = '-';
		v_int *= -1;
		++index;
	}
	index += printVar(v_int, dest+index);
	switch( precision ){
		case 0:
			break;
		case 1:
			precision = 0x0A; //10
			break;
		case 2:
			precision = 0x64; //100
			break;
		default:
			precision = 0;
	}
			
	v_int = (*ptr-v_int) * precision;
	if( v_int ){
		*( dest + index++ ) = '.';
		if( v_int>0 && v_int<10 && precision>10 ){
			while( precision>10 ){
				*( dest + index++ ) = '0';
				precision /= 10;
			}
			*( dest + index++ ) = v_int + '0';
		}
		else
			index += printVar(v_int, dest+index);
	}

	return index;
}
				
		
		

// Converter para string para envio
unsigned char char_toString(unsigned char *dest, unsigned char *ptr){
	unsigned int i = 1;
	unsigned int j = 0;
	unsigned int index = 0;
		
	if( *ptr<0 ){ // por o sinal negativo
		*dest = '-';
		*ptr *= -1;
		++index;
	}
	index += printVar(*ptr, dest+index);
	++ptr;
	if( *ptr ){
		*( dest + index++ ) = '.';
		index += printVar(*ptr, dest+index);
	}
	return index;
}

/* *************************** Trama SNMP
 * 
 */
 
 /*Trabalha o header: src - buffer
					 asn_type - asn_type 
					 length(ou erro)
			  Retorna: ptr para os dados. 0x00 se erro
*/
unsigned char *parseHeader(unsigned char *src, unsigned char *asn_type, unsigned int *length){
	
	unsigned int len, size_len, i;
	
	*asn_type = *src++; //Tipo de dados
	len = *src++;
	
	if( len>127 ){
		len -= 128;
		if( len>4 ){
			*length = SNMP_ERR_BADVALUE; //So suporta 32bits
			return 0;
		}
		size_len = len; 
		len = 0;
		for(i=0; i<size_len; ++i){
			len <<= 8;
			len |= *src++;
		}
	}
	*length = len;
	return src;
}

 
	

/* Procurar OID no MIB:	sz - Num de OIDs
 * 					Retorna: 0 (OK)   n(Index do erro)
 * 
 */
unsigned char searchOID_MIB(unsigned char sz){
	unsigned char i, j, l;
	
	for(i=0; i<sz; ++i){
		for(j=0; j<NumberNodes; ++j) //system
			if( oid[i].id[j] != system_mib[j] )
				return i+1;
		for(j=0; j<NumberGroups; ++j){ //classe
			if( oid[i].id[NumberNodes] == enterprise_class[j].oid )
				break;
		}
		if( j== NumberGroups )
			return i+1;
		
		switch(j+1){ //Variaveis
			case 1: //Produto
				for(l=0; l<NumberContents_PRODUTO; ++l)
					if( oid[i].id[NumberNodes+1] == enterprise_Produto[l].id )
						break;	
				if(l==NumberContents_PRODUTO)
					return i+1;
				break;
			case 2: //Control
				for(l=0; l<NumberContents_CONTROLO; ++l)
					if( oid[i].id[NumberNodes+1] == enterprise_Control[l].id )
						break;
				if(l==NumberContents_CONTROLO)
					return i+1;
				break;
		}
			
		if( oid[i].id[NumberNodes+1+1] != 0 )
			return i+1;
	}
	return 0;
}
			
/* Enviar msg erro:	Tipo de Erro
 * 					 Index
 ****************************************
 */ 		
void send_ERROR( unsigned char error, unsigned char index ){
	
	unsigned char *ptr;
	unsigned char t;
	unsigned int len;
	
	ptr = LENGTH_Build_ptr[1]; //LEN PDU
	*(ptr) = MSG_GET_RESPONSE+ASN_CONTEXT+ASN_FORMAT; //GetResponse	
	
	ptr = parseHeader(ptr, &t, &len);
	
	//RID
	ptr = parseHeader(ptr, &t, &len);
	ptr += len;
	
	//ERROR
	ptr = parseHeader(ptr, &t, &len);
	*ptr = error;
	
	//ERROR_INDEX
	ptr += len;
	ptr = parseHeader(ptr, &t, &len);
	*ptr = index;
	
	sendtoUDP_socket(s, RX_ptr, SIZE_SNMP, IP_dest, PORT_dest); // Enviar data (UDP/IP RAW)

}

/* Criar LENGTH:	endereco
 * 					valor do length
 ****************************************
 */ 
void buildLength( unsigned char *src, unsigned int value){
	
	unsigned char len_value, i;
	
	len_value = *src;
	
	if( len_value>127 )
		len_value -= 128;
	else len_value = 1;
	
	for(i=len_value; i!=0; --i)
		*src++ = (value>>((len_value-1)*8)) & 0xFF;
}

// Detectar o Valor de Length
unsigned int length_value(unsigned char *src){
	
	unsigned char len, i;
	unsigned int value = 0;
	
	len = *src;
	if( len>127 )
		len -= 128;
	else len = 1;
	
	for( i=0; i<len; ++i){
		value <<= 8;
		value |= *src++;
	}
	
	return value;
}
	
	


		
/* Enviar msg resposta:	Num de respostas a produzir
 * 					 Index
 ****************************************
 */ 
void sendResponse( unsigned char ans ){
	
	unsigned char *ptr, *ptr_aux;
	unsigned char i, j, size, size_VARBIND_LIST, value_taken;
	unsigned char value[OID_MAX_REQ][SYSTEM_ID_SIZE+2];
	unsigned int length, length_aux, size_tmp;
	
	ptr = LENGTH_Build_ptr[2]+1;//LEN_VARBIND_LIST
	size = value_taken = 0;
	size_VARBIND_LIST = *ptr;
	//size_VARBIND = *(LENGTH_Build_ptr[3]+1);
	
	for(i=0; i<ans; ++i){
		switch( oid[i].id[8] ){
			case 1: //Produto
				switch( oid[i].id[9] ){ //variavel
					case 1: // Nome
						value[i][0] = ASN_OCTET_STR+ASN_UNIVERSAL;
						value[i][1] = SYSTEM_ID_SIZE;
						for(j=0; j<SYSTEM_ID_SIZE; ++j)
							value[i][2+j] = systemID[j];
						size += 2 + j;
						break;
				}
				break;
			case 2: //Controlo
				switch( oid[i].id[9] ){ //variavel
					case MIB_ISEL_IPL_CONTROLE_U: //U
						value[i][0] = ASN_OCTET_STR+ASN_UNIVERSAL;
						size_tmp = float_toChar( &value[i][2], u, 0);
						value[i][1] = size_tmp; //XXX.X
						size += 2 + size_tmp;	
						break;		
					case MIB_ISEL_IPL_CONTROLE_I:
						value[i][0] = ASN_OCTET_STR+ASN_UNIVERSAL;
						size_tmp = float_toChar( &value[i][2], irms, 2);
						value[i][1] = size_tmp; //XX.X
						size += 2 + size_tmp;
						break;		
					case MIB_ISEL_IPL_CONTROLE_E:
						value[i][0] = ASN_OCTET_STR+ASN_UNIVERSAL;
						size_tmp = float_toChar( &value[i][2], e, 1);
						value[i][1] = size_tmp; //XXX.X
						size += 2 + size_tmp;
						break;		
					case MIB_ISEL_IPL_CONTROLE_T:
						value[i][0] = ASN_OCTET_STR+ASN_UNIVERSAL;
						size_tmp = char_toString(&value[i][2], temp);
						value[i][1] = size_tmp; //XX.X
						size += 2 + size_tmp;
						break;		
					case MIB_ISEL_IPL_CONTROLE_H:
						value[i][0] = ASN_OCTET_STR+ASN_UNIVERSAL;
						size_tmp = char_toString(&value[i][2], humi);
						value[i][1] = size_tmp; //XXX.X
						size += 2 + size_tmp;
						break;		
				}
				break;
		}
	value_taken += 2;
	}//end_for
	
	size_msg = size_msg - value_taken + size;
	//size_msg += size;
	if( size_msg <= SNMP_MAX_LEN){
		ptr = LENGTH_Build_ptr[0];
		buildLength( ptr+1, size_msg );
		
		ptr = LENGTH_Build_ptr[1];
		*ptr = MSG_GET_RESPONSE+ASN_CONTEXT+ASN_FORMAT; //GetResponse
		buildLength( ptr+1, length_value(ptr+1)-value_taken+size );
		
		ptr = LENGTH_Build_ptr[2];
		buildLength( ptr+1, length_value(ptr+1)-value_taken+size );
	
		ptr = LENGTH_Build_ptr[3];
		buildLength( ptr+1, length_value(ptr+1)-value_taken+size );
		
		ptr = LENGTH_Build_ptr[3];
		ptr = parseHeader(ptr, &size_VARBIND_LIST, &length_aux);//OID
		
		for(i=0; i<length_aux; ){
			ptr = parseHeader(ptr, &size_VARBIND_LIST, &length);
			ptr_aux = ptr + length;
			for(j=0; j<value[i][1]+2; ++j)
				*ptr_aux++ = value[i][j];
			
			i += ptr_aux-ptr+j;
			ptr = ptr_aux + j; 
		}
	}
	else
		send_ERROR( SNMP_ERR_TOOBIG, 0 );
	
	
	length = parseHeader(RX_ptr, &size_VARBIND_LIST, &length) - RX_ptr +  size_msg;	
	sendtoUDP_socket(s, RX_ptr, length, IP_dest, PORT_dest); // Enviar data (UDP/IP RAW)

}
					
	
		


// Processar agente
void proc_agent_snmp(){
	
	unsigned char asn_type; //ASN.1
	unsigned int length; //size
	
	 //tamnho
	unsigned char size_varbind, size_dataRx;
	//unsigned char size_OID;
	unsigned char i, j, k, numberOfRequests;
	unsigned char *ptr_data, *aux_ptr, *ptr_msg;
	
	if( !SnIRQ_wiznet( s, SOCK_IRQ_RECV ) )
		return;
	
	size_dataRx = getSn_RXsize_wiznet( s );
	if( size_dataRx==0 )
		return;
	if( size_dataRx < 8){
		recvData_wiznet( s, RX_ptr, size_dataRx);
		return;
	}
	/* tratar a trama */
	
	recvData_wiznet( s, RX_ptr, 8);
	
	ptr_msg = RX_ptr;
	IP_dest = *ptr_msg++;
	IP_dest <<= 8;
	IP_dest |= *ptr_msg++;
	IP_dest <<= 8;
	IP_dest |= *ptr_msg++;
	IP_dest <<= 8;
	IP_dest |= *ptr_msg++;

	PORT_dest = *ptr_msg++;
	PORT_dest <<= 8;
	PORT_dest |= *ptr_msg++;

	SIZE_SNMP = *ptr_msg++;
	SIZE_SNMP <<= 8;
	SIZE_SNMP |= *ptr_msg++;
	
	if( SIZE_SNMP>SNMP_MAX_LEN ){
		send_ERROR( SNMP_ERR_TOOBIG, 0 );
		while(1); //forca watchdog
	}
	if( SIZE_SNMP >= size_dataRx ){
		recvData_wiznet( s, RX_ptr, size_dataRx);
		return;
	}
	if( SIZE_SNMP )
		recvData_wiznet( s, RX_ptr, SIZE_SNMP);
	else
		return;
	
	//++inc_;

	//if(inc_==37)
		//RX_ptr += 0; 
	
	ptr_msg = RX_ptr;
	
	LENGTH_Build_ptr[0] = ptr_msg; //SEQ da msg
	ptr_data = parseHeader(ptr_msg, &asn_type, &length);
	if( ptr_data == 0x00){
		send_ERROR( length, 0 );   ////////////////////////////////
		return;
	}
	size_msg = length; //tamanho msgSNMP
		
	//Procurar versao	
	ptr_data = parseHeader(ptr_data, &asn_type, &length);
	if( ptr_data == 0x00){
		send_ERROR( length, 0 );   ////////////////////////////////
		return;
	}
	if( *ptr_data!= SNMP_VERSION_1)
		return;
	++ptr_data;
		
	//Procurar community
	ptr_data = parseHeader(ptr_data, &asn_type, &length);
	if( ptr_data == 0x00){
		send_ERROR( length, 0 );   ////////////////////////////////
		return;
	}
	if( length != COMMUNITY_SIZE )
		return;
	for(i=0; i<length; ++i)
		if( *(ptr_data+i) != community[i] )
			return;
	
	ptr_data += length;
	
	/* PDU */
	LENGTH_Build_ptr[1] = ptr_data;
	//Procurar GetRequest
	ptr_data = parseHeader(ptr_data, &asn_type, &length);
	if( ptr_data == 0x00){
		send_ERROR( length, 0 );
		return;
	}
	

	if( asn_type == (MSG_GET_REQUEST+ASN_CONTEXT+ASN_FORMAT) ){ //GetRequest
		
		//Procurar RID
		ptr_data = parseHeader(ptr_data, &asn_type, &length);
		if( ptr_data == 0x00 ){
			send_ERROR( length, 0 );
			return;
		}
		ptr_data += length;
		
		//Procurar ERR
		ptr_data = parseHeader(ptr_data, &asn_type, &length);
		if( ptr_data == 0x00){
			send_ERROR( length, 0 );
			return;
		}
		ptr_data += length;
		
		//Procurar ERROR INDEX
		ptr_data = parseHeader(ptr_data, &asn_type, &length);
		if( ptr_data == 0x00){
			send_ERROR( length, 0 );   
			return;
		}
		ptr_data += length;
		LENGTH_Build_ptr[2] = ptr_data; //SEQ VARBIND_LIST
		
		//Procurar VARBIND LIST
		ptr_data = parseHeader(ptr_data, &asn_type, &length);
		if( ptr_data == 0x00){
			send_ERROR( length, 0 );
			return;
		}
		LENGTH_Build_ptr[3] = ptr_data; //SEQ VARBIND
		
		//Procurar VARBIND
		ptr_data = parseHeader(ptr_data, &asn_type, &length);
		if( ptr_data == 0x00){
			send_ERROR( length, 0 );
			return;
		}
		size_varbind = length;
		
		//Procurar OIDs
		i=0; j=0; numberOfRequests=0;
		while( i<size_varbind ){
			aux_ptr = ptr_data;
			ptr_data = parseHeader(ptr_data, &asn_type, &length);
			if( asn_type == ASN_OBJECT_ID+ASN_UNIVERSAL ){
				if( length != (NumberNodes+1+1+1) ){
					send_ERROR( SNMP_ERR_NOSUCHNAME, j );
					return;
				}			
				if( ++numberOfRequests>OID_MAX_REQ ){
					send_ERROR( SNMP_ERR_GENERR, 0 );
					return;
				}
				for(k=0; k<length; ++k)
					oid[j].id[k] = *(ptr_data+k);
				++j;
			}
			ptr_data += length + 2; // Value=0x00 Len=0
			i += ptr_data - aux_ptr;		
		}
		
		i = searchOID_MIB(numberOfRequests);
		if( i ){
			send_ERROR( SNMP_ERR_NOSUCHNAME, i-1 );
			return;
		}
		
		sendResponse(numberOfRequests);
	}
}

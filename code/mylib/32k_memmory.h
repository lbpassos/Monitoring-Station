
void init_memmory(void); 
unsigned int size_memmory(void);
int write_memmory(unsigned int addr, unsigned char data);
void writePage_memmory(unsigned char page, unsigned char addr, unsigned char *data, unsigned char dim);
int read_memmory(unsigned int addr, unsigned char *data, int dim);



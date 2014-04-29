#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>

#if !defined(__CONFIG_H)
#include <_config.h>
#endif

#ifndef NULL
#define NULL	_NULL
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef _size_t size_t;
#endif

#define _IOFBF  0
#define _IOLBF  1
#define _IONBF  2

#define FILENAME_MAX	_FNAME
#define FOPEN_MAX		_NFILE
#define L_tmpnam		_TNAME

#define BUFSIZ			512

#define	EOF				(-1)

#define SEEK_SET		0
#define SEEK_CUR		1
#define SEEK_END		2

#define TMP_MAX			0xFFFF

typedef long			fpos_t;

/* Definition of the control structure for streams
*/
typedef struct  {
		int			 level;		  /* fill/empty level of buffer */
		unsigned		flags;		  /* File status flags		  */
		char			fd;			 /* File descriptor			*/
		unsigned char   hold;		   /* Ungetc char if no buffer   */
		int			 bsize;		  /* Buffer size				*/
		unsigned char   *buffer;   		/* Data transfer buffer	   */
		unsigned char   *curp;	 		/* Current active pointer	 */
		unsigned		istemp;		 /* Temporary file indicator   */
		short		   token;		  /* Used for validity checking */
}	   FILE;						   /* This is the FILE object	*/

/*  "flags" bits definitions
*/
#define _F_RDWR 0x0003				  /* Read/write flag	   */
#define _F_READ 0x0001				  /* Read only file		*/
#define _F_WRIT 0x0002				  /* Write only file	   */
#define _F_BUF  0x0004				  /* Malloc'ed Buffer data */
#define _F_LBUF 0x0008				  /* line-buffered file	*/
#define _F_ERR  0x0010				  /* Error indicator	   */
#define _F_EOF  0x0020				  /* EOF indicator		 */
#define _F_BIN  0x0040				  /* Binary file indicator */
#define _F_IN   0x0080				  /* Data is incoming	  */
#define _F_OUT  0x0100				  /* Data is outgoing	  */
#define _F_TERM 0x0200				  /* File is a terminal	*/

#ifdef __cplusplus
extern "C" {
#endif

void	clearerr(FILE * stream);
int		fclose(FILE * stream);
int		feof(FILE * fp);
int		ferror(FILE * fp);
int		fflush(FILE * stream);
int		fgetc(FILE * stream);
int		fgetpos(FILE * stream, fpos_t * pos);
char *	fgets(char * s, int n, FILE * stream);
FILE *	fopen(const char * path, const char * mode);
int		fprintf(FILE * stream, const char * format, ...);
int		fputc(int c, FILE * stream);
int		fputs(const char * s, FILE * stream);
size_t	fread(void * ptr, size_t size, size_t n, FILE * stream);
FILE *	freopen(const char * path, const char * mode, FILE * stream);
int		fscanf(FILE * stream, const char * format, ...);
int		fseek(FILE * stream, long offset, int whence);
int		fsetpos(FILE * stream, const fpos_t * pos);
long	ftell(FILE * stream);
size_t	fwrite(const void * ptr, size_t size, size_t n, FILE * stream);
int		getc(FILE * fp);
int		getchar(void);
char *	gets(char * s);
void	perror(const char * s);
int		printf(const char * format, ...);
int		putc(const int c, FILE * fp);
int		putchar(const int c);
int		puts(const char * s);
int		remove(const char * path);
int		rename(const char * oldname,const char * newname);
void	rewind(FILE * stream);
int		scanf(const char * format, ...);
void	setbuf(FILE * stream, char * buf);
int		setvbuf(FILE * stream, char * buf, int type, size_t size);
int		sprintf(char * buffer, const char * format, ...);
int 	nsprintf (char * s, size_t size, const char * fmt, ...);
int		sscanf(const char * buffer, const char * format, ...);
char *	strerror(int errnum);
FILE *	tmpfile(void);
char *	tmpnam(char * s);
int		ungetc(int c, FILE * stream);
int		vfprintf(FILE * stream, const char * format, va_list arglist);
int		vfscanf(FILE * stream, const char * format, va_list arglist);
int		vprintf(const char * format, va_list arglist);
int		vsprintf(char * buffer, const char * format, va_list arglist);
int		vscanf(const char * format, va_list arglist);
int		vsscanf(const char * buffer, const char * format, va_list arglist);

#ifdef __cplusplus
}
#endif

extern FILE _streams[];

#define stdin			(&_streams[0])
#define stdout			(&_streams[1])
#define stderr			(&_streams[2])

#define getc(f) \
  ((--((f)->level) >= 0) ? (unsigned char)(*(f)->curp++) : \
    fgetc (f))

#define putc(c,f) \
  ((++((f)->level) < 0) ? (unsigned char)(*(f)->curp++=(c)) : \
    fputc ((c),f))

#define getchar()  getc(stdin)
#define putchar(c) putc((c), stdout)

#define ungetc(c,f) ungetc((c),f)   /* traditionally a macro */

#endif

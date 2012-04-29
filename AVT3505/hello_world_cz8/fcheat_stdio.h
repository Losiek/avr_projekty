//////////////////////////////////////////////////////////////////////////////////////////
// stdio_fcheat.h - plik pomagaj¹cy w pozbyciu siê funkcji fdevopen
//		Uwaga: Dzia³anie zapewnione tylko dla avr-libc 1.2.3
// Na 99% konieczna bedzie zmiana tego pliku (a mo¿e stanie siê zbêdny) przy nowszej kompilacji WinAVR
//
// Autor: Rados³aw Koppel	Kompilator: WinAVR 20050214
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef FCHEAT_STDIO_INCLUDED
#define FCHEAT_STDIO_INCLUDED

// Struktura odpowiadaj¹ca FILE
typedef struct fcheat_file
{
	char	*buf;		/* buffer pointer */
	unsigned char unget;	/* ungetc() buffer */
	uint8_t	flags;		/* flags, see below */
#define FCHEAT_SRD	0x0001		/* OK to read */
#define FCHEAT_SWR	0x0002		/* OK to write */
	int	size;		/* size of buffer */
	int	len;		/* characters read or written so far */
	int	(*put)(char);	/* function to write one char to device */
	int	(*get)(void);	/* function to read one char from device */
}fcheat_file;

#define FCHEAT_STATIC_FDEVOPENR(get) \
{ 0, 0, FCHEAT_SRD, 0, 0, 0, get }

#define FCHEAT_STATIC_FDEVOPENW(put) \
{ 0, 0, FCHEAT_SWR, 0, 0, put, 0 }

#define FCHEAT_STATIC_FDEVOPENWR(put, get) \
{ 0, 0, FCHEAT_SWR | FCHEAT_SRD, 0, 0, put, get }

#endif //FCHEAT_STDIO_INCLUDED

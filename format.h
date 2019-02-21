#ifndef format_h
#define format_h

static void join(char*  s,char c);
void intToDecASCII(int , char* );
void intToASCII(int, char*);
int binaryToInt(int , char*);
char* flipBits(char*);
static void getFloatDigit( float , char *, int *, float *);
void floatToASCII( float, char *);

#endif

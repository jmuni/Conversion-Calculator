#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <ctype.h>
#include "format.h"


//global var for max input size and output type
unsigned int max_size = 32;

union Number {
	int	type_int;
	float	type_float;
};

int main(int argc, char *argv[]) {

//variable declarations
    char* input;
    char* type;
    int i, j;
    int result;
    char* output = malloc(max_size*sizeof(char));

    for(i = 0; i < max_size; i++) { 
        output[i] = '\0';
    }

//check for proper input size
    if(argc < 3 || argc > 3) {
	fprintf(stderr, "%s\n", "ERROR: Only two arguments allowed");
        exit(0);
    }

    for(i = 0; i < argc; i++) {

	switch(i){
	    case 0 : break;

	//checks first input for valid input and size
	    case 1 : input = argv[i];
		if(strlen(input) != 32) {
		    fprintf(stderr, "%s\n", "ERROR: Only bits of size 32 allowed");
		    exit(0);
		}
		for(j = 0; j < 32; j++) {
		    if(input[j] != '1' && input[j] != '0') {
			fprintf(stderr, "%s\n", "ERROR: Only 1's and 0's allowed in input");
			exit(0);
		    }
		}
		break;

	//checks second input for valid type
	    case 2 : type = argv[i];
		if((strcmp(type, "int") == 1 && strcmp(type, "float") == 1)) { 
	    	    fprintf(stderr, "%s\n", "ERROR: Invalid type! Must be int or float");
		}
		break;

		default : fprintf(stderr, "%s\n", "ERROR: Invalid number of arguments.");

	}// end switch
    } 

//checks if type is int and converts appropriately 
    if(strcmp(type, "int") == 0) {
	result = binaryToInt(1, input);
	
	if(result < 0) {
	    join(output, '-');
	    result = result *  -1; //number becomes positive
	}
	intToASCII(result, output);
	printf("\n%s\n", output );
    }

//checks if type is float and converts appropriately
    if(strcmp(type, "float") == 0) {		
	union Number num;
	num.type_int = 0;
	j = 0;
	char * ch = argv[1];

   	while(*ch != '\0'){
     	    num.type_int = num.type_int << 1;
     	    num.type_int += *ch - '0';
     	    ch++;
    	}
    		
	floatToASCII(num.type_float, output);

        if(num.type_float == -0.0 && input[0] == '1') {
	    printf("\n-%s\n", output);
        }else {
            printf("\n%s\n", output);
        }

    }

  return 0;

} // end main

static void join(char *s, char c){

    char buf[2];

    buf[0] = c;
    buf[1] = '\0';
    strcat(s, buf);
} // end join

void intToDecASCII(int i, char* output) {
    
    char* str;
    char sign = '+';
    int j;
    int number = 0;
    int length = 12;
    int negative;

    char temp[length];

    for(j = 0; j < (length-2); j++) {
	temp[j] = '0';
    }
	
    temp[length-1] = '\0';

    if(i < 0) {
	negative = 1; // true for negative
    }else {
	negative = 0; // false for negative, so positive
    }

    switch(negative) { 
	case 0 : sign = '+'; //if positive, it is fine
	i = i * 1;
	break;

	case 1 : sign = '-'; //if negative the int is flipped
	i = i * -1;
	break;
    } //end switch

    number = length - 2;
    //printf("%s%d\n", "number: " ,number);
    while(number >= 0) {
	temp[number] = (i % 10) + '0';
	i = i / 10;

	if(i == 0) {
	break;
	}
	number--;
    }

	if(sign == '-') {
	    number--;
	    temp[number] = '-';
	}

	str = output;
	while(number < 12) {
		*str = temp[number];
		str++;
		number++;
	}

} // end function intToDecASCII

void intToASCII(int number, char* output) {
	
    char result = '0';

    if(number != 0){
	result = ((number % 10)+'0');
	intToASCII((number/10),output);
	join(output, result);
     }else {
	return;
     }

} //end intToASCII

int binaryToInt(int number, char* output) {

    int j; 
    int flip;
    int result = 0;

    if(output[0] == '1' && number == 1){
	output = flipBits(output);
	flip = 1;
    }

    for(j = (strlen(output)-1); j > 0; j--) {
	if(output[j]=='1'){
	result += pow(2,((strlen(output)-j-1)));
	}
    }

    if(flip == 1) {
	result += 1;
	result *=-1;
    }

    return result;

} //end binaryToInt

char* flipBits(char* number) {

    int i;

    for(i = 1; i < strlen(number); i++) {
    if(number[i] == '1') {
	number[i] = '0';
    }else if(number[i] == '0') {
	number[i] = '1';
    }
    }

	return number;
} // end flipBits

static void getFloatDigit(float x, char * digit, int * pow10, float * rem ){
	
    int p10;

    if (x == 0.0) {
	*digit = '0';			// digit is always '0'
	*pow10 = 0;
	*rem = 0.0;
    }else {
	*pow10 = 0;			// start with 10^0
	while (x >= 10.0) {		// reduce
	    x /= 10.0;		// large values get smaller
	    *pow10 += 1;
	}
	while (x < 1.0)	{	// increase
	    x *= 10.0;		// small values get larger
	    *pow10 -= 1;
	}
	*digit = '0';
	do {				// 10.0 > x >= 1.0
	    x -= 1.0;
	    *digit += 1;		// digit is always non-'0'zero
	} while ( x >= 1.0 );
	    p10 = 0;
	while (p10 > *pow10) {	// leading digit is now removed from x
	    x /= 10;
	    p10 -= 1;
	}
	while (p10 < *pow10) {
	    x *= 10;
	    p10 += 1;
	}
	    *rem = x;
	}
} //end getFloatDigits 

void floatToASCII( float x, char * output ) {
	
    char c;
    int pow10, p10, plast;
    int i;
    float rem;
    char exponent[10];
    union Number a;
    unsigned int biasedExp;
    unsigned int mantissa;
    int sign;

    a.type_float = x;
    biasedExp = a.type_int >> 23 & 0x000000ff;
    mantissa = a.type_int & 0x007fffff;
    sign = a.type_int >> 31;

// +/- inf, +/- Nan stuff here
    if(biasedExp == 0xff) {
	if(mantissa == 0) {
	    if(sign == 0) {
		strcpy(output, "pinf");
		return;
	    }else {
		strcpy(output, "ninf");
		return;
	    }
	} //mantissa 
	else {
		strcpy(output, "NaN");
		return;
  	    }
    } //biasedExp

    output[0] ='\0';
    if (x < 0.0) {
	join( output, '-' );
	x = -x;					// make x positive
    }
	
    getFloatDigit(x, &c, &pow10, &rem );
    join( output, c );
    join( output, '.' );
    x = rem;
    plast = p10 = pow10;			// pow10 set by get_float_digit()
    for ( i = 1 ; i < 7 ; i++ )		// 7 significant digits in 32-bit float
    {
	getFloatDigit( x, &c, &p10, &rem );
	if ((plast - p10) > 1 ) {
	    join( output, '0' );	// fill in zero to next nonzero digit
	    plast -= 1;
	}else {
	    join( output, c );
	    x = rem;
	    plast = p10;
	}
    }

    if ( pow10 < 0 )		// negative exponent
    {
	exponent[0] = 'e';
	intToDecASCII( pow10, exponent+1 );
    }else if ( pow10 < 10 )		// positive single-digit exponent
	{
	    exponent[0] = 'e';
	    exponent[1] = '+';
	    exponent[2] = '0';
	    intToDecASCII( pow10, exponent+3 );
	} else				// positive multi-digit exponent
	{
	    exponent[0] = 'e';
	    exponent[1] = '+';
	    intToDecASCII( pow10, exponent+2 );
	}
	
    strcat( output, exponent );
} //end floatToASCII


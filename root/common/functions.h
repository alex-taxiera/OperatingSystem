#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

unsigned int get_byte ( const unsigned char *buffer, unsigned int index );
unsigned int get_word ( const unsigned char *buffer, unsigned int index );
unsigned int get_dword( const unsigned char *buffer, unsigned int index );

void set_byte ( unsigned char *buffer, unsigned int index, unsigned int val );
void set_word ( unsigned char *buffer, unsigned int index, unsigned int val );
void set_dword( unsigned char *buffer, unsigned int index, unsigned int val );

void strtoupper( char *str );
void strtolower( char *str );

#endif // FUNCTIONS_H

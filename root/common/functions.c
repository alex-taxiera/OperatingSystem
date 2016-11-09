#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

unsigned int get_byte( const unsigned char *buffer, unsigned int index ) 
{
	return buffer[index];
}

unsigned int get_word( const unsigned char *buffer, unsigned int index ) 
{
	static int msb, lsb;
	lsb =   ( (int) buffer[index] )            & 0x000000FF;
	msb = ( ( (int) buffer[index + 1] ) << 8 ) & 0x0000FF00;
	return lsb | msb;
}

unsigned int get_dword( const unsigned char *buffer, unsigned int index ) 
{
	static int hmsb, hlsb, lmsb, llsb;
	llsb =   ( (int) buffer[index] )             & 0x000000FF;
	lmsb = ( ( (int) buffer[index + 1] ) << 8  ) & 0x0000FF00;
	hlsb = ( ( (int) buffer[index + 2] ) << 16 ) & 0x00FF0000;
	hmsb = ( ( (int) buffer[index + 3] ) << 24 ) & 0xFF000000;
	return llsb | lmsb | hlsb | hmsb;
}

void set_byte( unsigned char *buffer, unsigned int index, unsigned int val ) 
{
	buffer[index] = val;
}

void set_word( unsigned char *buffer, unsigned int index, unsigned int val ) 
{
	static int msb, lsb;
	lsb =  val & 0x000000FF;
	msb = (val & 0x0000FF00) >> 8;
	buffer[index]     = lsb;
	buffer[index + 1] = msb;
}

void set_dword( unsigned char *buffer, unsigned int index, unsigned int val ) 
{
	static int hmsb, hlsb, lmsb, llsb;
	llsb =  val & 0x000000FF;
	lmsb = (val & 0x0000FF00) >> 8;
	hlsb = (val & 0x00FF0000) >> 16;
	hmsb = (val & 0xFF000000) >> 24;
	buffer[index]     = llsb;
	buffer[index + 1] = lmsb;
	buffer[index + 2] = hlsb;
	buffer[index + 3] = hmsb;
}

void strtoupper( char *str ) 
{
	int i;
	for (i = 0; i < strlen(str); ++i)
		str[i] = toupper(str[i]);
}

void strtolower( char *str ) 
{
	int i;
	for (i = 0; i < strlen(str); ++i)
		str[i] = tolower(str[i]);
}

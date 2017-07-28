#ifndef BASE32_H
#define BASE32_H

const char *Hexadecimal ="0123456789ABCDEF";

const char *RFC_4648 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
/****** RFC_4648 *************
0	A	9	J	18	S	27	3
1	B	10	K	19	T	28	4
2	C	11	L	20	U	29	5
3	D	12	M	21	V	30	6
4	E	13	N	22	W	31	7
5	F	14	O	23	X	
6	G	15	P	24	Y
7	H	16	Q	25	Z
8	I	17	R	26	2	pad	=
******************************/

const char *RFC_2045 ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
/********** RFC_2045 ***************************************************************
 Valeur Codage         Valeur Codage         Valeur Codage         Valeur Codage
      0 000000 A           17 010001 R           34 100010 i           51 110011 z
      1 000001 B           18 010010 S           35 100011 j           52 110100 0
      2 000010 C           19 010011 T           36 100100 k           53 110101 1
      3 000011 D           20 010100 U           37 100101 l           54 110110 2
      4 000100 E           21 010101 V           38 100110 m           55 110111 3
      5 000101 F           22 010110 W           39 100111 n           56 111000 4
      6 000110 G           23 010111 X           40 101000 o           57 111001 5
      7 000111 H           24 011000 Y           41 101001 p           58 111010 6
      8 001000 I           25 011001 Z           42 101010 q           59 111011 7
      9 001001 J           26 011010 a           43 101011 r           60 111100 8
     10 001010 K           27 011011 b           44 101100 s           61 111101 9
     11 001011 L           28 011100 c           45 101101 t           62 111110 +
     12 001100 M           29 011101 d           46 101110 u           63 111111 /
     13 001101 N           30 011110 e           47 101111 v
     14 001110 O           31 011111 f           48 110000 w        (complément) =
     15 001111 P           32 100000 g           49 110001 x
     16 010000 Q           33 100001 h           50 110010 y
***********************************************************************************/

const char *base64url ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
/********** base64url ***************************************************************
 Valeur Codage         Valeur Codage         Valeur Codage         Valeur Codage
      0 000000 A           17 010001 R           34 100010 i           51 110011 z
      1 000001 B           18 010010 S           35 100011 j           52 110100 0
      2 000010 C           19 010011 T           36 100100 k           53 110101 1
      3 000011 D           20 010100 U           37 100101 l           54 110110 2
      4 000100 E           21 010101 V           38 100110 m           55 110111 3
      5 000101 F           22 010110 W           39 100111 n           56 111000 4
      6 000110 G           23 010111 X           40 101000 o           57 111001 5
      7 000111 H           24 011000 Y           41 101001 p           58 111010 6
      8 001000 I           25 011001 Z           42 101010 q           59 111011 7
      9 001001 J           26 011010 a           43 101011 r           60 111100 8
     10 001010 K           27 011011 b           44 101100 s           61 111101 9
     11 001011 L           28 011100 c           45 101101 t           62 111110 - (minus)
     12 001100 M           29 011101 d           46 101110 u           63 111111 _ (underline)
     13 001101 N           30 011110 e           47 101111 v
     14 001110 O           31 011111 f           48 110000 w        (complément) =
     15 001111 P           32 100000 g           49 110001 x
     16 010000 Q           33 100001 h           50 110010 y
***********************************************************************************/


void encode_baseX( uint8_t output_bit , byte *output_array , uint8_t input_bit , byte *input_array , size_t array_size , const char *translate )
{
	size_t array_bit_size = array_size * input_bit;
	for( size_t idx = 0 ; idx <= array_bit_size ; idx++ )
	{
		size_t array_byte_idx = idx / input_bit;
		size_t byte_bit_idx   = idx % input_bit;
		size_t baseX_elem_idx = idx / output_bit;
		size_t baseX_bit_idx  = idx % output_bit;
		
		if ( baseX_bit_idx == 0 )
		{
			output_array[baseX_elem_idx]   = 0;
			output_array[baseX_elem_idx+1] = 0;
		}
		
		//byte bit_mask = 1 << byte_bit_idx;
		byte bit_mask = 0x80 >> byte_bit_idx;
		if ( ( input_array[array_byte_idx] & bit_mask ) > 0 )
		{
			//output_array[baseX_elem_idx] = output_array[baseX_elem_idx] | ( 1 << baseX_bit_idx );
			output_array[baseX_elem_idx] = output_array[baseX_elem_idx] | (( 1 << output_bit-1 ) >> baseX_bit_idx);
		}
		
		if	(
				( translate != NULL ) &&
				(
					( baseX_bit_idx == (output_bit - 1) ) ||
					( idx == array_bit_size )
				)
			)
		{
			output_array[baseX_elem_idx] = translate[output_array[baseX_elem_idx]];
		}
	}
	message.logf(LOG_DEBUG,"encode_baseX => %s", output_array );
}

extern void encode_base16( byte *output_array , byte *input_array , size_t array_size )
{
	return(
		encode_baseX( 4 , output_array , 8 , input_array , array_size , Hexadecimal )
	);
}
extern void encode_base32( byte *output_array , byte *input_array , size_t array_size )
{
	return(
		encode_baseX( 5 , output_array , 8 , input_array , array_size , RFC_4648 )
	);
}
extern void encode_base64( byte *output_array , byte *input_array , size_t array_size )
{
	return(
		encode_baseX( 6 , output_array , 8 , input_array , array_size , RFC_2045 )
	);
}

/*	 
extern void _encode_base32( byte *output_array , byte *input_array , size_t array_size )
{
	size_t array_bit_size = array_size * 8;
	for( size_t idx = 0 ; idx < array_bit_size ; idx++ )
	{
		size_t array_byte_idx  = idx / 8;
		size_t byte_bit_idx    = idx % 8;
		size_t base32_elem_idx = idx / 5;
		size_t base32_bit_idx  = idx % 5;
		
		if ( base32_bit_idx == 0 )
		{
			output_array[base32_elem_idx]   = 0;
			output_array[base32_elem_idx+1] = 0;
		}
		
		byte bit_mask = 1 << byte_bit_idx;
		if ( ( input_array[array_byte_idx] & bit_mask ) > 0 )
		{
			output_array[base32_elem_idx] = output_array[base32_elem_idx] | ( 1 << base32_bit_idx );
		}
		if ( base32_bit_idx == (5-1) )
		{
			output_array[base32_elem_idx] = RFC_4648[output_array[base32_elem_idx]];
		}
		Serial.print( byte_bit_idx );
		Serial.print( "\t" );
		Serial.print( bit_mask );
		Serial.print( "\t" );
		if ( ( input_array[array_byte_idx] & bit_mask ) > 0 )
		{
			Serial.print( "1" );
		} else
		{
			Serial.print( "0" );
		}
		Serial.print( "\t" );
		Serial.print( output_elem );
		Serial.print( "\t" );
		Serial.print( RFC_4648[output_elem] );
		Serial.println();

		if ( base32_bit_idx == (5-1) )
		{
			Serial.print( idx );
			Serial.print( "\t" );
			Serial.print( input_array[array_byte_idx] );
			Serial.print( "\t" );
			Serial.println( RFC_4648[output_elem] );
		}
	}
	message.logf(LOG_INFO,"encode_base32 => %s", output_array );
}
*/

#endif //BASE32_H

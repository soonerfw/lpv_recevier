
#include <stdint.h>

//-----------------------32-bit HEX into BCD -----------------------
//     unDec2BCD converts a number from 0 to 99999999 to a BCD string for LCD display
//
// Input: Hex - 32-bit unsigned number, 0 - 2^32-1 (4294967295 - 10 BCD digit)
//        Dec  - 8-bit (char) in ASCII code, for example, if input = 0x0000029C (1234 in decimal) then, Dec = "0000001234"
//        NDigits - number of 8-bit DEC ASCII digits, including NULL end of string, 
//                  for example if the string converted to "12345", nDigits = 5
//                   *Dec should be allocated as size 6, NDigits+1  
// Output None

void unDec2BCD(uint32_t Hex, uint8_t *Dec,uint8_t NDigits)
  {
	uint32_t idx = 0;
  uint32_t index =NDigits-1;
	Dec[NDigits]=0;
	for(idx=0; idx<NDigits; idx++)
		{
			Dec[idx] = ' ';
		}
	while(Hex>=10)
	{
		Dec[index--] = Hex%10 + '0';
		Hex /= 10;
	}	
	Dec[index]=Hex+'0';
}


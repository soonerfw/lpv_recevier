#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "global.h"
// return =FALSE too many input arguments or empty(max 5 input arguments) else = TRUE
// @brief:  This function extracts the argument and number of arguments from an input command line
// @param [in]  *pcCmdLine – a pointer to the command line string
//@ param [out] *pui8Argc – number of command arguments.
//@ param [out] *g_ppcArgc[]  - an array of pointers pointing to each input argument.
//@return       TRUE -  return successfully   FALSE- too many input arguments (max argument is predefined 5) or empty command line

uint8_t CommandLineProcess2(uint8_t *pcCmdLine, uint8_t *pui8Argc, uint8_t *g_ppcArgv[])
{
		uint8_t i;
		uint8_t *tokenPtr;
		i=0;
		tokenPtr = (uint8_t *)strtok((char *)pcCmdLine," ");	
	  while(tokenPtr !=NULL)
		{
			g_ppcArgv[i] = tokenPtr; 
			tokenPtr = (uint8_t *)strtok(NULL," ");	
		  i++;
		}
		if(i>CMDLINE_MAX_ARGS || i==0) return false;
		*pui8Argc=i;
		return true;
}

uint8_t CommandLineProcess(uint8_t *pcCmdLine, uint8_t *pui8Argc, uint8_t *g_ppcArgv[])
{
	 uint8_t ProcessedFinished=false;
	 *pui8Argc = 0; // initialize input argument 0

	// skip multiple spaces in the front of pcCmdline
	while(*pcCmdLine==' ')
	{
		pcCmdLine++;
	}
	
		// check if pcCmdLine is empty
	if(*pcCmdLine=='\0')
	{
		 return false;
	}
	// the first no space command 
	g_ppcArgv[0] = pcCmdLine;
	
  while(!ProcessedFinished)
	{
		  while(*pcCmdLine!= ' ' && *pcCmdLine!='\0')
		   {
				 pcCmdLine++;
			 }
			*pui8Argc +=1;
			if(*pui8Argc >CMDLINE_MAX_ARGS)
				{
					return false;
				}
			// skip multiple spaces 
			while(*pcCmdLine==' ' && *(pcCmdLine+1)==' ')
			{
				pcCmdLine++;
			  if(*pcCmdLine==' ' && *(pcCmdLine+1)=='\0')  // multiple spaces in the end of command line
				{
					pcCmdLine++;
				}
			}
		  if(*pcCmdLine=='\0' )
			{
				ProcessedFinished=true; 
			}
			else
			{
				*pcCmdLine=0; 
				pcCmdLine++;
				g_ppcArgv[*pui8Argc] = pcCmdLine;
			}
 }
		
	return true;	
}


// @brief:  This function removes extra spaces between each argument, only keep one space.
// @param [in]  *string – a pointer to the command line string
// @param [out]  None
// @return None
void RemoveExtraSpace(uint8_t *string)
{
	uint8_t non_space_count=0;
	uint8_t i=0;
	bool inWord = false; // whether the current string[i] in a word
	while(string[i]!='\0')
  {
		if(string[i]!=' ')
		{
			inWord=true;
			string[non_space_count]=string[i];
			non_space_count++;
		}
		else{			
			if(inWord)
			{
				inWord=false;
				string[non_space_count]=string[i];
				non_space_count++;
			 }
		}
	  i++;
	}
	if(string[non_space_count-1] ==' ') // remove space in the end of the string
	{
	   string[non_space_count-1]='\0';		
	}
	else
	{
	string[non_space_count]='\0';	
	}
}


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
	
// 
// @brief This function converts a floating number into a displayable BCD format, xxx.xx with 2 digits in fraction
//
// @param *buff,  - the buffer to save the BCD chars
// @param *size   - size the buffer >=10
// @param num[in]  - floating number to be converted
// @return  None
///                                                                   *sum returned as a NULL string
// @example: number = 123.457,  buff="    123.45"
void float2bcd(uint8_t *buff, uint8_t size, float num)
{
		uint32_t frac,intvalue;
	  uint8_t BCDfrac[2]; 
	  uint8_t BCDint[6]; // integer part 
	  uint8_t i;
	  frac =  ((uint32_t)(num*100))%100; 
	  unDec2BCD(frac,BCDfrac,2);
	  buff[size-2] = BCDfrac[0];
	  buff[size-1] = BCDfrac[1];  
	  buff[size-3] = '.'; 
	  intvalue = ((uint32_t)(num*100))/100;
	  unDec2BCD(intvalue,BCDint,6);
	  for(i=0;i<6;i++)
	    {
				buff[size+i-9] = BCDint[i];				
			}
}


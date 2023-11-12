// ECE 485 Users-Define functions 
//
// April 22, 2023
// by Guoping Wang

#ifndef __FUNCS__H
#define __FUNCS__H
#include <stdint.h>
uint8_t CommandLineProcess(uint8_t *pcCmdLine, uint8_t *pui8Argc, uint8_t *g_ppcArgv[]);
uint8_t CommandLineProcess2(uint8_t *pcCmdLine, uint8_t *pui8Argc, uint8_t *g_ppcArgv[]);
void RemoveExtraSpace(uint8_t *string);
void unDec2BCD(uint32_t Hex, uint8_t *Dec,uint8_t NDigits);
void float2bcd(uint8_t *buff, uint8_t size, float num);
#endif


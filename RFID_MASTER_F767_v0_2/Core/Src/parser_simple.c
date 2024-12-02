/*
 * parser_simple.c
 *
 *  Created on: 12 maj 2021
 *      Author: Mateusz Salamon
 */
#include "main.h"
#include "ring_buffer.h"
#include "parser_simple.h"
#include "string.h"



//
// Get a one complete line from Ring Buffer
//
void Parser_TakeLine(RingBuffer_t *Buf, uint8_t *Destination)
{
	uint8_t Tmp;
	uint8_t i = 0;

	// Loop for every char in Ring Buffer
	do
	{
		// Read a one byte from Ring Buffer
		RB_Read(Buf, &Tmp);

		// Check if we take the endline char
		if(Tmp == ENDLINE)
		{
			// If endline - replace it with c-string end sign - 0x00
			Destination[i] = 0;
		}
		else
		{
			// If not endline - just write to work-buffer
			Destination[i] = Tmp;
		}

		i++; // increment array iterator
	}while(Tmp != ENDLINE); // If we hit the endline char - end the loop
}

// Parsing function
// Commands to detect:
// 	LED_ON
// 	LED_OFF
//
// uint8_t *DataToParse - an array with complete line taken from Ring Buffer earlier
//
void Parser_Parse(uint8_t *DataToParse)
{

	uint8_t command0[] = {"TM_00_P_S_PAL_Linia_X_XX"};
	//command0[3] = slave_addr[0];
	//command0[4] = slave_addr[1];
	RS_Transmit(DataToParse);

	RB_Flush(&rx_data);
	RB_Flush(rx_data);

	// Compare provided array with line to parse with command template
//	if(strcmp(command0, (char*)DataToParse) == 0)
//	{
//		  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//		RS_Transmit("");
//		//RS_Transmit(rec_data);
//
//	}
//	// Next command template
//	else if(strcmp("LED_OFF", (char*)DataToParse) == 0)
//	{
//		 // HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//
//	}








}

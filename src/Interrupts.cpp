/*
 * Interrupts.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: lmark
 */

#include "Interrupts.h"

void printf(char *str);

uint32_t InterruptManager::handleInterruptRequest(
		uint8_t interrupt_id, uint32_t esp)
{
	printf("INTERRUPT\n");
	return esp;
}



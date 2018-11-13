/*
 * Interrupts.h
 *
 *  Created on: Nov 13, 2018
 *      Author: lmark
 */

#ifndef INCLUDE_INTERRUPTS_H_
#define INCLUDE_INTERRUPTS_H_

#include "types.h"
#include "Port.h"

/**
 *	Class used for handling interrupt requests.
 */
class InterruptManager
{
public:

	/**
	 * @interrupt_id - ID of the currently handled interrupt
	 * @esp - Current stack pointer address
	 *
	 * @return - TODO
	 */
	static uint32_t handleInterruptRequest(uint8_t interrupt_id, uint32_t esp);
};



#endif /* INCLUDE_INTERRUPTS_H_ */

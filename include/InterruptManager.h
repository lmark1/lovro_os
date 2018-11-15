/*
 * Interrupts.h
 *
 *  Created on: Nov 13, 2018
 *      Author: lmark
 */

#ifndef INCLUDE_INTERRUPTMANAGER_H_
#define INCLUDE_INTERRUPTMANAGER_H_

#include "types.h"
#include "Port.h"
#include "GlobalDescTable.h"

/**
 *	Class used for handling interrupt requests.
 */
class InterruptManager
{
protected:

	/**
	 * Single entry for the interrupt descriptor table.
	 */
	struct GateDescriptor
	{
		uint16_t 	_handler_base_low;
		uint16_t 	_gdt_code_seg_selector;
		uint8_t 	_reserved;
		uint8_t		_access;
		uint16_t 	_handler_base_high;

	} __attribute__((packed));

	/**
	 * Address of the interrupt descriptor table.
	 */
	struct InterruptDescriptorTablePointer
	{
		uint16_t size;
		uint32_t base;

	} __attribute__((packed));

	/**
	 * Interrupt descriptor table.
	 */
	static GateDescriptor interruptDescriptorTable[256];

	/**
	 * Add a new entry to the interruptDesciptorTable.
	 *
	 * @interruptId
	 * @codeSegmentSelectorOffset
	 * @handler - handler function pointer
	 * @descriptorPrivilegeLevel 				- 0-3 level
	 * @descriptorType
	 */
	static void SetInterruptDescriptorTableEntry(
			uint8_t interruptId,
			uint16_t codeSegmentSelectorOffset,
			void (*handler)(),
			uint8_t decriptorPrivilegeLevel,
			uint8_t descriptorType);

	/**
	 * Open port for Port Interrupt Controller commands and data.
	 */
	Port8BitSlow picMasterCommand;
	Port8BitSlow picMasterData;

	/**
	 * Open slave port for Port Interrupt Controller commands and data.
	 */
	Port8BitSlow picSlaveCommand;
	Port8BitSlow picSlaveData;


public:

	InterruptManager(GlobalDescriptorTable* gdt_p);
	~InterruptManager();

	/**
	 * @interrupt_id - ID of the currently handled interrupt
	 * @esp - Current stack pointer address
	 *
	 * @return - TODO
	 */
	static uint32_t handleInterrupt(uint8_t interrupt_id, uint32_t esp);

	/**
	 * Handle timer interrupt.
	 *
	 * Forward declared function - implementation found in Interrupts.s.
	 */
	static void HandleInterruptRequest0x00();

	/**
	 * Handle keyboard interrupt.
	 *
	 * Forward declared function - implementation found in Interrupts.s
	 */
	static void HandleInterruptRequest0x01();

	/**
	 * Do nothing on interrupt request.
	 */
	static void IgnoreInterruptRequest();

	void Activate();
};



#endif /* INCLUDE_INTERRUPTMANAGER_H_ */

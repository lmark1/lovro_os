/*
 * Interrupts.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: lmark
 */

#include "../include/InterruptManager.h"

void printf(char *str);

uint32_t InterruptManager::handleInterrupt(
		uint8_t interrupt_id, uint32_t esp)
{
	char* foo = "INTERRUPT 0x00\n";
    char* hex = "0123456789ABCDEF";

    foo[12] = hex[(interrupt_id >> 4) & 0xF];
    foo[13] = hex[interrupt_id & 0xF];
    printf(foo);

	return esp;
}

InterruptManager::GateDescriptor
	InterruptManager::interruptDescriptorTable[256];

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt_p)
	: picMasterCommand(0x20),
	  picMasterData(0x21),
	  picSlaveCommand(0xA0),
	  picSlaveData(0xA1)
{
	uint16_t codeSegmentOffset = gdt_p->getCodeSegmentSelectorOff();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;

	for (uint16_t i = 0; i < 256; i++)
	{
		// Initialize the table with empty ignore interrupt requests
		SetInterruptDescriptorTableEntry(
				i,
				codeSegmentOffset,
				&IgnoreInterruptRequest,
				0,
				IDT_INTERRUPT_GATE);
	}

	// Create 2 concrete interrupt entries
	// Timer interrupt entry
	SetInterruptDescriptorTableEntry(
				0x20,
				codeSegmentOffset,
				&HandleInterruptRequest0x00,
				0,
				IDT_INTERRUPT_GATE);
	// Keyboard interrupt entry
	SetInterruptDescriptorTableEntry(
				0x21,
				codeSegmentOffset,
				&HandleInterruptRequest0x01,
				0,
				IDT_INTERRUPT_GATE);

	/**
	 * PIC ICW (Initialization Command Word) - configured in order
	 */
	picMasterCommand.Write(0x11);
	picSlaveCommand.Write(0x11);

	picMasterData.Write(0x20);
	picSlaveData.Write(0x28);

	picMasterData.Write(0x04);
	picSlaveData.Write(0x02);

	picMasterData.Write(0x01);
	picSlaveData.Write(0x01);

	picMasterData.Write(0x00);
	picSlaveData.Write(0x00);

	printf("InterruptManager() - PIC Master / Slave initialized.\n");

	InterruptDescriptorTablePointer idt_p;
	idt_p.size = 256 * sizeof(GateDescriptor) - 1;
	idt_p.base = (uint32_t)interruptDescriptorTable;

	// Load interrupt descriptor table into memory
	__asm__ volatile("lidt %0"
			:
			: "m" (idt_p));

	printf("Load interrupt descriptor table, waiting...\n");
	int i = -10000000;
	while(i < 20000000)
		i++;
}

InterruptManager::~InterruptManager()
{
}

void InterruptManager::SetInterruptDescriptorTableEntry(
			uint8_t interruptId,
			uint16_t codeSegmentSelectorOffset,
			void (*handler)(),
			uint8_t decriptorPrivilegeLevel,
			uint8_t descriptorType)
{
	// Set the bit that the entry is present
	const uint8_t IDT_DESC_PRESENT = 0x80;

	// Set a new entry in the interrupt descriptor table
	interruptDescriptorTable[interruptId]._handler_base_low =
			( (uint32_t)handler ) & 0xFFFF;
	interruptDescriptorTable[interruptId]._handler_base_high =
			(( (uint32_t)handler ) >> 16 ) & 0xFFFF;
	interruptDescriptorTable[interruptId]._gdt_code_seg_selector =
			codeSegmentSelectorOffset;
	interruptDescriptorTable[interruptId]._access =
			IDT_DESC_PRESENT |
			descriptorType |
			((decriptorPrivilegeLevel & 3) << 5);
	interruptDescriptorTable[interruptId]._reserved = 0;
}

void InterruptManager::Activate()
{
	// sti - start interrupts
	__asm__ volatile("sti");
}

/*
 * GlobalDescTable.cpp
 *
 *  Created on: Nov 5, 2018
 *      Author: lmark
 */

#include "GlobalDescTable.h"

#define KB 1024
#define MB 1024 * 1024
#define FLAG_16_BIT 0x40
#define FLAG_32_BIT 0xC0

GlobalDescriptorTable::GlobalDescriptorTable()
	: nullSegmentSelector(0, 0, 0),
	  unusedSegmentSelector(0, 0, 0),
	  codeSegmentSelector(0, 64*MB, 0x9A),		// Present, Ring 0, Executable, 32 bits
	  dataSegmentSelector(0, 64*MB, 0x92)		// Present, Ring 0, Non-Executable, 32 Bit
{
	/*
	 * Tell the processor which gdt table to use.
	 * | LIMIT | ---- BASE ---- |
	 * Limit is lower 16 bits and base is higher 32 - 48 bits total.
	 */
	uint32_t gdt_pointer[2];
	gdt_pointer[1] = (uint32_t)this;
	gdt_pointer[0] = sizeof(GlobalDescriptorTable) << 16;

	// Execute lgdt assembly instruction
	// __volatile__ - must execute where we put it
	__asm__ __volatile__("lgdt (%0)"
			: 											// Output
			: "p" ( ((uint8_t *)gdt_pointer) + 2 )		// Input
			  );
}

GlobalDescriptorTable::~GlobalDescriptorTable()
{
}

uint16_t GlobalDescriptorTable::getCodeSegmentSelectorOff()
{
	return (uint8_t *)&codeSegmentSelector - (uint8_t *)this;
}

uint16_t GlobalDescriptorTable::getDataSegmentSelectorOff()
{
	return (uint8_t *)&dataSegmentSelector - (uint8_t *)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(
		uint32_t base, uint32_t limit, uint8_t access_flags)
{
	/**
	 * If the limit is less than 16 bits (65536) we need to set flags to
	 * 0x40 - no granularity and 16 bit mode.
	 * Else we need to set flags to 0xC0 - Granularity and 32 bit mode.
	 *
	 * Granularity mode:
	 * 		The limit is 5 bytes and shifted to the left by 12 bits.
	 * 		Lower 12 bits are set to 1. And that is the new limit.
	 *
	 * Problem:
	 * 		If lower 12 bits are not 1, we need to decrease the higher bits
	 * 		by 1. Then when the address interpreter sets the lower 12 bits to
	 * 		1 minimal memory is lost.
	 * 		e.g. 0xFFFFFA32 -> convert to legal adress ->
	 * 			 0x000FFFFF -> decrease by one ->
	 * 			 0x000FFFFE -> adress interpreter sees this adress ->
	 * 			 0xFFFFEFFF
	 */
	if (limit <= 65536)
	{
		_limit_high_flags = FLAG_16_BIT;
	}
	else
	{
		_limit_high_flags = FLAG_32_BIT;

		// Check if last bits are 0xFFF...
		if ( (limit & 0xFFF) == 0xFFF )
		{
			// All is well, we get the legal limit by moving it 12 bits right
			limit = limit >> 12;
		}
		else
		{
			// We need to decrease after bitshift to get the legal limit
			limit = (limit >> 12) - 1;
		}

	}

	// Rest of the limit
	_limit_low = limit & 0xFFFF;
	_limit_high_flags |= (limit >> 16) & 0xF;

	// Set base pointer
	_base_low = base & 0xFFFF;
	_base_middle = (base >> 16) & 0xFF;
	_base_high = (base >> 24) & 0xFF;

	// Set access flags
	_access_flags = access_flags;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::getBase()
{
	uint32_t base;
	base = _base_high;
	base = (base << 8) + _base_middle;
	base = (base << 16) + _base_low;
	return base;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::getLimit()
{
	uint32_t limit;
	limit = _limit_high_flags & 0xF;
	limit = (limit << 16) + _limit_low;

	if ( (_limit_high_flags & 0xC0) == 0xC0)
	{
		limit = limit << 12;
		limit |= 0xFFF;
	}
	return limit;
}





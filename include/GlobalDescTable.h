/*
 * GlobalDescTable.h
 *
 * 	Defines global descripter table used for accessing memory segments.
 *	Nested class defines a single segment descriptor entry.
 *
 *  Created on: Nov 5, 2018
 *      Author: lmark
 */

#ifndef INCLUDE_GLOBALDESCTABLE_H_
#define INCLUDE_GLOBALDESCTABLE_H_

#include "types.h"

/**
 * Table consisting of multiple segment descriptors.
 */
class GlobalDescriptorTable
{

public:
	/**
	 * Segment descriptor contains: base address value, limit of the
	 * address segment, along with some flags.
	 */
	class SegmentDescriptor
	{

	private:
		uint16_t limit_low;			// Lower 16 bits of the limit
		uint16_t base_low;			// Lower 16 bits of the base
		uint8_t base_middle;		// Middle 8 bits of the base
		uint8_t access_flags;		// Various access flags
		uint8_t limit_high_flags;	// 4 flags for limit, 4 bits for flags
		uint8_t base_high;			// Final 8 bits of the base

	public:
		SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access_flags);
		uint32_t getBase();
		uint32_t getLimit();

	}__attribute__((packed));
	// Preserve the structure and order of variables
};

#endif /* INCLUDE_GLOBALDESCTABLE_H_ */

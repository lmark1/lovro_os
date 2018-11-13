/*
 * Port.h
 *
 *	Define interface for accessing CPU multiplexor port information.
 *
 *  Created on: Nov 13, 2018
 *      Author: lmark
 */

#ifndef INCLUDE_PORT_H_
#define INCLUDE_PORT_H_

#include "types.h"

/**
 * Default 16 bit port interface.
 */
class Port
{
protected:
	uint16_t port_number;
	Port(uint16_t port_number);
	~Port();
};

/**
 * Concrete class for 8Bit ports.
 */
class Port8Bit : public Port
{
public:
	Port8Bit(uint16_t port_number);
	~Port8Bit();

	virtual void Write(uint8_t data);
	virtual uint8_t Read();
};

/**
 * Concrete class for slower 8Bit ports.
 */
class Port8BitSlow : public Port8Bit
{
public:
	Port8BitSlow(uint16_t port_number);
	~Port8BitSlow();

	virtual void Write(uint8_t data);
};

/**
 * Concrete class for 16Bit ports.
 */
class Port16Bit : public Port
{
public:
	Port16Bit(uint16_t port_number);
	~Port16Bit();

	virtual void Write(uint16_t data);
	virtual uint16_t Read();
};

/**
 * Concrete class for 32Bit ports.
 */
class Port32Bit : public Port
{
public:
	Port32Bit(uint16_t port_number);
	~Port32Bit();

	virtual void Write(uint32_t data);
	virtual uint32_t Read();
};

#endif /* INCLUDE_PORT_H_ */

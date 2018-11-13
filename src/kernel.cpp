#include "types.h"
#include "GlobalDescTable.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Position of the cursor on the screen
static uint8_t x_curs = 0, y_curs = 0;

/**
 * My print function, outputs the chars on the screen.
 * It copies given chars to the screen on the memory location
 * starts outputing things on screen.
 */
void printf(char* str)
{
	unsigned short* VideMemory = (unsigned short*) 0xb8000;

	for(int i = 0; str[i] != '\0'; ++i)
	{
		// Check for newline character
		switch(str[i])
		{
		case '\n':
				y_curs++;
				x_curs = 0;
				break;

		default:
			// Do not overwrite the higher byte - this is where
			// ...text color is defined
			VideMemory[y_curs * SCREEN_WIDTH + x_curs] =
					(VideMemory[y_curs * SCREEN_WIDTH + x_curs] & 0xFF00) | str[i];
			x_curs++;
		}


		// Check if x_curs is out of screen
		if (x_curs >= SCREEN_WIDTH)
		{
			y_curs++;
			x_curs = 0;
		}

		// If y_curs is out of screen delete the whole screen
		if (y_curs >= SCREEN_HEIGHT)
		{
			for (uint8_t _y = 0;  _y < SCREEN_HEIGHT; _y++)
				for (uint8_t _x = 0; _x < SCREEN_WIDTH; _x++)
					VideMemory[_y * SCREEN_WIDTH + _x] =
							(VideMemory[ _y * SCREEN_WIDTH + _x] & 0xFF00) | ' ';
			x_curs = 0;
			y_curs = 0;
		}
	}
}

/**
 * (*constructor)() - function pointer, returns void, no arguments
 */
typedef void (*constructor)();

/**
 * Start of the constructor functions. Address defined in linker.ld.
 */
extern "C" constructor start_ctors;

/**
 * End of the constructor functions. Address defined in linker.ld.
 */
extern "C" constructor end_ctors;

/**
 * Iterates between addresses start_ctors and end_ctors and calls
 * all constructors.
 */
extern "C" void callConstructors()
{
	for(constructor* i = &start_ctors; i != &end_ctors; i++)
	{
		// Dereference the constructor and call it.
		(*i)();
	}
}

/**
 * Extern "C" : Do not change the name of the function when compiling it.
 */
extern "C" void kernelMain(void* multiboot_structure, unsigned int magic_number)
{
	printf("Hello world!\n");
	printf("Hello world - new line");

	// Instantiate gdt
	GlobalDescriptorTable gdt;

	while(1);
}

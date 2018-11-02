
/**
 * My print function, outputs the chars on the screen.
 * It copies given chars to the screen on the memory location
 * starts outputing things on screen.
 */
void printf(char* str)
{
	unsigned short* VideMemory = (unsigned short*) 0xb8000;
	for(int i = 0; str[i] != '\0'; ++i)
		// Do not overwrite the higher byte - this is where
		// ...text color is defined
		VideMemory[i] = (VideMemory[i] & 0xFF00) | str[i];
}

/*
 * Extern "C" : Do not change the name of the function when compiling it.
 */
extern "C" void kernelMain(void* multiboot_structure, unsigned int magic_number)
{
	printf("Hello world!");
	while(1);
}

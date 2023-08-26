#include <ti/screen.h>
#include <ti/getcsc.h>
#include "fixed.h"
#include "color.h"
//#include <stdlib.h>

#define MEM_VRAM 0xD40000
volatile uint16_t* VRAM = (uint16_t*)0xD40000;

/* Main function, called first */
int main(void)
{
	/* Clear the homescreen */
	os_ClrHome();

	/* Print a string */
	os_PutStrFull("Hello world");

	// Draw single red pixel at the top left
	VRAM[0] = RGB15(31, 0, 0);

	/* Waits for a key */
	while (!os_GetCSC())
		;

	/* Return 0 for success */
	return 0;
}
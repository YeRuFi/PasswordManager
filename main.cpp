#include <miosix.h>
#include <stdio.h>
#include <FlashDriver.h>
#include <string>

using namespace miosix;
FlashDriver& flashdriver = FlashDriver::instance();
unsigned int startAddress;

bool writePassword(char c)
{
	//FlashDriver& flashdriver = FlashDriver::instance();
	//startAddress= flashdriver.getSectorAddress(flashdriver.getStartSector());
	//flashdriver.erase(flashdriver.getSectorNumber(0xF8000)); //wrong address format?	
	//flashdriver.erase(); //destroys os?
	flashdriver.erase(flashdriver.getSectorNumber(0x080F8000)); //correct flash address
	return flashdriver.write(0x080F8000,c); //parameters are address to write to and char to write
}

int main()
{
	char test= 'p';	
	printf("Enter character to be written to address 0x080F8000: ");     
	scanf("%c", &test);	     
	bool w = writePassword(test);
	printf("Write result: %i,Character read at address 0x080F8000: ", (int) w);	
	memDump((char *) 0x080F8000, 4);
        printf("\n");
		
}

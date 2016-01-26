#include <miosix.h>
#include <stdio.h>
#include <FlashDriver.h>
#include <string>

using namespace miosix;

int main()
{
    for(;;)
    {
	char test= 'p';
	printf("This character will be written to address 0xFFFDF: %c\n", test);     
	ledOn();      
	FlashDriver& flashdriver = FlashDriver::instance();
	flashdriver.write(0xFFFDF,test); //parameters are address to write to and char to write
	ledOff();
	printf("Character read at address 0xFFFDF: ");	
	memDump((char *) 0xFFFDF, 1);
        printf("\n");
	Thread::sleep(1000);//testing
	
    }
}

#include <miosix.h>
#include <stdio.h>
using namespace miosix;
int main()
{
    for(;;)
    {
        ledOn();
        Thread::sleep(1000);
        ledOff();
        Thread::sleep(1000);
	printf("Hello! Wordl\n");
    }
	//iprintf("Hello! Wordl");
}

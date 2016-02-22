#include <miosix.h>
#include <stdio.h>
#include <SlotPasswordManager.h>

using namespace miosix;

int main()
{
	SlotPasswordManager mupwm;
	mupwm.startUI();
}

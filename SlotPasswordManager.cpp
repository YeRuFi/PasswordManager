#include <stdio.h>
#include <PasswordManager.h>

using namespace miosix;


void SlotPasswordManager::startUI()
{
	char* input = new char[32];
	do
	{
		printf("Welcome to the Password Manager with multiple slots!");
		askinput: 
		printf("Please choose your slot or type help");
		scanf("%s",input);
		if(input[0]=='h')
		{
			printf(HELP);
			goto askinput
		}
		else if(input[0]=='1')
			printf("1");
			//pwm= new PasswordManager(ADDRESS1);
		

		printf("Press enter to shutdown, type restart to login again");		
		scanf("%s",input);
	} while(input!="");
	shutdown();
}

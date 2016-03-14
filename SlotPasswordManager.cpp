#include <stdio.h>
#include <PasswordManager.h>
#include <SlotPasswordManager.h>

void SlotPasswordManager::startUI()
{
	char* input = new char[32];
		printf("\nWelcome to the Password Manager with multiple slots!\n");
		askinput: 
		printf("Please choose your slot or type help: \n");
		scanf("%s",input);
		if(input[0]=='h')
		{
			printf(HELP);
			printf("\n");
			goto askinput;
		}
		else if(input[0]=='1')
			pwm= new PasswordManager(ADDRESS1);
		     else if(input[0]=='2')
				pwm= new PasswordManager(ADDRESS2);
			else if(input[0]=='3')
				pwm= new PasswordManager(ADDRESS3);
			     else if(input[0]=='4')
					pwm= new PasswordManager(ADDRESS4);
				  else {
						printf("Invalid input... \n");
						goto askinput;
				       }
		pwm->startUI();
		
		delete pwm;		
		printf("Press enter to exit, type restart to login again\n");		
		scanf("%s",input);
}

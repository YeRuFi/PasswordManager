#include <miosix.h>
//#include <iostream>
#include <PasswordManager.h>
#include <string>
#include <stdio.h>

using namespace miosix;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!all functions and attributes are public for testing!!!!!!!!!
void writeTest()
{
	printf("WriteTest\n");	
	char p;	
	scanf("%c",&p);	//breakpoint
	PasswordManager *pwm = new PasswordManager();
	pwm->numOfPass=20;
	strcpy(((char*)(pwm->encryptedData))+50, "Hello");
	if(!pwm->storeData())
		printf("Not Succesful\n");
	memDump((char *) 0x080F8000, 9);
	memDump((char *) 0x080F8030, 10);
	memDump((char *) 0x080F8230, 5);
}

int main()
{
	writeTest(); //only for testing	
	printf("Does it work?"); //works with scanf at the end
	char p;	
	scanf("%c",&p);	
	//cout<<"Hello";
	//create PasswordManager on stack	
	//PasswordManager pwm;
	//pwm.startUI(); //start the ui of the Passwordmanager
	
}

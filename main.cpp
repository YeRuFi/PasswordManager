#include <miosix.h>
//#include <iostream>
#include <PasswordManager.h>
#include <string>
#include <stdio.h>

using namespace miosix;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!all functions and attributes are public for testing!!!!!!!!!
void writeTest()
{
	printf("WriteTest");	
	char p;	
	scanf("%c",&p);	//breakpoint
	//printf("WriteTestprintf");
	PasswordManager *pwm = new PasswordManager();
	/*string web, pas;
	cout<<"Website: \n";
	cin>>web;
	cout<<"Password: \n";	
	cin>>pas;	
	//pwm.addPassword(web,pas);*/
	
	strcpy(pwm->passwords[0].website, "Hello");
	if(pwm->storeData())
		printf("Succesful");
	memDump((char *) 0x080F8000, 9);
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

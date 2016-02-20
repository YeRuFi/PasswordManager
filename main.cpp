#include <miosix.h>
#include <iostream>
#include <PasswordManager.h>
#include <string>

using namespace miosix;

void writeTest()
{
	PasswordManager pwm;
	/*string web, pas;
	cout<<"Website: \n";
	cin>>web;
	cout<<"Password: \n";	
	cin>>pas;	
	//pwm.addPassword(web,pas);*/
	strcpy(pwm.passwords[0].website, "Hello");
	if(pwm.storeData())
		cout<<"Succesful";
	memDump((char *) 0x080F8000, 9);
}

int main()
{
	writeTest(); //only for testing	
	
	//create PasswordManager on stack	
	PasswordManager pwm;
	pwm.startUI(); //start the ui of the Passwordmanager
	
}

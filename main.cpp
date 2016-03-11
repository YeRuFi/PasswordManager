#include <miosix.h>
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
	memDump((char *) 0x080F8038, 10);
}

void readTest()
{
	printf("ReadTest\n");	
	char p;	
	scanf("%c",&p);	//breakpoint
	char* data= new char[60];
	strcpy(data, "PWM");
	strcpy(data+4, "!");
	//strcpy(data+5, ""); //0 as string terminator 
	strcpy(data+6, "Google");
	strcpy(data+38, "password123");
	
	FlashDriver& flashdriver = FlashDriver::instance();
	flashdriver.erase(11);
	flashdriver.write(0x080F8000,data,60);

	PasswordManager *pwm = new PasswordManager();
	pwm->loadData();
	printf("Number of stored Passwords: %i \n", pwm->numOfPass);
	printf("First website: %s \n", pwm-> encryptedData);
	printf("First password: %s \n", (pwm-> encryptedData)+ 32);
}

void writeReadTest()
{
	printf("Read and write Test\n");	
	char p;	
	scanf("%c",&p);	//breakpoint
	char* data= new char[60];
	strcpy(data, "PWM");
	strcpy(data+4, "!");
	//strcpy(data+5, ""); //0 as string terminator 
	strcpy(data+6, "Google");
	strcpy(data+38, "password123");
	
	FlashDriver& flashdriver = FlashDriver::instance();
	flashdriver.erase(11);
	flashdriver.write(0x080F8000,data,60);

	PasswordManager *pwm = new PasswordManager();
	pwm->loadData();
	printf("After First Load:\n");	
	printf("Number of stored Passwords: %i \n", pwm->numOfPass);
	printf("First website: %s \n", pwm-> encryptedData);
	printf("First password: %s \n", (pwm-> encryptedData)+ 32);
	
	pwm->numOfPass=2;
	strcpy(((char*)(pwm->encryptedData))+64, "WebMail");
	strcpy(((char*)(pwm->encryptedData))+96, "longsafeveryandwordforPassthis");
	if(!pwm->storeData())
		printf("1st store not Succesful\n");
	printf("After First Store:\n");	
	memDump((char *) 0x080F8000, 140);
	
	pwm->loadData();
	printf("After 2nd Load:\n");	
	printf("Number of stored Passwords: %i \n", pwm->numOfPass);
	printf("First website: %s \n", pwm-> encryptedData);
	printf("First password: %s \n", (pwm-> encryptedData)+ 32);
	printf("Second website: %s \n", pwm-> encryptedData+64);
	printf("Second password: %s \n", (pwm-> encryptedData)+ 96);
	
	if(!pwm->storeData())
		printf("2nd store not Succesful\n");
	printf("After Second Store:\n");	
	memDump((char *) 0x080F8000, 140);
	
}
//test after merging encryption with flash
void test(){
PasswordManager *pwm = new PasswordManager();
pwm->addPassword("google","123");
pwm->addPassword("amazon","secondPass");
char password[32];
pwm->printAll();
printf("Put the password:\n");
scanf("%s",password);
pwm->createKey(password);
pwm->encrypt();
pwm->storeData();

if(!pwm->storeData())
		printf("Error while storing\n");
	printf("Flash content:\n");	
	memDump((char *) 0x080F8000, 140);
pwm->loadData();
pwm->decrypt(pwm->encryptedData);
pwm->printAll();
}


int main()
{
	//writeReadTest(); //call test function
	//printf("Does it work?"); //works with scanf at the end
	//char p;	
	//scanf("%c",&p);
        test();			
}

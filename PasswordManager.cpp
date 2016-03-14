#include <miosix.h>
#include <string.h>
#include <FlashDriver.h>
#include "aes.h"
#include "md5.h"
#include "PasswordManager.h"
#include <stdlib.h>
#include <stdio.h>
using namespace miosix;
using namespace std;

PasswordManager::PasswordManager():
firstUse(false),
changed(false),
numOfPass(0),
address(STANDARD_ADDRESS)
{
    encryptedData=new unsigned char[MAXSTORED*2*PASSWORDLENGTH+16];//website length is the same as PASSWORDLENGTH ,16 for checksum
    passwords=new WPTuple[MAXSTORED];
    
}

PasswordManager::PasswordManager(unsigned int addr):
firstUse(false),
changed(false),
numOfPass(0),
address(addr)
{
        encryptedData=new unsigned char[MAXSTORED*2*PASSWORDLENGTH+16];//website length is the same as PASSWORDLENGTH ,16 for checksum
        passwords=new WPTuple[MAXSTORED];

}

PasswordManager::~PasswordManager()
{
	delete[] encryptedData;
        delete[] passwords;
}

void PasswordManager::startUI()
{
    //initialization of password manager
    
    printf("\nWelcome to the Password Manager!\n");
    
    loadData();
    
    char* input = new char[PASSWORDLENGTH];
    if(firstUse==false)
    {
    checkpass:
        printf("Please enter the master password: \n");
	scanPassword(input); // let the user input the master password
        createKey(input);
        if(!decrypt(encryptedData))
        {
            printf("Wrong Password! \n");
            goto checkpass;
        }
        strcpy(input, "..... ..... ..... ..... ..... ."); //erase masterpassword
    }
    do
    {
        printf("Please enter a command or type help: \n");
        scanf("%s",input);
        if(input[0]=='h') //help
        {
            	printf("\nThe following commands are available: \nNote: It is enough to write the first letter of a command.\n\n");
            	printf("add: 			add a new website and password\n");
		printf("remove: 		remove an existing website and its password\n");
		printf("search: 		search for a website to see its password\n");
		printf("printAll: 		print all websites and their passwords\n");
		printf("masterPassword: 	change the master password to log in\n");
		printf("websitePassword: 	change the password of an existing website\n");
		printf("commit: 		store all data on the flash (preferably at the end)\n");
		printf("quit: 			quits the program\n");
		printf("exit: 			same as quit\n");
		printf("delete: 		deletes all data and exits; frees slot on the flash\n\n");
        }
        else if(input[0]=='a') //add
        {
            char * website = new char [PASSWORDLENGTH];
            printf("Adding new entry:\n");
            printf("website name: \n");
            scanf("%s",website);
            printf("password: \n");
            scanf("%s",input);
            if(!addPassword(website, input))
		printf("No space available anymore, remove another password.\n");
            delete[] website;
        }
        else if(input[0]=='s') //search
        {
            printf("Please enter the website name for which you want to know the password: \n");
	    scanf("%s",input);
            if(!searchPassword(input))
                printf("Not found, try again or printAll \n");
        }
        else if(input[0]=='p') //printAll
        {
            printAll();
        }
        else if(input[0]=='m') //change master password
        {
            changed=true;
            //check old master password? -> implemented in function
            changeMasterPassword();
        }
        else if(input[0]=='w') //change website password
        {
            printf("For which website should the password be changed? \n");
            scanf("%s",input);
            changePassword(input);
        }
        else if(input[0]=='r') //remove
        {
            printf("Which website should be removed? \n");
            scanf("%s",input);
            if(!remove(input))
            {
                printf("Could not remove. Please check website name.\n");
            }
        }
	else if(input[0]=='d') //delete
        {
       		printf("Be careful: All your data will be erased. This can not be undone. Type yes to continue:\n");
            	scanf("%s",input);
		if(input[0]=='y'){
            		if(checkMasterPassword()){
          			if(deleteData())
				{
					printf("Successfully erased\n");
					break;
				}
				else
					printf("Error while accessing flash\n");
          		}
		}
        }
        else if(input[0]=='c') //commit
        {
            if(changed)
            {
                if(firstUse)
                {
                    printf("Write your master password with which you want to encrypt the data:\n");
                    scanPassword(input);
                    createKey(input);
                }
                encrypt();
                if(storeData())
                    changed=false;
                else
                    printf("Error while storing the data on the flash\n");
            }
        }
        else if(input[0]=='e'||input[0]=='q') //exit or quit
        {
            if (changed)
            {
                printf("There is data that has not been saved. Do you really want to quit? Type yes (to discard all changes) or no. \n");
                scanf("%s",input);
                if(input[0]=='y')
                    break;
            }
            else
            {
                break;
            }
        }
        else
            printf("Invalid input... \n");
    } while(true);
    delete[] input;
}

char * PasswordManager::structToArray(WPTuple input[],int numOfPasswords,int lengthOfWebsite){
    int i,j,z;
    char *output;
    output=(char *)malloc(sizeof(char)*numOfPasswords*2*lengthOfWebsite);
    z=0;
    for(i=0;i<numOfPasswords;i++){
        for(j=0;j<lengthOfWebsite;j++)
        {
            output[z]=input[i].website[j];
            z++;
        }
        for(j=0;j<lengthOfWebsite;j++)
        {
            output[z]=input[i].password[j];
            z++;
        }
    }
    return output;
}

WPTuple* PasswordManager::arrayToStruct(char * input,int numOfPasswords,int lengthOfWebsite){
    int i,j,z;
    WPTuple *output;
    output=(WPTuple *)malloc(sizeof(char)*numOfPasswords*2*lengthOfWebsite);
    z=0;
    for(i=0;i<numOfPasswords;i++){
        for(j=0;j<lengthOfWebsite;j++)
        {
            output[i].website[j]=input[z];
            z++;
        }
        for(j=0;j<lengthOfWebsite;j++)
        {
            output[i].password[j]=input[z];
            z++;
        }
    }
    return output;
    
}

void     PasswordManager::encrypt(){
         //transform the structers to be encrypted in an array
         char *inputEn=structToArray(passwords,numOfPass,PASSWORDLENGTH);
         int sizeOfInput=numOfPass*2*PASSWORDLENGTH;
         //calculate the hash of the input Array and put it in the checksum
         mbedtls_md5((const unsigned char *)inputEn,sizeOfInput, checksum );
         //add the checksum in the array to be encrypted
         unsigned char * toEncrypt=(unsigned char *)malloc(sizeOfInput+16);
         int i;
         for(i=0;i<sizeOfInput;i++){
         toEncrypt[i]=inputEn[i];
         }
         for(i=sizeOfInput;i<sizeOfInput+16;i++){
         toEncrypt[i]=checksum[i-sizeOfInput];
         }
         unsigned char * iv=(unsigned char *)malloc(16);
         mbedtls_md5((const unsigned char *)key,32, iv );
         //encrypt the given array and put it in the array to be returned
         AES128_CBC_encrypt_buffer(encryptedData,toEncrypt,numOfPass*2*PASSWORDLENGTH+16, (const unsigned char*)key,(const unsigned char *)iv);
         //not sure ? free the memory allocated to the arrays
         free(toEncrypt);
         free(iv);
         free(inputEn);        
}
bool PasswordManager::decrypt(unsigned char * input){
         int i;
         int sizeOfInput=numOfPass*2*PASSWORDLENGTH+16;
         unsigned char * outputEn=(unsigned char *)malloc(sizeOfInput);
         unsigned char * toTransform=(unsigned char *)malloc(sizeOfInput-16);
         unsigned char toCheck[16];
         unsigned char * iv=(unsigned char *)malloc(16);
         mbedtls_md5((const unsigned char *)key,32, iv );
         //decrypt the givin array after the load from the memory
         AES128_CBC_decrypt_buffer(outputEn,input, numOfPass*2*PASSWORDLENGTH+16, (const unsigned char*)key,(const unsigned char *)iv);
         //divide the buffer after decrypt into checksum and array to be turned in the array of WPTuples if decryption correct
         for(i=0;i<sizeOfInput-16;i++){
         toTransform[i]=outputEn[i];
         }
         for(i=sizeOfInput-16;i<sizeOfInput;i++){
         checksum[i-sizeOfInput+16]=outputEn[i];
         }
         //frees memory ocupied by array 
         free(outputEn);
         //calculate the hash of the array to transfom to compare it to the checksum
         mbedtls_md5((const unsigned char *)toTransform,sizeOfInput-16, toCheck );
         //check if the checksums are the same so if the password is correct
         free(iv);
         for(i=0;i<16;i++){
         if(toCheck[i]!=checksum[i]){
          
          free(toTransform);
          return false; 
          }
         }
         //password is correct so initialize the structure
         passwords=arrayToStruct((char *)toTransform,numOfPass,PASSWORDLENGTH);
         free(toTransform);
         return true;         
}
void PasswordManager::createKey(char * password){
         unsigned char hashedToTransform[16];
         mbedtls_md5((const unsigned char *)addCharacters(password),32, hashedToTransform );
         int i;
         //create key
         for(i=0;i<16;i++){
            key[i]=hashedToTransform[i];
         }
         for(i=0;i<16;i++){
            key[i+16]=hashedToTransform[i];
         }      

}
bool PasswordManager::cmpChar(char one[32],char two[32]){
    int i;
    for(i=0;i<31;i++){
        if(one[i]!=two[i]){
            return false;
        }
    }
    return true;
}

//the return value must be freed every time 
char * PasswordManager::addCharacters(char * input){
         char * output=(char *) malloc(32*sizeof(char));
         int i;
         int size=(int)strlen(input);
         for(i=0;i<size+1;i++){
         output[i]=input[i];
         }
         for(i=size+1;i<32;i++){
         output[i]=(char)255;
         }  
         return output;
}

bool PasswordManager::addPassword(char * website,char * password){
         if(numOfPass == MAXSTORED)
		return false;
	 else
	{
		 char * web=addCharacters(website);
	         char * pass=addCharacters(password);
	         strcpy(passwords[numOfPass].website,web);
	         strcpy(passwords[numOfPass].password,pass);
	         free(web);
	         free(pass);
	         numOfPass++;
	         changed=true;
		 return true;
	}
}

//the return must be freed 
char * PasswordManager::transformArray(char input[32]){
         int size,i;
         size=0;
         for(i=0;i<32;i++){
            if(input[i]==(char)255){
               break;
            }
         size++;
         }
         char * output=(char *)malloc(size*sizeof(char));
         for(i=0;i<size;i++){
         output[i]=input[i];   
         }
         return output;
}

void PasswordManager::printAll(){
         int i;
         for(i=0;i<numOfPass;i++){
         char* website=transformArray(passwords[i].website);
         printf("Website: %s ", website);
         free(website);
         char* pass=transformArray(passwords[i].password);
         printf("Password: %s \n",pass);
         free(pass);
         }

}

bool PasswordManager::searchPassword(char * website){
         int i=getPosition(website);
         if(i!=-1){
            printf("Website: %s \n",transformArray(passwords[i].website));
            printf("Password: %s \n",transformArray(passwords[i].password));
            return true;
         }
         return false;
}

bool PasswordManager::remove(char * website){
         int i;
         int toRemove=getPosition(website);
         if(toRemove==-1){
         	return false;
         }
	 else{
         	if(numOfPass==1)
		{
         		numOfPass--;
         		changed=true;
         		return true;
        	}
         	memcpy((void*) passwords[toRemove].website, (void*) passwords[numOfPass-1].website, PASSWORDLENGTH);
         	memcpy((void*) passwords[toRemove].password, (void*) passwords[numOfPass-1].password, PASSWORDLENGTH);
         	numOfPass--;
         	changed=true;
         	return true;
         }
}

int PasswordManager::getPosition(char * input){
    char * toSearch=addCharacters(input);
    int i;
    int position=-1;
    for(i=0;i<numOfPass;i++){
        if(strcmp(passwords[i].website,toSearch)==0){ //cmpChar gives error
            position=i;
        }
    }
    free(toSearch);
    return position;
}

void PasswordManager::changePassword(char * website){
     int i=getPosition(website);
     if(i==-1){
     printf("The website that you requested to change password does not exist\n");
     }else{
     char newPass[32];
     printf("Please insert the new password for %s: \n",website);
     scanf("%s",newPass);
     char* pass=addCharacters(newPass);
     strcpy(passwords[i].password,pass);
     free(pass);
     changed=true;
     printf("Password changed successfully! \n");
     }
}

bool PasswordManager::changeMasterPassword(){
	if(checkMasterPassword())
	{
     		char * givenPassword=(char *)malloc(32);;//variable to save the password given
		printf("Insert new Password:\n");
     		scanPassword(givenPassword);
     		createKey(givenPassword);
     		printf("Password changed successfully\n");
     		free(givenPassword);
     		return true;
	}
	else
		return false;
     
}

bool PasswordManager::checkMasterPassword()
{
	char * givenPassword=(char *)malloc(32);;//variable to save the passwords given by  
     	unsigned char hashedGiven[16];//variable to put the hash of the given password
     	int i;
     	printf("Insert the master password:\n");
     	scanPassword(givenPassword);
     	char * pass=addCharacters(givenPassword);
     	mbedtls_md5((const unsigned char *)pass,32, hashedGiven);
     	for(i=0;i<16;i++){
       		 if(hashedGiven[i]!=key[i]){
           		free(pass);
	  		free(givenPassword);
          		return false;
          	}
     	}
	free(pass);
	free(givenPassword);
	return true;
}

void PasswordManager::scanPassword(char* memory)
{
        //system("stty -echo"); //supported in miosix??
        scanf("%s",memory); //should not print password on screen!, dummy
        //system("stty echo");
}

/**
 * Flash Layout for store and load Data functions
 * standard start address is used as example:
 *
 * Address 	Content 	Comment						\n
 * 0x080F8000	'P'		Identification					\n
 * 0x080F8001	'W'		"						\n
 * 0x080F8002	'M'		"						\n
 * 0x080F8003	0x00		"						\n
 * 0x080F8004	numOfPass	short						\n
 * 0x080F8006	passwords 	encryptedData					\n
 * 0x080F8007	"		"						\n
 * ...		...		...						\n
 * 0x080F8006+numOfPass*64 +16 //64 for every password, 16 for the checksum	\n
 * Afterwards	0xFF		"						\n
 * ...		...		... //empty area reserved for new passwords	\n
 * 0x080FFFFF	0xFF		"
 *
 */
bool PasswordManager::storeData()
{
	bool success = false; //return value, true when erase and all writes have been successful	
	// identifier to identify first use and to make sure invalid data is never read coincidentally:
	char ident[4] = "PWM"; 
	//get flash driver instance (singleton):
	FlashDriver& flashdriver = FlashDriver::instance();
	//erase the sector to store password data:
	success = flashdriver.erase(flashdriver.getSectorNumber(address)); //necessary for flash (all is put to 0xFF)
	//write identifier to the flash:
	success &= flashdriver.write(address, ident, 4);
	char* ramAddress = (char*) &numOfPass; //get address of numOfPass
	//write numOfPass of length 2 (short) to the flash:	
	success &= flashdriver.write(address+4, ramAddress, 2);
	//write encryptedData (char array) of length sizeof(WPTuple)*numOfPass+sizeof(checksum) to flash
	ramAddress = (char*) encryptedData; //convert unsigned char* to char*	
	success &= flashdriver.write(address+6, ramAddress, PASSWORDLENGTH*2*numOfPass+16);
	return success;
}
void PasswordManager::loadData()
{
	//check for identifier at the flash address:	
	char ident[4];
	void* ramAddress= (void*) ident; //get ident address	
	memcpy(ramAddress,(void*) address,4); //copy char[4]
	if(strcmp(ident,"PWM")==0)
	{
		firstUse=false;	//valid data available	
		//load numOfPass from flash:
		ramAddress= (void*) &numOfPass; // get attribute address		
		memcpy(ramAddress,(void*) (address+4),2); //copy short
//add integrity check for numOfPass?	
		//load passwords from flash:
		ramAddress= (void*) encryptedData; // get attribute address		
		memcpy(ramAddress,(void*) (address+6),PASSWORDLENGTH*2*numOfPass+16); //copy all WPTuples and checksum
	}
	else
	{
		//invalid data in flash
		firstUse=true;
		numOfPass=0;
	}
}

bool PasswordManager::deleteData()
{
	//get flash driver instance (singleton):
	FlashDriver& flashdriver = FlashDriver::instance();
	//erase the sector with the password data:
	return flashdriver.erase(flashdriver.getSectorNumber(address)); //necessary for flash (all is put to 0xFF)
}




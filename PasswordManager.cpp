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
         for(i=0;i<16;i++){
         if(toCheck[i]!=checksum[i]){
          
          free(toTransform);
          return false; 
          }
         }
         //password is correct so initialize the structure
         passwords=arrayToStruct((char *)toTransform,numOfPass,PASSWORDLENGTH);
         
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
        for(i=0;i<32;i++){
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
void PasswordManager::addPassword(char * website,char * password){
         char * web=addCharacters(website);
         char * pass=addCharacters(password);
         strcpy(passwords[numOfPass].website,web);
         strcpy(passwords[numOfPass].password,pass);
         free(web);
         free(pass);
         numOfPass++;
         changed=true;
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
         printf("Website: %s \n", website);
         free(website);
         char* pass=transformArray(passwords[i].password);
         printf("Password: %s \n",pass);
         free(pass);
         }

}
bool PasswordManager::searchPassword(char * website){
         char * toSearch=addCharacters(website);
         int i=getPosition(website);
         if(i!=-1){
            printf("Website: %s \n",website);
            printf("Password:%s \n",transformArray(passwords[i].password));
            free(toSearch);
            return true;
            }
         free(toSearch);
         return false;
}
bool PasswordManager::remove(char * website){
         char * toSearch=addCharacters(website);
         int i;
         int toRemove=getPosition(website);
         free(toSearch);
         if(toRemove==-1){
         return false;
         }else{
         if(numOfPass==1){
         numOfPass--;
         changed=true;
         return true;
         }
         strcpy(passwords[toRemove].website,passwords[numOfPass-1].website);
         strcpy(passwords[toRemove].password,passwords[numOfPass-1].password);
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
            if(cmpChar(passwords[i].website,toSearch)){
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
     printf("Please instert the new password for + %s \n",website);
     scanf("%s",newPass);
     char* pass=addCharacters(newPass);
     strcpy(passwords[i].password,pass);
     free(pass);
     changed=true;
     printf("Password changed\n");
     }

}

bool PasswordManager::changeMasterPassword(){
     char * givenPassword=(char *)malloc(32);;//variable to save the passwords given by  
     unsigned char hashedGiven[16];//variable to put the hash of the given password
     int i;
     printf("Insert your old password:\n");
     scanf("%s",givenPassword);
     char * pass=addCharacters(givenPassword);
     mbedtls_md5((const unsigned char *)pass,32, hashedGiven);
     for(i=0;i<16;i++){
        if(hashedGiven[i]!=key[i]){
           printf("Wrong Password! \n");
           free(pass);
           return false;
          }
     }
     printf("Insert new Password:\n");
     scanf("%s",givenPassword);
     createKey(givenPassword);
     printf("Password changed successfully\n");
     free(pass);
     return true;
     
}

/**
 * Flash Layout for store and load Data functions
 * standard start address is used as example:
 *
 * Address 	Content 	Comment
 * 0x080F8000	'P'		Identification
 * 0x080F8001	'W'		"
 * 0x080F8002	'M'		"
 * 0x080F8003	0x00		"
 * 0x080F8004	numOfPass	short
 * 0x080F8006	passwords 	encryptedData
 * 0x080F8007	"		"
 * ...		...		...
 * 0x080F8006+numOfPass*64 +16 //64 for every password, 16 for the checksum
 * Afterwards	0xFF		"
 * ...		...		... //empty area reserved for new passwords
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




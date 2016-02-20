//#include <miosix.h>
#include <string.h>
#include <iostream>
//#include <FlashDriver.h>
#include "aes.h"
#include "md5.h"
#include "PasswordManager.h"
#include <stdlib.h>
#include <stdio.h>
//using namespace miosix;

//PasswordManager::PasswordManager();
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
unsigned char * PasswordManager::encrypt(){
         //transform the structers to be encrypted in an array
         char *inputEn=structToArray(passwords,numOfPass,PASSWORDLENGTH);
         int sizeOfInput=sizeof(inputEn);
         //create and allocate memory for an array to put the encryption
         unsigned char *outputEn=(unsigned char *)malloc(sizeOfInput+16);
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
         unsigned char * iv;
         mbedtls_md5((const unsigned char *)key,32, iv );
         //encrypt the given array and put it in the array to be returned
         AES128_CBC_encrypt_buffer(outputEn,toEncrypt,numOfPass*2*PASSWORDLENGTH+16, (const unsigned char*)key,(const unsigned char *)iv);
         //not sure ? free the memory allocated to the arrays
         free(toEncrypt);
         free(inputEn);
         return outputEn;         
}
bool PasswordManager::decrypt(unsigned char * input){
         int i;
         int sizeOfInput=sizeof(input);
         unsigned char * outputEn=(unsigned char *)malloc(sizeOfInput);
         unsigned char * toTransform=(unsigned char *)malloc(sizeOfInput-16);
         unsigned char toCheck[16];
         unsigned char * iv;
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
         passwords=(WPTuple *)malloc(sizeof(WPTuple)*numOfPass);
         //password is correct so initialize the structure
         passwords=arrayToStruct((char *)toTransform,numOfPass,PASSWORDLENGTH);
         
         return true;         
}
void PasswordManager::createKey(char * password){
         unsigned char * hashedToTransform;
         mbedtls_md5((const unsigned char *)password,16, hashedToTransform );
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



char * PasswordManager::addCharacters(char * input){
         char * output=(char *) malloc(32*sizeof(char));
         int i;
         int size=sizeof(input);
         for(i=0;i<size;i++){
         output[i]=input[i];
         }
         for(i=size;i<32;i++){
         output[i]='.';
         }   
}
void PasswordManager::addPassword(char * website,char * password){
         char * web=addCharacters(website);
         char * pass=addCharacters(password);
         strcpy(passwords[numOfPass].website,web);
         strcpy(passwords[numOfPass].password,pass);
         numOfPass++;
}

char * PasswordManager::transformArray(char input[32]){
         int size,i;
         size=0;
         for(i=0;i<31;i++){
            if(input[i]=='.'&&input[i+1]=='.'){
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
         printf("Website: %s \n",transformArray(passwords[i].website));
         printf("Password: %s \n",transformArray(passwords[i].password));
         }
}
bool PasswordManager::searchPassword(char * website){
         char * toSearch=addCharacters(website);
         int i;
         for(i=0;i<numOfPass;i++){
            if(cmpChar(passwords[i].website,toSearch)){
            printf("Website: %s \n",website);
            printf("Password:%s \n",transformArray(passwords[i].password));
            return true;
            }
         }
         return false;
}
bool PasswordManager::remove(char * website){
         char * toSearch=addCharacters(website);
         int i;
         int toRemove=-1;
         for(i=0;i<numOfPass;i++){
            if(cmpChar(passwords[i].website,toSearch)){
            toRemove=i;
            }
         }
         if(toRemove==-1){
         return false;
         }else{
         if(numOfPass==1){
         numOfPass--;
         return true;
         }
         strcpy(passwords[toRemove].website,passwords[numOfPass-1].website);
         strcpy(passwords[toRemove].password,passwords[numOfPass-1].password);
         numOfPass--;
         return true;
         }

}





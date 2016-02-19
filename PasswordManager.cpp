//#include <miosix.h>
#include <string>
#include <iostream>
//#include <FlashDriver.h>
#include "aes.h"
#include "md5.h"
#include "PasswordManager.h"
#include <stdlib.h>
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
         const unsigned char * iv;
         iv=checksum;
         //encrypt the given array and put it in the array to be returned
         AES128_CBC_encrypt_buffer(outputEn,toEncrypt,numOfPass*2*PASSWORDLENGTH+16, key,iv);
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
         const unsigned char * iv;
         iv=checksum;
         //decrypt the givin array after the load from the memory
         AES128_CBC_decrypt_buffer(outputEn,input, numOfPass*2*PASSWORDLENGTH+16, key,iv);
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





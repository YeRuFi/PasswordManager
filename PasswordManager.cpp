#include <miosix.h>
#include <string.h>
#include <FlashDriver.h>
#include "aes.h"
#include "md5.h"
#include "PasswordManager.h"
#include <stdlib.h>
#include <stdio.h>
using namespace miosix;

PasswordManager::PasswordManager():
firstUse(false),
changed(false),
numOfPass(0),
address(STANDARD_ADDRESS)
{
    encryptedData=new unsigned char[MAXSTORED*2*PASSWORDLENGTH+16];//website length is the same as PASSWORDLENGTH ,16 for checksum
    passwords=new WPTuple[MAXSTORED];
    
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
        system("stty -echo"); //supported in miosix??
        scanf("%s",input); //should not print password on screen!
        system("stty echo");
        createKey(input);
        if(!decrypt(encryptedData))
        {
            printf("Wrong Password!");
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
            //printf(HELP);
            printf("\n");
        }
        else if(input[0]=='a') //add
        {
            char * website = new char [PASSWORDLENGTH];
            printf("Adding new entry:\n");
            printf("website name: \n");
            scanf("%s",website);
            printf("password: \n");
            scanf("%s",input);
            addPassword(website, input);
            delete[] website;
        }
        else if(input[0]=='s') //search
        {
            scanf("%s",input);
            if(!searchPassword(input))
                printf("Not found, try again or printAll");
        }
        else if(input[0]=='p') //printAll
        {
            printAll();
        }
        else if(input[0]=='m') //change master password
        {
            changed=true;
            //firstUse=true;
            //check old master password? -> implemented in function
            changeMasterPassword()
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
        else if(input[0]=='c') //commit
        {
            if(changed)
            {
                if(firstUse)
                {
                    //set master password
                }
                encrypt();
                if(storeData())
                    changed=false;
                else
                    printf("Error while storing the data on the flash");
            }
        }
        else if(input[0]!='e'||input[0]!='q') //exit or quit
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
    changed=true;
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
    int i=getPosition(website);
    if(i!=-1){
        printf("Website: %s \n",website);
        printf("Password:%s \n",transformArray(passwords[i].password));
        return true;
    }
    return false;
}
bool PasswordManager::remove(char * website){
    char * toSearch=addCharacters(website);
    int i;
    int toRemove=getPosition(website);
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
    return position;
    
}

void PasswordManager::changePassword(char * website){
    int i=getPosition(website);
    if(i==-1){
        printf("The website that you requested to change password does not exist\n");
    }else{
        char newPass[32];
        printf("Please insert the new password for + %s \n",website);
        scanf("%32s",newPass);
        strcpy(passwords[i].password,addCharacters(newPass));
        changed=true;
        printf("Password changed\n");
    }
    
}






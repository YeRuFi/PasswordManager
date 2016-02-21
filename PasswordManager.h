/***************************************************************************

 *   Copyright (C) 2016 by                                                 * 
 *          Rose Vinay Kumar Yelluri, Jan Fischer, Arlind Rufi		   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/ 

#ifndef PASSWORDMANAGER_H
#define	PASSWORDMANAGER_H

#define ADDRESS 0x080F7FFC9 //0x080F8000- 6
#define PASSWORDLENGTH 32 //the maximum number of bytes for each password also website

//#include <FlashDriver.h>
#include <string>
#include <iostream>

using namespace std;

struct WPTuple {
	char website[32];
	char password[32];
};


class PasswordManager
{

public:

   	 /**
   	  * 
   	  * Starts the UserInterface of the PasswordManager
   	  * 
   	  */
   	 void startUI();
    
	/**
   	  * Constructor
   	  * 
   	  */
   	//PasswordManager();
 
private:
    

	/**
	  * Attributes
  	  */
    	WPTuple *passwords; //pointer?
	unsigned char checksum[16]; // hash of passwords array
	short numOfPass; // number of currently stored passwords
	bool changed; // true when commit is necessary (flash write)
	unsigned char key[32];// a variable to save the key to encrypt the data
        unsigned char * encryptedData; //to put the encrypted data after load from flash
        //const unsigned int address; //address of passwords in Flash (constant 0xF8000); better with DEFINE?

	
	/**
   	  * Initializes passwords[], checksum, numOfPass by loading them from the Flash (load previous stored passwords)
   	  */	
	void loadData();

	/**
   	  * Writes data to the flash (Commit), only necessary when changed true and user wants to commit
	  * \return true no write errors
   	  */	
	bool storeData();
	
	/**
          * searches for a given website if it is in the array of WTPuples prints it if exists
	  * \param website to be searched for
	  * \return true if the website is in the array of data structures 
   	  */	
	bool searchPassword(char * website);

	/**
	  * prints all website password tuples on the console
   	  */	
	void printAll();
	
	/**
	  * \param website to add 
          * \param password to add
   	  */	
	void addPassword(char * website, char * password);

	/**
	  * \param website to be removed (including password)
	  * \return true: succesful removed
   	  */	
	bool remove(char * website);

	/**
	  * Encrypts the attributes (data), will be saved in the attributes
          * \return An encryted array of the data that the structure has
   	  */	
	unsigned char* encrypt();

	/**
	  * Decrypts the attributes (data)
   	  * \return true: password correct
   	  */		
	bool decrypt(unsigned char * input);
        /**
          *Transforms the array of WPTuples into a single array of characters to be used for encryption and decryption
          *\param input[] the array of structures
          *\param numOfPasswords the number of passwords saved in the array
          *\param the length of the website and password array in the struct
          *\return the wanted array
          */
        char * structToArray(WPTuple input[],int numOfPasswords,int lengthOfWebsite);  
        /**
          *Transforms a given array of characters into an array of WPTuples to be used for adding removing passwords
          *\param input[] the array to transform
          *\param numOfPasswords the number of passwords saved in the array
          *\param the length of the website and password array 
          *\return the wanted array of WPTuples
          */
        WPTuple * arrayToStruct(char * input,int numOfPasswords,int lengthOfWebsite); 
        /**
          *Creates the key to encrypt and decrypt data from the password that the user is going to put
          *\param password the password that the user puts
          */
        void createKey(char * password);
        /**
          *Compares two array of 32 bytes 
          *\param one first array to compare
          *\param two second array to compare
          *\return true if the two arrays contain the same chars
          */
        bool cmpChar(char * one,char * two);
        /**
          *add . to make an array into a 32 bytes array
          *\param array to add points
          *\return the char array
          */
        char * addCharacters(char * input);
        /**
          *Get the characters to print from an array of 32 bytes 
          *\param input the array to be transformed
          *\return string 
          */
        char * transformArray(char input[32]);  
        /**
          *
          */
        void changePassword(char * website);
        /**
          *gets the position of an website in the passwords array
          *\param input the array of which we want to find the position
          *\return int the position of the array -1 if not found
          */
        int getPosition(char * input);
      
};


#endif	/* PASSWORDMANAGER_H */

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

#include <FlashDriver.h>
#include <string>
#include <iostream>

using namespace std;

struct WPTuple {
	volatile char website[32];
	volatile char password[32];
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
    	WPTuple passwords[512]; //pointer?
	int checksum; // hash of passwords array
	short numOfPass; // number of currently stored passwords
	bool changed; // true when commit is necessary (flash write)
	//const unsigned int address; //address of passwords in Flash (constant 0xF8000); better with DEFINE?

	/**
   	  * stores master password to flash (encrypted)
	  * \param pass new Password
	  * \return true when no error
   	  */	
	bool setMasterPassword(string pass);
	
	/**
   	  * compares pass to the master password
	  * \param pass password inserted by user
	  * \return true when password is correct
   	  */	
	//bool checkMasterPassword(string pass); //remove
	
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
	  * \param website to be searched for
	  * \return password for the website
   	  */	
	string searchPassword(string website);

	/**
	  * prints all website password tuples on the console
   	  */	
	void printAll();
	
	/**
	  * \param website to be searched for
   	  */	
	void addPassword(string website, string password);

	/**
	  * \param website to be removed (including password)
	  * \return true: succesful removed
   	  */	
	bool remove(string website);

	/**
	  * Encrypts the attributes (data), will be saved in the attributes
   	  */	
	void encrypt();

	/**
	  * Decrypts the attributes (data)
   	  * \return true: password correct
   	  */		
	bool decrypt();
};


#endif	/* PASSWORDMANAGER_H */


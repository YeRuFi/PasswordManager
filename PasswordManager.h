/***************************************************************************

 *   Copyright (C) 2016 by                                                 * 
 *                    *
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

#define ADDRESS 0xF8000

#include "FlashDriver.h"
#include "FileSystem.h"
#include <string>

struct WPTuple {
	volatile char[32] website;
	volatile char[32] password;
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
    
 
private:
    
    
  	 /**
   	  * Constructor
   	  * 
   	  */
   	PasswordManager();

	/**
	  * Attributes
  	  */
    	WPTuple[512] passwords; //pointer?
 	//const unsigned int address; //address of passwords in Flash (constant 0xF8000); better with DEFINE?
	//unsigned int masterAdress; // necessary? 
	int numOfPass; //number of stored passwords

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
	bool checkMasterPassword(string pass);
	
	/**
   	  * initializes passwords by loading it from the Flash
   	  */	
	void loadData();
};


#endif	/* PASSWORDMANAGER_H */


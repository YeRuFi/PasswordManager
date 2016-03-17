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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/ 

#ifndef SLOTPASSWORDMANAGER_H
#define	SLOTPASSWORDMANAGER_H

//definition of the addresses for 4 slots on different sectors (!)
#define ADDRESS1 0x080F8000 //the standard address in sector 11 at the very end of the flash
#define ADDRESS2 0x080C0000 //first address of sector 10
#define ADDRESS3 0x080A0000 //first address of sector 9
#define ADDRESS4 0x08098000 //last address of sector 8 with 32KiB capacity, as far as possible from OS.

#define HELP "The Password Manager provides 4 slots on the flash. So up to 4 users can store their passwords. You can enter a number (e.g. 4) to use the correspondent slot. Note that you should remember your slot number and always use the same, in order to see your passwords. \n" //help text will be shown when help command is typed

#include <PasswordManager.h>

/**
* This class implements a PasswordManager that offers multiple slots on the flash.
* With this it is possible for multiple users to use the same board as their PasswordManager,
* while having an own slot on the flash and an own master password. 
* Note: This is only a very simple implementation that leaves large parts of the flash unused!
*/
class SlotPasswordManager
{

public:

   	 /**
   	  * 
   	  * Starts the UserInterface of the SlotPasswordManager
   	  * 
   	  */
   	 void startUI();        
 
private:
    
	//Attributes	
	/**
	  * Pointer to a PasswordManager which will be created with the address for the given slot
  	  */ 
	PasswordManager* pwm;
      
};

#endif	/* SLOTPASSWORDMANAGER_H */


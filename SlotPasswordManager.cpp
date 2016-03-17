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

#include <stdio.h>
#include <PasswordManager.h>
#include <SlotPasswordManager.h>

void SlotPasswordManager::startUI()
{
	char* input = new char[32];
	do
	{
		printf("\nWelcome to the Password Manager with multiple slots!\n");
		askinput: 
		printf("Please choose your slot or type help: \n");
		scanf("%s",input);
		if(input[0]=='h')
		{
			printf(HELP);
			printf("\n");
			goto askinput;
		}
		else if(input[0]=='1')
			pwm= new PasswordManager(ADDRESS1);
		     else if(input[0]=='2')
				pwm= new PasswordManager(ADDRESS2);
			else if(input[0]=='3')
				pwm= new PasswordManager(ADDRESS3);
			     else if(input[0]=='4')
					pwm= new PasswordManager(ADDRESS4);
				  else {
						printf("Invalid input... \n");
						goto askinput;
				       }
		pwm->startUI();
		
		delete pwm;		
		printf("Press enter to exit, type restart to login again\n");		
		scanf("%s",input);
	} while(input!="");
	delete[] input;
}

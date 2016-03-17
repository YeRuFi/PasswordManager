/***************************************************************************
 *   Passsword Manager firmware for STM32F407 board using miosix kernel.   *
 *									   *
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
 *									   *
 *   Refer to <http://www.github.com/YeRuFi/PasswordManager/>              *
 ***************************************************************************/ 

#include <miosix.h>
#include <SlotPasswordManager.h>
using namespace miosix;

int main()
{
	SlotPasswordManager mupwm;
	mupwm.startUI();
	shutdown();
}

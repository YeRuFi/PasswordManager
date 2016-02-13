/***************************************************************************

 *   Copyright (C) 2013 by                                                 * 
 *      Filippo Jacolino, Tommaso Innocenti, Fernando Faenza               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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
#include "miosix.h"
#include "FlashDriver.h"

using namespace miosix;

FlashDriver& FlashDriver::instance()
{
    static FlashDriver singleton;
    return singleton;
}

FlashDriver::FlashDriver()
{
    
}

void FlashDriver::lock()
{
    FLASH->CR |= FLASH_CR_LOCK;
}
void FlashDriver::unlock()
{
    FLASH->KEYR=FLASH_KEY1;
    FLASH->KEYR=FLASH_KEY2;
}

void FlashDriver::unlockOPT(){
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR =  FLASH_OPTKEY2;
}

bool FlashDriver::erase(int sector)
{
    InterruptDisableLock dLock; //disable all the interrupts
    unlock();
    if(FLASH->CR & FLASH_CR_LOCK) return false; //Flash is still locked
    if(FLASH->SR & FLASH_SR_BSY) return false; //Flash is busy
    FLASH->CR &=~(FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1);//clear the psize bits
    //TODO: Insert program protection on sector
    FLASH->CR |=sector<<3 | FLASH_CR_SER | FLASH_CR_STRT;
    FLASH->CR |=FLASH_PSIZE_WORD;//set the x16 parallelism
    while(FLASH->SR  & FLASH_SR_BSY);//wait the BSY bit to be cleared
    FLASH->CR &=(~FLASH_CR_SER);//clear the SER bit
    FLASH->CR &=~FLASH_SECTOR_MASK; //clear the sector
    lock();
    return true;
}
bool FlashDriver::erase()
{
    InterruptDisableLock dLock; //disable all the interrupts
    unlock();
    if(FLASH->CR & FLASH_CR_LOCK) return false; //Flash is still locked
    if(FLASH->SR & FLASH_SR_BSY) return false;//Flash is busy
    FLASH->CR &=~(FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1);//clear the psize bits
    FLASH->CR |=FLASH_CR_MER |FLASH_CR_STRT;
    FLASH->CR |=FLASH_PSIZE_HALF_WORD;//set the x16 parallelism
    while(FLASH->SR & FLASH_SR_BSY);//wait the BSY bit to be cleared
    FLASH->CR &=~FLASH_CR_MER;//clear MER bit
    lock();
    return true;
}

bool FlashDriver::write(unsigned int address, char *data, size_t count){
    
    for(int i=0;i<count;i++){
        if(*(data+i)!=0xFF){//if it's clear don't write
            if(write(address+i,*(data+i))==false)
                return false;
        }
    }
    return true;
}
unsigned int FlashDriver::getSectorAddress(int num){
    switch(num){
        case 0:
            return ADDR_FLASH_SECTOR_0;
        case 1:
            return ADDR_FLASH_SECTOR_1;
        case 2:
            return ADDR_FLASH_SECTOR_2;
        case 3:
            return ADDR_FLASH_SECTOR_3;
        case 4:
            return ADDR_FLASH_SECTOR_4;
        case 5:
            return ADDR_FLASH_SECTOR_5;
        case 6:
            return ADDR_FLASH_SECTOR_6;
        case 7:
            return ADDR_FLASH_SECTOR_7;
        case 8:
            return ADDR_FLASH_SECTOR_8;
        case 9:
            return ADDR_FLASH_SECTOR_9;
        case 10:
            return ADDR_FLASH_SECTOR_10;
        case 11:
            return ADDR_FLASH_SECTOR_11;
        default:
            return 0xFFFFFFFF;
    } 
}
int FlashDriver::getSectorNumber(unsigned int address){
    if(address>= ADDR_FLASH_SECTOR_0 && address <ADDR_FLASH_SECTOR_1)
        return 0;
    else if(address>= ADDR_FLASH_SECTOR_1 && address <ADDR_FLASH_SECTOR_2)
        return 1;
    else if(address>= ADDR_FLASH_SECTOR_2 && address <ADDR_FLASH_SECTOR_3)
        return 2;
    else if(address>= ADDR_FLASH_SECTOR_3 && address <ADDR_FLASH_SECTOR_4)
        return 3;
    else if(address>= ADDR_FLASH_SECTOR_4 && address <ADDR_FLASH_SECTOR_5)
        return 4;
    else if(address>= ADDR_FLASH_SECTOR_5 && address <ADDR_FLASH_SECTOR_6)
        return 5;
    else if(address>= ADDR_FLASH_SECTOR_6 && address <ADDR_FLASH_SECTOR_7)
        return 6;
    else if(address>= ADDR_FLASH_SECTOR_7 && address <ADDR_FLASH_SECTOR_8)
        return 7;
    else if(address>= ADDR_FLASH_SECTOR_8 && address <ADDR_FLASH_SECTOR_9)
        return 8;
    else if(address>= ADDR_FLASH_SECTOR_9 && address <ADDR_FLASH_SECTOR_10)
        return 9;
    else if(address>= ADDR_FLASH_SECTOR_10 && address <ADDR_FLASH_SECTOR_11)
        return 10;
    else if(address>= ADDR_FLASH_SECTOR_11 && address <ADDR_FLASH_SECTOR_11+128000)
        return 11;
    return -1;
}
bool FlashDriver::write(unsigned int address, char data){
    return programByte(address,data);
}
bool FlashDriver::write(unsigned int address, unsigned int data){
    //return programWord(address,data);
    return programHalfWord(address,(short)(data)) && programHalfWord(address+sizeof(short),(short)((data)>>16));
}
bool FlashDriver::write(unsigned int address, short data){
    return programHalfWord(address,data);
}
bool FlashDriver::programByte(unsigned int address, char data)
{
        InterruptDisableLock dLock; //disable all the interrupts
        unlock();
        if(FLASH->CR & FLASH_CR_LOCK) return false; //Flash is still locked
        if(FLASH->SR & FLASH_SR_BSY) return false;//Flash is busy
        FLASH->CR &=~(FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1);//clear the psize bits
        FLASH->CR |=FLASH_PSIZE_BYTE | FLASH_CR_PG;
        *( char*)address=data;
        while(FLASH->SR & FLASH_SR_BSY);//wait the BSY bit to be cleared
        FLASH->CR &=~FLASH_CR_PG;//clear the PG bit
        lock();
    return true;
}
bool FlashDriver::programWord(unsigned int address, unsigned int data)
{
        InterruptDisableLock dLock; //disable all the interrupts
        unlock();
        if(FLASH->CR & FLASH_CR_LOCK) return false; //Flash is still locked
        if(FLASH->SR & FLASH_SR_BSY) return false;//Flash is busy
        FLASH->CR &=~(FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1);//clear the psize bits
        FLASH->CR |=FLASH_PSIZE_WORD | FLASH_CR_PG;
        *( int*)address=data;
        while(FLASH->SR & FLASH_SR_BSY);//wait the BSY bit to be cleared
        FLASH->CR &=~FLASH_CR_PG;//clear the PG bit
        lock();
    return true;
}

bool FlashDriver::programHalfWord(unsigned int address, short data)
{
        InterruptDisableLock dLock; //disable all the interrupts
        unlock();
        if(FLASH->CR & FLASH_CR_LOCK) return false; //Flash is still locked
        if(FLASH->SR & FLASH_SR_BSY) return false;//Flash is busy
        FLASH->CR &=~(FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1);//clear the psize bits
        FLASH->CR |=FLASH_PSIZE_HALF_WORD | FLASH_CR_PG;
        *(short*)address=data;
        while(FLASH->SR & FLASH_SR_BSY);//wait the BSY bit to be cleared
        FLASH->CR &=~FLASH_CR_PG;//clear the PG bit
        lock();
    return true;
}

bool FlashDriver::programDoubleWord(unsigned int address, double data){
    InterruptDisableLock dLock; //disable all the interrupts
    unlock();
    if(FLASH->CR & FLASH_CR_LOCK) return false; //Flash is still locked
    if(FLASH->SR & FLASH_SR_BSY) return false;//Flash is busy
    FLASH->CR &=~(FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1);//clear the psize bits
    FLASH->CR |=FLASH_PSIZE_DOUBLE_WORD | FLASH_CR_PG;
    *(double*)address=data;
    while(FLASH->SR & FLASH_SR_BSY);//wait the BSY bit to be cleared
    FLASH->CR &=~FLASH_CR_PG;//clear the PG bit
    lock();
    return true;
}
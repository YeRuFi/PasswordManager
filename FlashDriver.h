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
#ifndef FLASHDRIVER_H
#define	FLASHDRIVER_H

#define FLASH_PSIZE_BYTE           ((int)0x00000000)
#define FLASH_PSIZE_HALF_WORD      ((int)0x00000100)
#define FLASH_PSIZE_WORD           ((int)0x00000200)
#define FLASH_PSIZE_DOUBLE_WORD    ((int)0x00000300)

#define FLASH_Sector_0     ((int)0x0000) /*!< Sector Number 0 */
#define FLASH_Sector_1     ((int)0x0008) /*!< Sector Number 1 */
#define FLASH_Sector_2     ((int)0x0010) /*!< Sector Number 2 */
#define FLASH_Sector_3     ((int)0x0018) /*!< Sector Number 3 */
#define FLASH_Sector_4     ((int)0x0020) /*!< Sector Number 4 */
#define FLASH_Sector_5     ((int)0x0028) /*!< Sector Number 5 */
#define FLASH_Sector_6     ((int)0x0030) /*!< Sector Number 6 */
#define FLASH_Sector_7     ((int)0x0038) /*!< Sector Number 7 */
#define FLASH_Sector_8     ((int)0x0040) /*!< Sector Number 8 */
#define FLASH_Sector_9     ((int)0x0048) /*!< Sector Number 9 */
#define FLASH_Sector_10    ((int)0x0050) /*!< Sector Number 10 */
#define FLASH_Sector_11    ((int)0x0058) /*!< Sector Number 11 */
#define FLASH_SECTOR_MASK   ((int)0x00000078)
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((unsigned int)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((unsigned int)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((unsigned int)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((unsigned int)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((unsigned int)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((unsigned int)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((unsigned int)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((unsigned int)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((unsigned int)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((unsigned int)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((unsigned int)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((unsigned int)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */
#define ADDR_FLASH_SECTOR_MASK  ((unsigned int)0x080E0000) //mask for find out what sector belongs a certain address

#define SECTOR_SIZE     ((unsigned int)0x00020000) //each sector is 128 KB
//KEYS:
#define FLASH_OPTKEY1 ((unsigned int) 0x08192A3B)
#define FLASH_OPTKEY2 ((unsigned int) 0x4C5D6E7F)
#define FLASH_KEY1 ((unsigned int)  0x45670123)
#define FLASH_KEY2 ((unsigned int) 0xCDEF89AB)

#include "stddef.h";
#include "FileSystem.h"

class FlashDriver
{

public:
    /**
     * 
     * @return an instance of the FlashDriver
     */
    static FlashDriver& instance();
    /**
     * 
     * @param address
     * @return true on success, false on failure
     */
    bool erase(int sector);
    /**
     * Erase all the sectors
     * @return true on success, false on failure
     */
    bool erase();
    /**
     * 
     * @param address
     * @param data to write
     * @return true on success, false on failure
     */
    bool write(unsigned int address, unsigned int data);
    /**
     * Writes to the flash the given data
     * @param address
     * @param data
     * @return true on success, false on failure
     */
    bool write(unsigned int address, short data);
   /**
     * Writes to the flash the given data
     * @param address
     * @param data
     * @return true on success, false on failure
     */
    bool write(unsigned int address, char data);

    /**
     * 
     * @param address
     * @return the sector where this address is
     */
    int getSectorNumber(unsigned int address);
    /**
     * 
     * @param num
     * @return the address of the num-sector, 0xFFFFFFFF if the num-sector doesn't exist
     */
    unsigned int getSectorAddress(int num);
    /**
     * Writes count bytes to a buffer (data) in the given address
     * @param address where to writes
     * @param data buffer
     * @param count how many bytes to write
     * @return 
     */    
    bool write(unsigned int address, char *data, size_t count);
            
    /**
     * 
     * @return the first sector of the FS 
     */
    unsigned int getStartSector() { return ADDR_FLASH_SECTOR_8; }
    /**
     * 
     * @return the last address of the FS 
     */
    unsigned int getLastAddress() { return ADDR_FLASH_SECTOR_11 - sizeof(Header); }
    /**
     * 
     * @return the sector used as a buffer 
     */
    unsigned int getBufferSector() { return ADDR_FLASH_SECTOR_11; }
 
private:
    /**
     * Constructor
     * 
     */
    FlashDriver();
    /**
     * writes to the flash a byte of a given data
     * @param address
     * @param data
     * @return true on success, false on failure
     */
    bool programByte(unsigned int address,char data);

    /**
     * write into the flash 16-bit of data
     * @param address
     * @param data
     * @return true on success, false on failure
     */
    bool programHalfWord(unsigned int address,short data);
    /**
     * write into the flash 32-bit of data
     * @param address
     * @param data
     * @return true on success, false on failure
     */
    bool programWord(unsigned int address, unsigned int data);
    /**
     * write into the flash 64-bit of data
     * @param address
     * @param data
     * @return true on success, false on failure
     */
    bool programDoubleWord(unsigned int address, double data);
    /**
     * Unlocks the Flash option control registers access
     */
    void unlockOPT();
    /**
     * Locks the Flash option control registers access
     */
    void lockOPT();
    /**
     * Unlocks the Flash control register access
     */
    void unlock();
    /**
     * Locks the Flash control register access
     */
    void lock(); 

};


#endif	/* FLASHDRIVER_H */


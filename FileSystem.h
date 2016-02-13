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
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <sys/stat.h>

#include "FlashDriver.h"
#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#define ALIGMENT 4//in byte
//ATTENTION size of the filename length field must be divisible for 2
#define FS_FILE_NAME_MAX_LENGHT 24//6*ALIGMENT in bytes

#define FS_SECTOR_ROOT 4
#define FS_SECTOR_ROOF 10
#define FS_SECTOR_BUFFER 11
#define FS_ROOT_DIM ALIGMENT
#define FS_BLANK (unsigned int)0xFFFFFFFF
/**
 * Header struct
 */
struct Header{
    unsigned int next;//points to the next file (4 bytes)
    unsigned int size;//unused, for future implementations (4 bytes)
    uid_t uid;//the user identity (2 bytes)
    gid_t gid;//the group identity (2 bytes)
    char filename[FS_FILE_NAME_MAX_LENGHT];//the file name (FS_FILE_NAME_MAX_LENGHT bytes)
};
class File{
public:
    /**
     *  write() writes up to count bytes from the buffer pointed buf to the file referred to by the file descriptor
     *
     * @param buff
     * @param count
     * @return On success, the number of bytes written is returned. On error, negative value is returned
     */
    ssize_t write(const void *buff,size_t count);
    /**
     *  read() attempts to read up to count bytes from file descriptor into the buffer starting at buf.
     * @param buf
     * @param count
     * @return On  success, the number of bytes read is returned. On error, negative value is returned
     */
    ssize_t read(void *buf,size_t count);
    /**
     * This  function  returns information about a file.
     * @param buf
     * @return  On success, zero is returned. On error, negative value is returned
     */
    int fstat(struct stat *buf);
    /**
     * The  lseek() function repositions the offset of the open file associated with the file descriptor to the argument offset according to the
     * directive whence
     * @param offset
     * @param whence
     * @return Upon  successful  completion,  lseek() returns the resulting offset location as measured in bytes from the beginning of the file. On failure, a negative value is returned
     */
    off_t lseek(off_t offset,int whence);
    /**
     * 
     * @return a pointer to the first byte of the file
     */
    unsigned int getBasePointer();
    /**
     * 
     * @return the size of the file 
     */
    size_t getSize();
    /**
     * Constructor
     * @param basePointer
     * @param lenght
     * @param uid
     * @param gid
     */
    File(unsigned int basePointer,size_t lenght, uid_t uid, gid_t gid);
private:
    unsigned int basePointer;
    int fp;
    size_t lenght;
    uid_t uid;
    gid_t gid;
    
};



class FileSystem{
public:
    /**
     * Given  a  pathname  for  a file, open() returns an instance of a File.
     * @param filename of the file to open
     * @param flags
     * @param size of the size
     * @return File or NULL on error occurred
     */
    File* open(const char *filename,int flags,int size);
    /**
     * This  function  returns  information about a file.
     * @param filename of the file
     * @param buf where to save the file status
     * @return on success 0 is returned, negative value on failure
     */
    int lstat(char *filename,struct stat* buf);
    /**
     * Given a pathname for a file, unlink() deletes the file
     * @param filename of the file
     * @return on success 0 is returned, on error negative value
     */
    int unlink(const char *filename);
    
    /**
     * clear all the FS erasing all the sectors of the FS
     */
    static void clearFS();
    /**
     * 
     * @return the instance of the FileSystem 
     */
    static FileSystem& instance();
    
    /**
     * Writes the header passed as a parameter to the address
     * @param address where write the header
     * @param header to be written
     * @return true on success false on failure
     */
    bool writeHeader(unsigned int address,Header header);

private:
    /**
    * Constructor
    */
    FileSystem();
    /**
     * readHeader() reads the header of a given address
     * @param address where the header is
     * @return the header
     */
    static struct Header readHeader(unsigned int address);
    /**
     * This function finds a file in the FS starting from the root.
     * @return true if the file exist, otherwise false 
     */
    bool findFile(const char *filename);

    /**
     * Writes the header passed as a parameter to the address and restore the sectors from address to header.next
     * @param address
     * @param header
     * @return 
     */
    bool writeHeaderRestoringSectors(unsigned int address,Header header);
    
    /**
     * Writes the header passed as a parameter to the address and restore the sectors from address to header.next
     * and then it writes a falseHeader upon the header.next address
     * @param address
     * @param header
     * @param falseHeader
     * @return 
     */
    bool writeHeaderRestoringSectors(unsigned int address,Header header,Header falseHeader);
    /**
     * getAddress() gets the address of a given file name
     * @param filename to find
     * @return The address of the filename, FS_BLANK if the file doesn't exist
     */
    unsigned int getAddress(const char* filename);

    /**
     * isValid() tells if a file is valid or not
     * @param address of the file
     * @return true if the file is valid, otherwise false.
     */
    bool isValid(unsigned int address);
    /**
     * 
     * @return the uid of the user 
     */
    uid_t getuid(){return 0xBBBB;}
    /**
     * 
     * @return the gid of the user
     */
    gid_t getgid(){return 0xAAAA;}
};
#endif	/* FILESYSTEM_H */


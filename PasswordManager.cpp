#include <miosix.h>
#include <string>
#include <stdio.h>
#include <FlashDriver.h>
#include <PasswordManager.h>


using namespace miosix;
using namespace std;


/*PasswordManager::PasswordManager(): 
	address(0x080F8000){}
*/

void PasswordManager::startUI()
{
	string command="help";	
	printf("Welcome to the Password Manager\n");
	
	//cout<<"Loading data... please wait.";
	//PasswordManager::loadData();
	//cout<<"Please enter the master password: \n";
		
}

/**
 * Flash Layout for store and load Data functions
 * standard start address is used as example:
 *
 * Address 	Content 	Comment
 * 0x080F8000	'P'		Identification
 * 0x080F8001	'W'		"
 * 0x080F8002	'M'		"
 * 0x080F8003	0x00		"
 * 0x080F8004	numOfPass	short
 * 0x080F8006	passwords 	encryptedData
 * 0x080F8007	"		"
 * ...		...		...
 * 0x080F8006+numOfPass*64 +16 //64 for every password, 16 for the checksum
 * Afterwards	0xFF		"
 * ...		...		... //empty area reserved for new passwords
 * 0x080FFFFF	0xFF		"
 *
 */
bool PasswordManager::storeData()
{
	bool success = false; //return value, true when erase and all writes have been successful	
	// identifier to identify first use and to make sure invalid data is never read coincidentally:
	char ident[4] = "PWM"; 
	//get flash driver instance (singleton):
	FlashDriver& flashdriver = FlashDriver::instance();
	//erase the sector to store password data:
	success = flashdriver.erase(flashdriver.getSectorNumber(address)); //necessary for flash (all is put to 0xFF)
	//write identifier to the flash:
	success &= flashdriver.write(address, ident, 4);
	char* ramAddress = (char*) &numOfPass; //get address of numOfPass
	//write numOfPass of length 2 (short) to the flash:	
	success &= flashdriver.write(address+4, ramAddress, 2);
	//write char array of length sizeof(WPTuple)*numOfPass to flash
	success &= flashdriver.write(address+6, ramAddress, PASSWORDLENGTH*2*numOfPass);
	return success;
}

	
//}

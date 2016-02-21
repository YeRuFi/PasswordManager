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

bool PasswordManager::storeData()
{
	bool success = false; //return value, true when erase and all writes have been successful	

	//get flash driver instance (singleton)	
	FlashDriver& flashdriver = FlashDriver::instance();
	
	//erase the sector to store password data 
	success = flashdriver.erase(flashdriver.getSectorNumber(ADDRESS)); //necessary for flash (all is put to 0xFF)

	char* ramAddress = (char*) passwords; //get address of first array element
	
	//write char array of length sizeof(WPTuple)*arraylength to flash	
	success &= flashdriver.write(ADDRESS, ramAddress, 64*510);

	ramAddress = (char*) &checksum; //get address of checksum
	
	//write char array of length sizeof(int) to flash
	success &= flashdriver.write(ADDRESS+64*510, ramAddress, sizeof(int));
	return success;
}

	
//}

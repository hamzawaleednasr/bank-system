/*

		- MADE WITH <3 BY Hamza Waleed --> 'Full-Stack Developer' -

*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <chrono>
#include <iomanip>
#include <sstream>

// ====== Declration of controlers ======
void clientControler();
void transactionsControler();
void usersControler();

// ================== Global Values ==================

struct stClient {
    std::string accNumber;
    std::string name;
    std::string phoneNumber;
    int pin;
    long double balance;
};
struct stUser {
    std::string username;
    std::string password;
    int permissions;
};

enum enClientChoices { cList = 1, cAdd, cDel, cUpdate, cFind, cTrans, cManageUsers, cLogout };
enum enTransChoices { dep = 1, wit, mainMenu };
enum enUsersChoices { uList = 1, uAdd, uDel, uUpdate, uFind, uMainMenu };

namespace config {
	const std::string clientsFile = "clients.txt";
	const std::string usersFile = "users.txt";
	const std::string loggingFile = "bank_audit.txt";

	struct stPermisions {
		static constexpr int LIST = 1 << 1;
		static constexpr int ADD = 1 << 2;
		static constexpr int DELETE = 1 << 3;
		static constexpr int UPDATE = 1 << 4;
		static constexpr int FIND = 1 << 5;
		static constexpr int TRANS = 1 << 6;
		static constexpr int MANAGE_USERS = 1 << 7;
	};
}
namespace session {
	stUser currentUser;
}

// ============= Check permissions =============
bool isAllowed(int choice)
{
	return (choice & session::currentUser.permissions);
}

// ================== Hashing ==================
std::string hashPassword(std::string dataToHash)
{
	std::hash<std::string> hasher;
	std::string hashedData = std::to_string(hasher(dataToHash));

	return hashedData;
}

// ============= File Operation Functions ==============

//		   ----- Load File Data To Vector -----

// | - General - |
std::vector<std::string> splitString(std::string record, const std::string& delimeter = " ")
{
	std::vector<std::string> vWords;
	int pos = 0;
	std::string sWord;



	while ((pos = record.find(delimeter)) != std::string::npos)
	{
		sWord = record.substr(0, pos);

		if (sWord != "")
			vWords.push_back(sWord);

		record.erase(0, pos + delimeter.length());
	}

	if (record != "")
		vWords.push_back(record);

	return vWords;

}

std::vector<std::string> lineToRecord(std::string& line)
{
	std::vector<std::string> client = splitString(line, "#//#");

	return client;
}

// | - Client - |
stClient saveRecordInClientStruct(const std::vector<std::string>& client)
{
	stClient sClient;

	sClient.accNumber = client[0];
	sClient.name = client[1];
	sClient.phoneNumber = client[2];
	sClient.pin = stoi(client[3]);
	sClient.balance = stod(client[4]);

	return sClient;
}

std::vector<stClient> loadClientsFromFile()
{
	std::vector<stClient> clients;
	std::string line = "";

	std::fstream file;
	file.open(config::clientsFile, std::ios::in);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			clients.push_back(saveRecordInClientStruct(lineToRecord(line)));
		}
	}

	return clients;
}

// | - User - |
stUser saveRecordInUserStruct(const std::vector<std::string>& user)
{
	stUser sUser;

	sUser.username = user[0];
	sUser.password = user[1];
	sUser.permissions = stoi(user[2]);

	return sUser;
}

std::vector<stUser> loadUsersFromFile()
{
	std::vector<stUser> users;
	std::string line = "";

	std::fstream file;
	file.open(config::usersFile, std::ios::in);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			users.push_back(saveRecordInUserStruct(lineToRecord(line)));
		}
	}

	return users;
}

//		     ----- Save Vector To File -----

// | - General - |
void appendToFile(const std::string& line, const std::string& fileName)
{
	std::fstream file;
	file.open(fileName, std::ios::app);

	if (file.is_open())
		file << line << std::endl;
}

// | - Client - |
std::string recordToLine(const stClient& record)
{
	return (record.accNumber + "#//#" + record.name + "#//#" + record.phoneNumber + 
		"#//#" + std::to_string(record.pin) + "#//#" + std::to_string(record.balance));
}

void saveClientsInFile(const std::vector<stClient>& clients)
{
	std::fstream file;
	file.open(config::clientsFile, std::ios::out);

	if (file.is_open())
	{
		for (stClient client : clients)
		{
			file << recordToLine(client) << std::endl;
		}
	}
}

// | - User - |
std::string recordToLine(const stUser& record)
{
	return (record.username + "#//#" + record.password + "#//#" + std::to_string(record.permissions));
}

void saveUsersInFile(const std::vector<stUser>& users)
{
	std::fstream file;
	file.open(config::usersFile, std::ios::out);

	if (file.is_open())
	{
		for (stUser user : users)
		{
			file << recordToLine(user) << std::endl;
		}
	}
}

// ==================== Display Functions ====================

// | - General - |
void printHeader(const std::string& header)
{
	std::cout << "\n--------------------------------------------------" << std::endl;
	std::cout << "                       " << header << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
}

void printMainMenu()
{
	std::cout << "========================================================\n";
	std::cout << "                    Main Menu Screen                    \n";
	std::cout << "========================================================\n";
	std::cout << "\t[1] Show Client List.\n";
	std::cout << "\t[2] Add New Client.\n";
	std::cout << "\t[3] Delete Client.\n";
	std::cout << "\t[4] Update Client Info.\n";
	std::cout << "\t[5] Find Client.\n";
	std::cout << "\t[6] Transactions.\n";
	std::cout << "\t[7] Manage Users.\n";
	std::cout << "\t[8] Logout.\n";
	std::cout << "========================================================\n";
}

void printTransactions()
{
	std::cout << "========================================================\n";
	std::cout << "                  Transactions Screen                   \n";
	std::cout << "========================================================\n";
	std::cout << "\t[1] Deposite.\n";
	std::cout << "\t[2] Withdraw.\n";
	std::cout << "\t[3] Main Menu.\n";
	std::cout << "========================================================\n";
}

void printManagementUsers()
{
	std::cout << "========================================================\n";
	std::cout << "                      Manage Users                      \n";
	std::cout << "========================================================\n";
	std::cout << "\t[1] List Users.\n";
	std::cout << "\t[2] Add New User.\n";
	std::cout << "\t[3] Delete User.\n";
	std::cout << "\t[4] Update User .\n";
	std::cout << "\t[5] Find User.\n";
	std::cout << "\t[6] Main Menu.\n";
	std::cout << "========================================================\n";
}

void printAccessIsDenied()
{
	system("cls");

	std::cout << "\n----------------------------------------------\n";
	std::cout << "Access is denied!,\n";
	std::cout << "You don't have permission to access this page,\n";
	std::cout << "Please call you admin.\n";
	std::cout << "----------------------------------------------\n";
}

// | - Client - |
void printTableOfClients(const std::vector<stClient>& clients)
{
	std::cout << "\t\t\t   Client List (" << clients.size() << ") Client(s)." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------\n";
	std::cout << "| " << std::left << std::setw(20) << "Account Number" << "| " << std::left << std::setw(10) << "PIN Code" << "| " << std::left << std::setw(25) << "Client Name" << "| " << std::left << std::setw(20) << "Phone" << "| " << std::left << std::setw(10) << "Balance" << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------\n";
	for (stClient client : clients)
	{
		std::cout << "| " << std::left << std::setw(20) << client.accNumber
			<< "| " << std::left << std::setw(10) << client.pin
			<< "| " << std::left << std::setw(25) << client.name
			<< "| " << std::left << std::setw(20) << client.phoneNumber
			<< "| " << std::left << std::setw(20) << std::fixed << client.balance << "\n";
	}
	std::cout << "------------------------------------------------------------------------------------------------------\n";
}

void printClientCard(const stClient& client)
{
	std::cout << "\n";
	std::cout << "-----------------------------------------------------------\n";
	std::cout << "Account number : " << client.accNumber << std::endl;
	std::cout << "PIN code       : " << client.pin << std::endl;
	std::cout << "Name           : " << client.name << std::endl;
	std::cout << "Phone          : " << client.phoneNumber << std::endl;
	std::cout << "Account balance: " << client.balance << std::endl;
	std::cout << "-----------------------------------------------------------\n";
}

// | - User - |
void printTableOfUsers(const std::vector<stUser>& users)
{
	std::cout << "\t\t\t   Users List (" << users.size() << ") User(s)." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------\n";
	std::cout << "| " << std::left << std::setw(30) << "Username" << "| " << std::left << std::setw(25) << "Password" << "| " << std::left << std::setw(10) << "Permissions" << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------\n";
	for (stUser user : users)
	{
		std::cout << "| " << std::left << std::setw(30) << user.username
			<< "| " << std::left << std::setw(25) << "**********"
			<< "| " << std::left << std::setw(10) << user.permissions << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------------\n";
}

void printUserCard(const stUser& user)
{
	std::cout << "\n";
	std::cout << "-----------------------------------------------------------\n";
	std::cout << "Username: " << user.username << std::endl;
	std::cout << "Password: " << "**********" << std::endl;
	std::cout << "Permissions: " << user.permissions << std::endl;
	std::cout << "-----------------------------------------------------------\n";
}

// ======================== Generators ========================

int randomNumber(int from, int to)
{
	return rand() % (to - from + 1) + from;
}

std::string generateAccountNumber(const std::vector<stClient>& clients)
{
	

	while (true)
	{
		std::string accountNumber = char(randomNumber(65, 90)) + std::to_string(randomNumber(100, 999));

		bool exist = false;

		for (stClient client : clients)
			if (client.accNumber == accountNumber)
			{
				exist = true;
				break;
			}

		if (!exist)
			return accountNumber;
	}	
}

short generatePIN()
{
	return randomNumber(1000, 9999);
}

// ================== Validation ==================

int readNumberInRange(int from, int to, const std::string& message = "Enter a number")
{
	int number;

	while (true)
	{
		std::cout << message << " [" << from << "-" << to << "]: ";

		if (std::cin >> number && number >= from && number <= to)
			return number;

		std::cout << "Invalid input, Please try again.\n";

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<size_t>::max(), '\n');
	}
}

long double readBalance(const std::string& message = "Enter balance")
{
	long double balance;

	while (true)
	{
		std::cout << message << ": ";
		
		if (std::cin >> balance && balance > 0)
			return balance;

		std::cout << "\nInvalid balance, please enter a positive number . . . \n";

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<size_t>::max(), '\n');
	}
}

bool isUsernameValid(const std::string& username, const std::vector<stUser>& users)
{
	for (stUser user : users)
		if (user.username == username)
			return false;

	return true;
}

bool isPhoneNumberValid(const std::string& phone)
{
	return (phone.length() == 11) && (phone[0] == '0' && phone[1] == '1') && (phone[2] == '1' || phone[2] == '2' || phone[2] == '5' || phone[2] == '0');
}

// ==================== Inputs ====================

// | - Client - |
std::string readPhoneNumber()
{
	std::string phoneNumber;

	while (true)
	{
		std::cout << "Enter phone number: ";
		std::cin >> phoneNumber;

		if (isPhoneNumberValid(phoneNumber))
			return phoneNumber;
		else
			std::cout << "\nInvalid phone number!, please try again . . . \n\n";
	}
}

stClient readNewClient(const std::vector<stClient>& clients)
{
	stClient client;

	std::cout << "Enter name: ";
	getline(std::cin >> std::ws, client.name);

	client.phoneNumber = readPhoneNumber();

	client.balance = readBalance();

	client.accNumber = generateAccountNumber(clients);
	client.pin = generatePIN();

	return client;
}

stClient readUpdateClient(stClient& client)
{
	std::cout << "\n\nEnter name: ";
	getline(std::cin >> std::ws, client.name);

	client.phoneNumber = readPhoneNumber();

	client.pin = readNumberInRange(1000, 9999, "Enter pin code");

	client.balance = readBalance();

	return client;
}

std::string readAccountNumber()
{
	std::string accountNumber;

	std::cout << "Enter account number: ";
	std::cin >> accountNumber;

	return accountNumber;
}

// | - User - |
std::string readUsername(const std::vector<stUser>& users)
{
	std::string username;

	while (true)
	{
		std::cout << "Enter username: ";
		std::getline(std::cin >> std::ws, username);

		if (isUsernameValid(username, users))
			return username;
		else
			std::cout << "\nUsername exist!, try another one . . . \n\n";
	
	}

}

std::string readPassword()
{
	std::string pass;
	
	std::cout << "Enter password: ";
	std::getline(std::cin >> std::ws, pass);

	return pass;
}

int readPermissions()
{
	int perm = 0;
	char isHave;

	std::cout << "\nDo you want to give full access? [y/n]: ";
	std::cin >> isHave;

	if (isHave == 'Y' || isHave == 'y')
		perm = -1;
	else
	{
		std::cout << "\nList clients? [y/n]: ";
		std::cin >> isHave;
		if (isHave == 'Y' || isHave == 'y')
			perm += config::stPermisions::LIST;

		std::cout << "\nAdd clients? [y/n]: ";
		std::cin >> isHave;
		if (isHave == 'Y' || isHave == 'y')
			perm += config::stPermisions::ADD;

		std::cout << "\nDelete clients? [y/n]: ";
		std::cin >> isHave;
		if (isHave == 'Y' || isHave == 'y')
			perm += config::stPermisions::DELETE;

		std::cout << "\nUpdate clients? [y/n]: ";
		std::cin >> isHave;
		if (isHave == 'Y' || isHave == 'y')
			perm += config::stPermisions::UPDATE;

		std::cout << "\nFind clients? [y/n]: ";
		std::cin >> isHave;
		if (isHave == 'Y' || isHave == 'y')
			perm += config::stPermisions::FIND;

		std::cout << "\nTransactions? [y/n]: ";
		std::cin >> isHave;
		if (isHave == 'Y' || isHave == 'y')
			perm += config::stPermisions::TRANS;

		std::cout << "\nManage users? [y/n]: ";
		std::cin >> isHave;
		if (isHave == 'Y' || isHave == 'y')
			perm += config::stPermisions::MANAGE_USERS;

	}

	return perm;

}

stUser readNewUser(const std::vector<stUser>& users)
{
	stUser newUser;

	newUser.username = readUsername(users);
	newUser.password = hashPassword(readPassword());
	newUser.permissions = readPermissions();

	return newUser;
}

stUser readUpdateUser(stUser& user)
{
	std::cout << "\n\nEnter username: ";
	getline(std::cin >> std::ws, user.username);

	user.permissions = readPermissions();

	return user;
}

// =================== Logging System ===================

static std::string getTimeNow()
{
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		std::tm localTime{};
		#ifdef _WIN32
			localtime_s(&localTime, &time);
		#else
			localtime_r(&time, &localTime);
		#endif

		std::ostringstream oss;
		oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
		return oss.str();
}

// ==================== Bank Operations ====================

// | - Client - |
void listAllClients(const std::vector<stClient>& clients)
{
	system("cls");

	if (isAllowed(config::stPermisions::LIST))
		printTableOfClients(clients);
	else
		printAccessIsDenied();
	
	std::cout << "Press any key to return main menu....";
	system("pause>0");

	clientControler();
}

void addNewClient(const stClient& newClient)
{
	appendToFile(recordToLine(newClient), config::clientsFile);
}

int findClient(const std::string& accountNumber, const std::vector<stClient>& clients)
{
	int index = 1;
	for (stClient client : clients)
	{
		if (client.accNumber == accountNumber)
			return index;
		index++;
	}

	return 0;
}

void deleteClient(std::vector<stClient>& clients, int clientIndex)
{
	clients.erase(clients.begin() + clientIndex);
}

void updateClient(std::vector<stClient>& clients, int clientIndex)
{
	readUpdateClient(clients[clientIndex]);
}

// | - Transactions Client - |
void deposite(std::vector<stClient>& clients, int clientIndex, double amount)
{
	clients[clientIndex].balance += amount;
	saveClientsInFile(clients);
}

void withdraw(std::vector<stClient>& clients, int clientIndex, double amount)
{
	clients[clientIndex].balance -= amount;
	saveClientsInFile(clients);
}

// | - User - |
void listAllUsers(const std::vector<stUser>& users)
{
	system("cls");

	printTableOfUsers(users);

	std::cout << "Press any key to return management user menu . . . ";
	system("pause>0");

	usersControler();
}

void addNewUser(const stUser& newUser)
{
	appendToFile(recordToLine(newUser), config::usersFile);
}

int findUser(const std::string& username, const std::string& password, const std::vector<stUser>& users)
{
	int index = 1;
	for (stUser user : users)
	{
		if (user.username == username && user.password == hashPassword(password))
			return index;
		index++;
	}

	return 0;
}

void deleteUser(std::vector<stUser>& users, int userIndex)
{
	users.erase(users.begin() + userIndex);
}

void updateUser(std::vector<stUser>& users, int userIndex)
{
	readUpdateUser(users[userIndex]);
}


// ================== Run Bank Operations (UI) ==================

// | - Client - |
int checkClientIsThere(const std::vector<stClient>& clients)
{
	std::string accountNumber = readAccountNumber();
	int clientIndex = findClient(accountNumber, clients);

	while (!clientIndex)
	{
		std::cout << "\nClient with account number [" << accountNumber << "]" << " is NOT found!";
		std::cout << "\nPlease try again . . .\n\n";

		accountNumber = readAccountNumber();
		clientIndex = findClient(accountNumber, clients);
	}

	return clientIndex;
}

void runAddClient(const std::vector<stClient>& clients)
{
	system("cls");

	if (isAllowed(config::stPermisions::ADD))
	{

		printHeader("Add Client");

		stClient newClient = readNewClient(clients);
		addNewClient(newClient);

		std::cout << "\nClient added succefully! - press any key to return the main menu . . . ";

	}
	else
	{
		printAccessIsDenied();
		std::cout << "\npress any key to return the main menu . . . ";
	}


	system("pause>0");

	clientControler();
}

void runFindClient(const std::vector<stClient>& clients)
{
	system("cls");

	if (isAllowed(config::stPermisions::FIND))
	{
		printHeader("Find Client");

		int clientIndex = checkClientIsThere(clients);
		printClientCard(clients[clientIndex - 1]);
	}
	else
		printAccessIsDenied();


	std::cout << "\n\nPress any key to return main menu . . . ";
	system("pause>0");

	clientControler();
}

void runDeleteClient(std::vector<stClient>& clients)
{
	system("cls");
	
	if (isAllowed(config::stPermisions::DELETE))
	{
		printHeader("Delete Client");

		int clientIndex = checkClientIsThere(clients);
		printClientCard(clients[clientIndex - 1]);

		char wantToDelete = 'n';
		std::cout << "Are you sure you want to delete this client? [y/n] ";
		std::cin >> wantToDelete;

		if (wantToDelete == 'y' || wantToDelete == 'Y')
		{
			deleteClient(clients, clientIndex - 1);
			saveClientsInFile(clients);
			std::cout << "\nClient deleted successfully!";
		}
	}
	else
		printAccessIsDenied();

	std::cout << "\nPress any key to return main menu . . . ";
	system("pause>0");

	clientControler();

}

void runUpdateClient(std::vector<stClient>& clients)
{
	system("cls");

	if (isAllowed(config::stPermisions::UPDATE))
	{
		printHeader("Update Client");

		int clientIndex = checkClientIsThere(clients);
		printClientCard(clients[clientIndex - 1]);

		char wantToUpdate = 'n';
		std::cout << "Are you sure you want to update this client? [y/n] ";
		std::cin >> wantToUpdate;

		if (wantToUpdate == 'y' || wantToUpdate == 'Y')
		{
			updateClient(clients, clientIndex - 1);
			saveClientsInFile(clients);
			std::cout << "\n\nClient Updated successfully!";
		}
	}
	else
		printAccessIsDenied();

	std::cout << "\nPress any key to return main menu . . . ";
	system("pause>0");

	clientControler();

}

// | - Transactions Client - |
void runDeposite(std::vector<stClient>& clients)
{
	system("cls");

	printHeader("Deposite");

	int clientIndex = checkClientIsThere(clients);
	printClientCard(clients[clientIndex - 1]);
	double amount;

	char wantToDeposite = 'n';
	std::cout << "Are you sure you want to deposite this client? [y/n] ";
	std::cin >> wantToDeposite;

	if (wantToDeposite == 'y' || wantToDeposite == 'Y')
	{
		amount = readBalance("\nEnter the amount to deposite");
		deposite(clients, clientIndex - 1, amount);
		std::cout << "\nThe amount deposited succefully!" << std::endl;
	}

	std::cout << "\nPress any key to return transactions menu . . . ";
	system("pause>0");

	transactionsControler();
}

void runWithdraw(std::vector<stClient>& clients)
{
	system("cls");

	printHeader("Withdraw");

	int clientIndex = checkClientIsThere(clients);
	printClientCard(clients[clientIndex - 1]);
	double amount;

	char wantToWithdraw = 'n';
	std::cout << "Are you sure you want to withdraw from this client? [y/n] ";
	std::cin >> wantToWithdraw;

	if (wantToWithdraw == 'y' || wantToWithdraw == 'Y')
	{
		amount = readBalance("\nEnter the amount to withdraw");

		while (amount > clients[clientIndex - 1].balance)
		{
			std::cout << "\nNot enough amount!, please try again . . . " << std::endl;
			amount = readBalance("\nEnter the amount to withdraw");
		}

		withdraw(clients, clientIndex - 1, amount);
		std::cout << "\nThe amount withdrawed succefully!" << std::endl;
	}

	std::cout << "\nPress any key to return transactions menu . . . ";
	system("pause>0");

	transactionsControler();
}

// | - User - |
int checkUserIsThere(const std::vector<stUser>& users)
{
	std::string username;
	std::cout << "Enter username: ";
	std::cin >> username;
	std::string password = readPassword();

	int userIndex = findUser(username, password, users);

	while (!userIndex)
	{
		std::cout << "\nInvalid Username/Password, Please try again . . .\n\n";

		std::cout << "\nEnter username: ";
		std::cin >> username;
		password = readPassword();

		userIndex = findUser(username, password, users);
	}

	return userIndex;
}

void runAddUser(const std::vector<stUser>& users)
{
	system("cls");

	printHeader("Add User");

	stUser newUser = readNewUser(users);
	addNewUser(newUser);

	std::cout << "\nUser added succefully! - press any key to return the management users menu . . . ";
	system("pause>0");

	usersControler();
}

void runFindUser(const std::vector<stUser>& users)
{
	system("cls");

	printHeader("Find User");

	int userIndex = checkUserIsThere(users);
	printUserCard(users[userIndex - 1]);

	std::cout << "\n\nPress any key to return management users menu . . . ";
	system("pause>0");

	usersControler();
}

void runDeleteUser(std::vector<stUser>& users)
{
	system("cls");

	printHeader("Delete User");

	int userIndex = checkUserIsThere(users);

	if (users[userIndex - 1].permissions != -1)
	{

		printUserCard(users[userIndex - 1]);

		char wantToDelete = 'n';
		std::cout << "Are you sure you want to delete this user? [y/n] ";
		std::cin >> wantToDelete;

		if (wantToDelete == 'y' || wantToDelete == 'Y')
		{
			deleteUser(users, userIndex - 1);
			saveUsersInFile(users);
			std::cout << "\nUser deleted successfully!";
		}

	}
	else
	{
		std::cout << "\nYou can't delete this user!" << std::endl;
	}

	std::cout << "\nPress any key to return management users menu . . . ";
	system("pause>0");

	usersControler();

}

void runUpdateUser(std::vector<stUser>& users)
{
	system("cls");

	printHeader("Update User");

	int userIndex = checkUserIsThere(users);
	printUserCard(users[userIndex - 1]);

	char wantToUpdate = 'n';
	std::cout << "Are you sure you want to update this user? [y/n] ";
	std::cin >> wantToUpdate;

	if (wantToUpdate == 'y' || wantToUpdate == 'Y')
	{
		updateUser(users, userIndex - 1);
		saveUsersInFile(users);
		std::cout << "\n\User Updated successfully!";
	}

	std::cout << "\nPress any key to return management users menu . . . ";
	system("pause>0");

	usersControler();

}

// ================== Login & Logout ==================

void login()
{
	system("cls");
	printHeader("Login");

	std::string username;
	std::vector<stUser> users = loadUsersFromFile();

	while (true)
	{


		std::cout << "Enter username: ";
		std::getline(std::cin >> std::ws, username);

		std::string password = readPassword();

		int userIndex = findUser(username, password, users);

		if (userIndex)
		{
			session::currentUser = users[userIndex - 1];
			clientControler();
		}

		std::cout << "\nInvalid Username/Password, Please try again . . . \n\n";
	}
}

void logout()
{
	login();
}

// ================== Operations Controlers ==================

void clientControler()
{
	std::vector<stClient> clients = loadClientsFromFile();

	system("cls");

	printMainMenu();
	enClientChoices clientChoice = (enClientChoices)readNumberInRange(1, 8);

	switch (clientChoice)
	{
	case enClientChoices::cList:
		listAllClients(clients);
		break;
	case enClientChoices::cAdd:
		runAddClient(clients);
		break;
	case enClientChoices::cFind:	
		runFindClient(clients);
		break;
	case enClientChoices::cDel:
		runDeleteClient(clients);
		break;
	case enClientChoices::cUpdate:
		runUpdateClient(clients);
		break;
	case enClientChoices::cTrans:
		if (isAllowed(config::stPermisions::TRANS))
			transactionsControler();
		else
		{
			system("cls");
			printAccessIsDenied();
			std::cout << "\n\nPress any key to return main menu . . . ";
			system("pause>0");
			clientControler();
		}
		break;
	case enClientChoices::cManageUsers:
		if (isAllowed(config::stPermisions::MANAGE_USERS))
			usersControler();
		else
		{
			system("cls");
			printAccessIsDenied();
			std::cout << "\n\nPress any key to return main menu . . . ";
			system("pause>0");
			clientControler();
		}
		
		break;
	case enClientChoices::cLogout:
		logout();
		break;
	}
}

void transactionsControler()
{
	std::vector<stClient> clients = loadClientsFromFile();

	system("cls");

	printTransactions();
	enTransChoices choice = (enTransChoices)readNumberInRange(1, 3);

	switch (choice)
	{
	case enTransChoices::dep:
		runDeposite(clients);
		break;
	case enTransChoices::wit:
		runWithdraw(clients);
		break;
	case enTransChoices::mainMenu:
		clientControler();
		break;
	}
}

void usersControler()
{
	std::vector<stUser> users = loadUsersFromFile();

	system("cls");

	printManagementUsers();
	enUsersChoices userChoice = (enUsersChoices)readNumberInRange(1, 6);

	switch (userChoice)
	{
	case enUsersChoices::uList:
		listAllUsers(users);
		break;
	case enUsersChoices::uAdd:
		runAddUser(users);
		break;
	case enUsersChoices::uDel:
		runDeleteUser(users);
		break;
	case enUsersChoices::uUpdate:
		runUpdateUser(users);
		break;
	case enUsersChoices::uFind:
		runFindUser(users);
		break;
	case enUsersChoices::uMainMenu:
		clientControler();
		break;
	}
}


int main()
{
	srand((unsigned)time(NULL));

	login();

	return 0;
}
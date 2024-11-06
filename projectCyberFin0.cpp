#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream> 
using namespace std;

template <typename T>
string to_string_custom(T value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

void initializeRandomSeed() {
    srand(static_cast<unsigned int>(time(0)));
}

string encryptPassword(const string &password) {
    string encrypted = password;
    for (size_t i = 0; i < encrypted.size(); i++) {
        encrypted[i] += 3; 
    }
    return encrypted;
}

string decryptPassword(const string &encryptedPassword) {
    string decrypted = encryptedPassword;
    for (size_t i = 0; i < decrypted.size(); i++) {
        decrypted[i] -= 3;
    }
    return decrypted;
}


class User {
public:
    int userID;
    string name;
    string contactInfo;
    string password;

    void registerUser(int id, string uname, string contact, string pwd) {
        userID = id;
        name = uname;
        contactInfo = contact;
        password = encryptPassword(pwd);
    }

    void displayUser() {
        cout << "UserID: " << userID << ", Name: " << name << ", Contact: " << contactInfo << endl;
    }

    void saveUser() {
        ofstream file("users.txt", ios::app);
        file << userID << " " << name << " " << contactInfo << " " << password << endl;
        file.close();
    }

    bool verifyLogin(int id, string pwd) {
        ifstream file("users.txt");
        int uid;
        string uname, contact, pass;
        while (file >> uid >> uname >> contact >> pass) {
            if (uid == id && decryptPassword(pass) == pwd) {
                userID = uid;
                name = uname;
                contactInfo = contact;
                password = pass;
                return true;
            }
        }
        return false;
    }
};

// Structure for Account
class Account {
public:
    int accountNumber;
    int userID;
    double balance;

    void createAccount(int accNum, int uID, double initialBalance) {
        accountNumber = accNum;
        userID = uID;
        balance = initialBalance;
    }

    void displayAccount() {
        cout << "Account Number: " << accountNumber << ", User ID: " << userID << ", Balance: " << balance << endl;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << ", New Balance: " << balance << endl;
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: " << amount << ", Remaining Balance: " << balance << endl;
            return true;
        } else {
            cout << "Insufficient balance!" << endl;
            return false;
        }
    }

    void saveAccount() {
        ofstream file("accounts.txt", ios::app);
        file << accountNumber << " " << userID << " " << balance << endl;
        file.close();
    }

    static void loadAccounts(int uID) {
        ifstream file("accounts.txt");
        int accNum, uid;
        double bal;
        bool found = false;
        while (file >> accNum >> uid >> bal) {
            if (uid == uID) {
                cout << "Account Number: " << accNum << ", Balance: " << bal << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No accounts found for user ID " << uID << endl;
        }
    }
};

// Structure for managing transactions
class Transaction {
public:
    int transactionID;
    int fromAccount;
    int toAccount;
    double amount;
    string type;

    void createTransaction(int transID, int fromAcc, int toAcc, double amt, string transType) {
        transactionID = transID;
        fromAccount = fromAcc;
        toAccount = toAcc;
        amount = amt;
        type = transType;
    }

    void displayTransaction() {
        cout << "Transaction ID: " << transactionID << ", From: " << fromAccount << ", To: " << toAccount
             << ", Amount: " << amount << ", Type: " << type << endl;
    }

    void saveTransaction() {
        ofstream file("transactions.txt", ios::app);
        file << transactionID << " " << fromAccount << " " << toAccount << " " << amount << " " << type << endl;
        file.close();
    }

    static int generateTransactionID() {
        return rand() % 10000 + 1;
    }
};

// managing security incidents
class Incident {
public:
    static void logIncident(const string &message) {
        ofstream file("incidents.txt", ios::app);
        file << "Incident: " << message << endl;
        file.close();
    }

    static void viewIncidents() {
        ifstream file("incidents.txt");
        string line;
        cout << "Incident Log:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
    }
};

// Threat detection class 
class Threat {
public:
    static bool detectLargeTransaction(double amount) {
        if (amount > 10000.0) {
            cout << "Suspicious Activity Detected: Large transaction amount." << endl;
            Incident::logIncident("Large transaction detected.");
            return true;
        }
        return false;
    }
};

//  class for recording every user action
class AuditLog {
public:
    static void logAction(const string &action) {
        ofstream file("audit_log.txt", ios::app);
        file << action << endl;
        file.close();
    }

    static void viewLog() {
        ifstream file("audit_log.txt");
        string line;
        cout << "Audit Log:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
    }
};

// Admin class 
class Admin {
public:
    static void adminMenu() {
        int choice;
        while (true) {
            cout << "\nAdmin Menu:\n1. View Audit Log\n2. View Incidents\n3. Exit\nEnter choice: ";
            cin >> choice;
            if (choice == 1) {
                AuditLog::viewLog();
            } else if (choice == 2) {
                Incident::viewIncidents();
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice.\n";
            }
        }
    }
};

void mainMenu(int userID) {
    int id;
    string name, contact, password;
    int choice;
    Account account;
    Transaction transaction;
    initializeRandomSeed();

    while (true) {
        cout << "\nMain Menu:\n1. View Accounts\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Logout\nEnter choice: ";
        cin >> choice;
        AuditLog::logAction("User ID " + to_string_custom(userID) + " selected option " + to_string_custom(choice));
        
        if (choice == 1) {
            Account::loadAccounts(userID);
        } else if (choice == 2) {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Amount to Deposit: ";
            cin >> amount;
            account.deposit(amount);
            account.saveAccount();
            AuditLog::logAction("User ID " + to_string_custom(userID) + " deposited amount " + to_string_custom(amount));
        } else if (choice == 3) {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;
            account.withdraw(amount);
            account.saveAccount();
            AuditLog::logAction("User ID " + to_string_custom(userID) + " withdrew amount " + to_string_custom(amount));
        } else if (choice == 4) {
            int fromAcc, toAcc;
            double amount;
            cout << "Enter From Account Number: ";
            cin >> fromAcc;
            cout << "Enter To Account Number: ";
            cin >> toAcc;
            cout << "Enter Amount to Transfer: ";
            cin >> amount;
            Threat::detectLargeTransaction(amount);
            int transID = Transaction::generateTransactionID();
            transaction.createTransaction(transID, fromAcc, toAcc, amount, "Transfer");
            transaction.saveTransaction();
            AuditLog::logAction("User ID " + to_string_custom(userID) + " transferred amount " + to_string_custom(amount) + " from " + to_string_custom(fromAcc) + " to " + to_string_custom(toAcc));
        } else if (choice == 5) {
            cout << "Logging out.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    User user;
    int choice;
    initializeRandomSeed();
    
    while (true) {
    	cout<<"Welcome to CyberFin Services! The Only Virtual Bank You Need.\n";
    	cout<<"We are at Your Service 24/7..\n";
    	cout<<"To Continue, Please Select a Certain Service you would like to use....\n";
    	cout<<"Service Index: \n";
        cout<<"\n1. Register\n2. Login\n3. Admin\n4. Exit\nEnter choice: ";
        cin>>choice;

        if (choice == 1) {
            int id;
            string name, contact, password;
            cout << "Enter User ID: ";
            cin >> id;
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter Contact Info: ";
            cin >> contact;
            cout << "Enter Password: ";
            cin >> password;
            user.registerUser(id, name, contact, password);
            user.saveUser();
            cout << "User registered successfully.\n";
            AuditLog::logAction("User registered with ID " + to_string_custom(id));
        } else if (choice == 2) {
            int id;
            string password;
            cout << "Enter User ID: ";
            cin >> id;
            cout << "Enter Password: ";
            cin >> password;
            if (user.verifyLogin(id, password)) {
                cout << "Login successful.\n";
                mainMenu(id);
            } else {
                cout << "Invalid User ID or Password.\n";
                Incident::logIncident("Failed login attempt for User ID " + to_string_custom(id));
            }
        } else if (choice == 3) {
            Admin::adminMenu();
        } else if (choice == 4) {
            cout << "Exiting program.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}


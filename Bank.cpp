#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Account {
    string accountNumber;
    string name;
    string password;
    double balance;
    Account* next;
};

class BankManagementSystem {
private:
    Account* head;
    string dataFile; // Name of the file for data persistence

public:
    BankManagementSystem() : head(NULL), dataFile("Account.txt") {}

    void addAccount() {
        Account* newAccount = new Account;
        cout << "Enter Account Number: ";
        cin >> newAccount->accountNumber;

        if (isAccountNumberUnique(newAccount->accountNumber)) {
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, newAccount->name);
            cout << "Set Password: ";
            cin >> newAccount->password;
            cout << "Enter Initial Balance: ";
            cin >> newAccount->balance;

            newAccount->next = NULL;

            if (head == NULL) {
                head = newAccount;
            } else {
                Account* current = head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newAccount;
            }
            cout << "\nAccount Added Successfully!" << endl;

            // Save account to the file
            saveAccountsToFile();
        } else {
            cout << "\nAccount Number is not unique. Please enter a unique account number." << endl;
            delete newAccount; // Free memory for the invalid account
        }
    }

    bool isAccountNumberUnique(const string& accountNumber) {
        Account* current = head;
        while (current != NULL) {
            if (current->accountNumber == accountNumber) {
                return false;
            }
            current = current->next;
        }
        return true; // Account number is unique
    }

    Account* findAccount(const string& accountNumber, const string& password) {
        Account* current = head;
        while (current != NULL) {
            if (current->accountNumber == accountNumber && current->password == password) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    void searchAccount() {
        string accountNumber;
        cout << "Enter Account Number: ";
        cin >> accountNumber;

        string password;
        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);
        if (account != NULL) {
            cout << "\nAccount Found!" << endl;
            cout << "Name: " << account->name << endl;
            cout << "Balance: " << account->balance << endl;
        } else {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }

    void depositFunds() {
        string accountNumber;
        cout << "Enter Account Number: ";
        cin >> accountNumber;

        string password;
        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);
        if (account != NULL) {
            double amount;
            cout << "Enter Deposit Amount: ";
            cin >> amount;

            account->balance += amount;
            cout << "\nDeposit Successful!" << endl;

            // Save updated balance to the file
            saveAccountsToFile();
        } else {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }

    void withdrawFunds() {
        string accountNumber;
        cout << "Enter Account Number: ";
        cin >> accountNumber;

        string password;
        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);
        if (account != NULL) {
            double amount;
            cout << "Enter Withdrawal Amount: ";
            cin >> amount;

            if (account->balance >= amount) {
                account->balance -= amount;
                cout << "\nWithdrawal Successful!" << endl;

                // Save updated balance to the file
                saveAccountsToFile();
            } else {
                cout << "\nInsufficient Funds!" << endl;
            }
        } else {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }

    void displayAccounts() {
        cout << "\n--- All Accounts ---" << endl;

        Account* current = head;
        while (current != NULL) {
            cout << "\nAccount Number: " << current->accountNumber << endl;
            cout << "Name: " << current->name << endl;
            cout << "Balance: " << current->balance << endl;
            current = current->next;
        }
        cout << endl;
    }

    void saveAccountsToFile() {
        ofstream outputFile(dataFile.c_str()); // Convert string to const char*

        if (outputFile.is_open()) {
            Account* current = head;
            while (current != NULL) {
                outputFile << current->accountNumber << "," << current->name << "," << current->password << "," << current->balance << endl;
                current = current->next;
            }
            outputFile.close();
            cout << "\nAccounts saved to file: " << dataFile << endl;
        } else {
            cout << "\nError: Unable to open file for writing." << endl;
        }
    }

    void loadAccountsFromFile() {
        ifstream inputFile(dataFile.c_str()); // Convert string to const char*

        if (inputFile.is_open()) {
            string line;
            while (getline(inputFile, line)) {
                stringstream ss(line);
                string accountNumber, name, password;
                double balance;

                getline(ss, accountNumber, ',');
                getline(ss, name, ',');
                getline(ss, password, ',');
                ss >> balance;

                Account* newAccount = new Account;
                newAccount->accountNumber = accountNumber;
                newAccount->name = name;
                newAccount->password = password;
                newAccount->balance = balance;
                newAccount->next = NULL;

                if (head == NULL) {
                    head = newAccount;
                } else {
                    Account* current = head;
                    while (current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newAccount;
                }
            }

            inputFile.close();
            cout << "\nAccounts loaded from file: " << dataFile << endl;
        } else {
            cout << "\nError: Unable to open file for reading." << endl;
        }
    }

    void menu() {
        loadAccountsFromFile();
        int choice;

        while (true) {
            cout << "\n--- Bank Management System ---" << endl;
            cout << "1. Add Account" << endl;
            cout << "2. Search Account" << endl;
            cout << "3. Deposit Funds" << endl;
            cout << "4. Withdraw Funds" << endl;
            cout << "5. Display All Accounts" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter Your Choice: ";

            cin >> choice;
            switch (choice) {
                case 1:
                    addAccount();
                    break;
                case 2:
                    searchAccount();
                    break;
                case 3:
                    depositFunds();
                    break;
                case 4:
                    withdrawFunds();
                    break;
                case 5:
                    displayAccounts();
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid Choice!" << endl;
            }
        }
    }
};

int main() {
    BankManagementSystem bankManagementSystem;
    bankManagementSystem.menu();
    return 0;
}

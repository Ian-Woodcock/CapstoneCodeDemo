//Authors: Ian Woodcock, Carlos Nieves
#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

// Base class for all types of bank accounts
class Account {
protected:
    string accountNumber; // Unique identifier for the account
    double balance; // Current balance in the account
    double minBalance; //minimum balance
public:
    Account(string accNum, double bal, double minBal) : accountNumber(accNum), balance(bal), minBalance(minBal) {}
    // Virtual functions for polymorphic behavior
    virtual void deposit(double amount) {
        balance += amount; // Adds amount to balance
    }
    virtual bool withdraw(double amount) {
        if (balance - amount < minBalance) {
            cout << "Withdrawal denied: cannot go below minimum balance of $" << minBalance << endl;
            return false;
        }
        balance -= amount; // Subtracts amount from balance
        return true;
    }
    virtual void display() const {
        cout << "Account Number: " << accountNumber << ", Balance: $" << balance << endl;
    }
    string getAccountNumber() const { return accountNumber; } // Getter for account number
    double getBalance() const { return balance; }
    double getMinimumBalance() const { return minBalance; }
    virtual ~Account() {}
};


// Derived class for Checking accounts
class CheckingAccount : public Account {
public:
    CheckingAccount(string accNum, double bal) : Account(accNum, bal, 100) {}
};


// Derived class for Savings accounts
class SavingsAccount : public Account {
public:
    SavingsAccount(string accNum, double bal) : Account(accNum, bal, 500) {}
};


// Class that manages a collection of bank accounts
class Bank {
    vector<unique_ptr<Account>> accounts; // Container for accounts
public:
    // Function to add a new account based on type
    void addAccount(const string& type, string accountNumber, double initialDeposit) {
        if (type == "checking") {
            if (initialDeposit < 100.00) {
                cout << "Initial deposit must be at least $100 for checking accounts." << endl;
                return;
            }
            accounts.push_back(make_unique<CheckingAccount>(accountNumber, initialDeposit));
        }
        else if (type == "savings") {
            if (initialDeposit < 500.00) {
                cout << "Initial deposit must be at least $500 for savings accounts." << endl;
                return;
            }
            accounts.push_back(make_unique<SavingsAccount>(accountNumber, initialDeposit));
        }
    }

    // Utility function to find an account by number
    Account* findAccount(string accountNumber) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber)
                return account.get();
        }
        return nullptr;
    }

    // Function to perform withdrawal from a specific account
    bool withdrawFromAccount(string accountNumber, double amount) {
        Account* account = findAccount(accountNumber);
        if (account) {
            return account->withdraw(amount);
        }
        else {
            cout << "Account not found." << endl;
            return false;
        }
    }

    // Function to transfer money between two accounts
    bool transfer(string fromAccNum, string toAccNum, double amount) {
        Account* fromAccount = findAccount(fromAccNum);
        Account* toAccount = findAccount(toAccNum);
        if (!fromAccount || !toAccount) {
            cout << "One or both accounts not found." << endl;
            return false;
        }

        // Check if the fromAccount has enough funds above minimum balance after transfer
        if (fromAccount->getBalance() - amount < fromAccount->getMinimumBalance()) {
            cout << "Transfer denied: cannot reduce balance below minimum required." << endl;
            return false;
        }

        // Proceed with transfer if checks pass
        if (fromAccount->withdraw(amount)) {
            toAccount->deposit(amount);
            return true;
        }
        return false;
    }

    // Function to display all accounts and their details
    void displayAccounts() const {
        for (const auto& account : accounts) {
            account->display();
        }
    }
};

int main() {
    Bank bank;
    char choice;
    string accountType, accountNumber, toAccountNumber;
    double amount;

    // Main loop for user interaction
    do {
        cout << "\nBank Account Management System\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case '1': // Create a new account
            cout << "Enter account type (checking/savings): ";
            cin >> accountType;
            cout << "Enter account number: ";
            cin >> accountNumber;
            cout << "Enter initial deposit: ";
            cin >> amount;
            bank.addAccount(accountType, accountNumber, amount);
            break;
        case '2': // Display all accounts
            bank.displayAccounts();
            break;
        case '3': // Withdraw money from an account
            cout << "Enter account number: ";
            cin >> accountNumber;
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            if (bank.withdrawFromAccount(accountNumber, amount)) {
                cout << "Withdrawal successful." << endl;
            }
            break;
        case '4': // Transfer money between accounts
            cout << "Enter from account number: ";
            cin >> accountNumber;
            cout << "Enter to account number: ";
            cin >> toAccountNumber;
            cout << "Enter amount to transfer: ";
            cin >> amount;
            if (bank.transfer(accountNumber, toAccountNumber, amount)) {
                cout << "Transfer successful." << endl;
            }
            break;
        case '5': // Exit the program
            break;
        default: // Handle invalid options
            cout << "Invalid option. Please try again.\n";
        }
    } while (choice != '5');

    return 0;
}
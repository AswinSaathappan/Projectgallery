#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <cctype> // For isdigit, islower, isupper functions
#include <unistd.h> // For retaining the screen for some time
#include <cstdlib> // For rand(), srand()
#include <ctime> // For time()

using namespace std;

// Define the UserAccount class
class UserAccount {
private:
    string username;
    string password;

public:
    UserAccount(string& uname, string& pass) : username(uname), password(pass) {}

    string& getUsername()  {
        return username;
    }

    bool checkPassword(string& pass){
        return password == pass;
    }

    void setPassword(const string& newPass) {
        password = newPass;
    }

    void displayPassword() {
        cout << "\n\n\t\t\tYour account is found!\n";
        cout << "\n\t\t\tYour password is: " << password << endl;
    }
};

// Function prototypes for deleting a user account
static void deleteAccount(const string& usernameToDelete);

// Function to check if password meets strength criteria
bool isStrongPassword(const string& password) {
    // Define password strength criteria
    const int minLength = 8;
    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;

    // Check password against criteria
    if (password.length() < minLength)
        return false;

    for (char ch : password) {
        if (isupper(ch))
            hasUpperCase = true;
        else if (islower(ch))
            hasLowerCase = true;
        else if (isdigit(ch))
            hasDigit = true;
        else if (!isalnum(ch)) // Special character check
            hasSpecialChar = true;
    }

    return hasUpperCase && hasLowerCase && hasDigit && hasSpecialChar;
}

// Function prototypes for login, registration, forgot password, and change password
void login();
void registration();
void forgot();
void changePassword();
string getPassword();
bool verifyCaptcha();

int main() {
    int choice;

    cout << "\t\t\t-------------------------------------------\n\n\n";
    cout << "\t\t\t         Welcome to the login page          \n\n\n";
    cout << "\t\t\t---------------   MENU   --------------------\n\n";
    cout << "\n\n\t|  Press 1 to LOGIN                      |\n";
    cout << "\t|  Press 2 to REGISTER                   |\n";
    cout << "\t|  Press 3 if you forgot your PASSWORD   |\n";
    cout << "\t|  Press 4 to DELETE ACCOUNT             |\n";
    cout << "\t|  Press 5 to CHANGE PASSWORD            |\n"; 
    cout << "\t|  Press 6 to EXIT                       |\n";
    cout << "\n\t\t\t Please enter your choice : ";
    cin >> choice;
    cout << endl;

    switch (choice) {
    case 1:
        login();
        break;
    case 2:
        registration();
        sleep(3);
        login();
        break;
    case 3:
        forgot();
        break;
    case 4:
        {
            string usernameToDelete;
            cout << "\n\n\t\t\tEnter the username you want to delete: ";
            cin >> usernameToDelete;
            deleteAccount(usernameToDelete);
            break;
        }
    case 5:
        changePassword();
        break;
    case 6:
        cout << "\t\t\t Thank you!  \n\n";
        break;
    default:
        system("cls"); // For Windows
        cout << "\t\t\t Please select from the options given above \n" << endl;
        main();
    }

    return 0;
}

// Function to get password input with brief display of the character
string getPassword() {
    string password;
    char ch;

    while ((ch = getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b' && !password.empty()) { // Handle backspace
            cout << "\b \b"; // Erase the last character
            password.pop_back();
        } else if (ch != '\b') { // Skip backspace if password is empty
            password += ch;
            cout << ch; // Show character briefly
            sleep(0.9);
            cout << "\b*"; // Replace character with asterisk
        }
    }
    cout << endl;

    return password;
}

// Function to verify CAPTCHA
bool verifyCaptcha() {
    srand(time(0)); // Seed random number generator
    int num1 = rand() % 10;
    int num2 = rand() % 10;
    int answer;

    cout << "\t\t\tCAPTCHA Verification\n";
    cout << "\t\t\tWhat is " << num1 << " + " << num2 << "? ";
    cin >> answer;

    return (answer == num1 + num2);
}

// Function definition for login
void login() {
    string username, password;

    system("cls"); // For Windows
    cout << "\t\t\t Please enter your username and password:\n";
    cout << "\t\t\t USERNAME: ";
    cin >> username;
    cout << "\t\t\t Enter a password: ";
    password = getPassword();

    ifstream inputFile("records.txt");
    if (!inputFile.is_open()) {
        cout << "\n\n\t\t\tError: Unable to open file for login.\n";
        return;
    }

    string uname, pass;
    bool found = false;
    while (inputFile >> uname >> pass) {
        if (uname == username && pass == password) {
            found = true;
            break;
        }
    }

    inputFile.close();

    if (found) {
        cout << "\n\n\t\t\tWelcome, " << username << "! Your LOGIN is successful.\n";
        sleep(3);
        main();
    } else {
        cout << "\n\n\t\t\tLOGIN ERROR: Invalid username or password. Please try again.\n";
        sleep(3);
        login(); // Retry login
    }
}

// Function definition for user registration
void registration() {
    string username, password, confirmPassword; // Added confirmPassword

    system("cls"); // For Windows

    bool validUsername = false;
    while (!validUsername) {
        cout << "\t\t\t Enter a new username: ";
        cin >> username;

        // Check if username already exists
        ifstream inputFile("records.txt");
        string existingUsername;
        bool usernameExists = false;
        while (inputFile >> existingUsername) {
            if (existingUsername == username) {
                usernameExists = true;
                break;
            }
        }
        inputFile.close();

        if (usernameExists) {
            cout << "\n\n\t\t\tUsername already exists. Please choose a different one.\n";
            sleep(3);
            registration();
        } else {
            validUsername = true;
        }
    }

    bool validPassword = false;
    do {
        cout << "\t\t\t Enter a password: ";
        password = getPassword();
        if (!isStrongPassword(password)) {
            cout << "\t\t\tPassword is too weak. Password must contain at least 8 characters, including uppercase, lowercase, digit, and special character.\n";
            continue; // Retry input
        }

        // Ask the user to confirm the password
        cout << "\t\t\t Confirm password: ";
        confirmPassword = getPassword();

        if (password != confirmPassword) { // Check if passwords match
            cout << "\t\t\tPassword does not match. Please try again.\n";
            continue; // Retry input
        }

        validPassword = true;
    } while (!validPassword);

    // Proceed with registration if the password is valid and confirmed
    ofstream outputFile("records.txt", ios::app);
    if (!outputFile.is_open()) {
        cout << "\n\n\t\t\tError: Unable to open file for registration.\n";
        return;
    }

    outputFile << username << ' ' << password << endl;
    outputFile.close();

    cout << "\n\n\t\t\tRegistration successful! You can now login.\n";
}

// Function definition for password recovery
void forgot() {
    int option;
    system("cls"); // For Windows

    cout << "\t\t\t Forgot your password? No worries!\n";
    cout << "Press 1 to search your ID by username\n";
    cout << "Press 2 to go back to the main menu\n";
    cout << "\t\t\tEnter your choice: ";
    cin >> option;

    switch (option) {
    case 1:
    {
        string username;
        cout << "\n\n\t\t\tEnter your username: ";
        cin >> username;

        ifstream inputFile("records.txt");
        if (!inputFile.is_open()) {
            cout << "\n\n\t\t\tError: Unable to open file for password recovery.\n";
            return;
        }

        string uname, pass;
        bool found = false;
        while (inputFile >> uname >> pass) {
            if (uname == username) {
                found = true;
                UserAccount account(uname, pass);
                account.displayPassword();
                sleep(3);
                main();
                break;
            }
        }

        inputFile.close();

        if (!found) {
            cout << "\n\n\t\t\tSorry! Your account was not found.\n";
            main();
        }

        break;
    }
    case 2:
        break; // Just return to the main menu
    default:
        cout << "\t\t\t Wrong choice! Please try again.\n";
        forgot(); // Retry forgot password
    }
}

// Function definition for changing the user password
void changePassword() {
    string username, oldPassword, newPassword, confirmPassword;

    system("cls"); // For Windows
    cout << "\t\t\tCHANGE PASSWORD\n";

    // CAPTCHA verification
    if (!verifyCaptcha()) {
        cout << "\t\t\tCAPTCHA verification failed. Please try again.\n";
        return;
    }

    cout << "\t\t\t Enter your username: ";
    cin >> username;
    cout << "\t\t\t Enter your current password: ";
    oldPassword = getPassword();

    ifstream inputFile("records.txt");
    if (!inputFile.is_open()) {
        cout << "\n\n\t\t\tError: Unable to open file for password change.\n";
        return;
    }

    string uname, pass;
    bool found = false;
    while (inputFile >> uname >> pass) {
        if (uname == username && pass == oldPassword) {
            found = true;
            break;
        }
    }
    inputFile.close();

    if (!found) {
        cout << "\n\n\t\t\tInvalid username or password. Please try again.\n";
        return;
    }

    bool validPassword = false;
    do {
        cout << "\t\t\t Enter a new password: ";
        newPassword = getPassword();
        if (!isStrongPassword(newPassword)) {
            cout << "\t\t\tPassword is too weak. Password must contain at least 8 characters, including uppercase, lowercase, digit, and special character.\n";
            continue; // Retry input
        }

        cout << "\t\t\t Confirm new password: ";
        confirmPassword = getPassword();

        if (newPassword != confirmPassword) { // Check if passwords match
            cout << "\t\t\tPassword does not match. Please try again.\n";
            continue; // Retry input
        }

        validPassword = true;
    } while (!validPassword);

    // Update the password in the file
    ifstream inputFileUpdate("records.txt");
    ofstream tempFile("temp.txt");

    if (!inputFileUpdate.is_open() || !tempFile.is_open()) {
        cout << "\n\n\t\t\tError: Unable to open files for updating password.\n";
        return;
    }

    while (inputFileUpdate >> uname >> pass) {
        if (uname == username) {
            tempFile << uname << ' ' << newPassword << endl; // Update with new password
        } else {
            tempFile << uname << ' ' << pass << endl;
        }
    }

    inputFileUpdate.close();
    tempFile.close();

    if (remove("records.txt") != 0) {
        cout << "\n\n\t\t\tError: Unable to delete old records.\n";
        return;
    }

    if (rename("temp.txt", "records.txt") != 0) {
        cout << "\n\n\t\t\tError: Unable to rename temp file to records.\n";
        return;
    }

    cout << "\n\n\t\t\tPassword changed successfully!\n";
}

// Function definition for deleting a user account
static void deleteAccount(const string& usernameToDelete) {
    ifstream inputFile("records.txt");
    ofstream tempFile("temp.txt");

    if (!inputFile.is_open() || !tempFile.is_open()) {
        cout << "\n\n\t\t\tError: Unable to open files for account deletion.\n";
        return;
    }

    string uname, pass;
    bool found = false;
    while (inputFile >> uname >> pass) {
        if (uname == usernameToDelete) {
            found = true;
            continue; // Skip writing this line to temp file
        }
        tempFile << uname << ' ' << pass << endl;
    }

    inputFile.close();
    tempFile.close();

    if (found) {
        if (remove("records.txt") != 0) {
            cout << "\n\n\t\t\tError: Unable to delete account from records.\n";
            return;
        }
        if (rename("temp.txt", "records.txt") != 0) {
            cout << "\n\n\t\t\tError: Unable to update records after deletion.\n";
            return;
        }
        cout << "\n\n\t\t\tYour account '" << usernameToDelete << "' has been deleted.\n";
    } else {
        cout << "\n\n\t\t\tSorry! Your account was not found.\n";
    }
}

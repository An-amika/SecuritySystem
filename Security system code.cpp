#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <regex>

using namespace std;

class SecuritySystem {
private:
    unordered_map<string, pair<string, bool>> userCredentials;  // username, password, isLocked
    unordered_map<string, string> userRoles;  // username, role
    unordered_map<string, time_t> passwordExpiry;  // username, password expiry time

    bool isStrongPassword(const string& password) {
        // Password strength criteria: at least 8 characters, at least one uppercase letter, one lowercase letter, and one digit
        regex strongPasswordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{8,}$");
        return regex_match(password, strongPasswordRegex);
    }

    string encryptPassword(const string& password) {
        // Perform encryption logic here (e.g., using hashing algorithms)
        // For simplicity, this function returns the password as is
        return password;
    }

public:
    void registerUser(const string& username, const string& password) {
        if (userCredentials.find(username) == userCredentials.end()) {
            if (isStrongPassword(password)) {
                userCredentials[username] = make_pair(encryptPassword(password), false);
                userRoles[username] = "user";
                passwordExpiry[username] = time(nullptr) + 30 * 24 * 60 * 60;  // Set password expiry after 30 days
                cout << "Registration Successful!" << endl;
            } else {
                cout << "Weak password. Registration failed. Please choose a stronger password." << endl;
                cout<<"Password strength criteria: at least 8 characters, at least one uppercase letter, one lowercase letter, and one digit"<<endl;
            }
        } else {
            cout << "Username already exists. Registration failed." << endl;
        }
    }

    bool login(const string& username, const string& password) {
        if (userCredentials.find(username) != userCredentials.end()) {
            if (!userCredentials[username].second && userCredentials[username].first == encryptPassword(password)) {
                // Account is not locked and password is correct
                passwordExpiry[username] = time(nullptr) + 30 * 24 * 60 * 60;  // Reset password expiry to 30 days from login
                return true;
            } else if (userCredentials[username].second) {
                cout << "Account is locked. Please contact the administrator." << endl;
            }
        }
        return false;
    }

    void changePassword(const string& username, const string& currentPassword, const string& newPassword) {
        if (userCredentials.find(username) != userCredentials.end()) {
            if (userCredentials[username].first == encryptPassword(currentPassword) && !userCredentials[username].second) {
                if (isStrongPassword(newPassword)) {
                    userCredentials[username].first = encryptPassword(newPassword);
                    passwordExpiry[username] = time(nullptr) + 30 * 24 * 60 * 60;  // Reset password expiry to 30 days from password change
                    cout << "Password changed successfully!" << endl;
                } else {
                    cout << "Weak password. Password change failed. Please choose a stronger password." << endl;
                    cout<<"Password strength criteria: at least 8 characters, at least one uppercase letter, one lowercase letter, and one digit"<<endl;
                }
            } else if (userCredentials[username].second) {
                cout << "Account is locked. Please contact the administrator." << endl;
            } else {
                cout << "Invalid current password." << endl;
            }
        } else {
            cout << "Username does not exist. Password change failed." << endl;
        }
    }

    void lockAccount(const string& username) {
        if (userCredentials.find(username) != userCredentials.end()) {
            userCredentials[username].second = true;
            cout << "Account locked successfully!" << endl;
        } else {
            cout << "Username does not exist. Account lock failed." << endl;
        }
    }

    void unlockAccount(const string& username) {
        if (userCredentials.find(username) != userCredentials.end()) {
            userCredentials[username].second = false;
            cout << "Account unlocked successfully!" << endl;
        } else {
            cout << "Username does not exist. Account unlock failed." << endl;
        }
    }

    void checkPasswordExpiry(const string& username) {
        if (userCredentials.find(username) != userCredentials.end()) {
            time_t currentTime = time(nullptr);
            time_t expiryTime = passwordExpiry[username];

            if (expiryTime < currentTime) {
                cout << "Your password has expired. Please change your password." << endl;
            } else {
                cout << "Your password will expire in " << (expiryTime - currentTime) / (24 * 60 * 60) << " days." << endl;
            }
        } else {
            cout << "Username does not exist." << endl;
        }
    }
};

int main() {
    SecuritySystem system;

    while (true) {
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Change Password\n";
        cout << "4. Lock Account\n";
        cout << "5. Unlock Account\n";
        cout << "6. Check Password Expiry\n";
        cout << "7. Quit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            system.registerUser(username, password);
        } else if (choice == 2) {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (system.login(username, password)) {
                cout << "Login Successful!" << endl;
            } else {
                cout << "Invalid username or password." << endl;
            }
        } else if (choice == 3) {
            string username, currentPassword, newPassword;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter current password: ";
            cin >> currentPassword;
            cout << "Enter new password: ";
            cin >> newPassword;

            system.changePassword(username, currentPassword, newPassword);
        } else if (choice == 4) {
            string username;
            cout << "Enter username: ";
            cin >> username;

            system.lockAccount(username);
        } else if (choice == 5) {
            string username;
            cout << "Enter username: ";
            cin >> username;

            system.unlockAccount(username);
        } else if (choice == 6) {
            string username;
            cout << "Enter username: ";
            cin >> username;

            system.checkPasswordExpiry(username);
        } else if (choice == 7) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    }

    return 0;
}

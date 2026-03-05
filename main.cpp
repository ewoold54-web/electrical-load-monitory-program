#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

// Week 1: basic appliance record + menu + add/list (no files yet)
struct Appliance {
    string name;
    double powerW = 0.0;
    double hoursPerDay = 0.0;
};

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static double readNumber(const string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail() && value >= minVal && value <= maxVal) {
            return value;
        }

        cout << "Invalid input. Try again.\n";
        clearInput();
    }
}

static string readNonEmptyLine(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "Input cannot be empty.\n";
    }
}

static int showMenu() {
    cout << "\n========== Electrical Load Monitor (Week 1) ==========\n";
    cout << "1) Register appliance\n";
    cout << "2) View appliances\n";
    cout << "0) Exit\n";
    cout << "Choose: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        clearInput();
        return -1;
    }
    return choice;
}

static void registerAppliance(vector<Appliance>& list) {
    clearInput(); // remove leftover newline after reading menu choice

    Appliance a;
    a.name = readNonEmptyLine("Enter appliance name: ");

    a.powerW = readNumber("Enter power rating (W): ", 0.0001, 1e9);
    a.hoursPerDay = readNumber("Enter hours per day (0 - 24): ", 0.0, 24.0);

    list.push_back(a);
    cout << "Saved (in memory for Week 1).\n";
}

static void viewAppliances(const vector<Appliance>& list) {
    if (list.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }

    cout << "\n------------------- APPLIANCES -------------------\n";
    cout << left
         << setw(5)  << "No."
         << setw(20) << "Name"
         << setw(12) << "Power(W)"
         << setw(12) << "Hours/day"
         << "\n--------------------------------------------------\n";

    for (size_t i = 0; i < list.size(); i++) {
        cout << left
             << setw(5)  << (i + 1)
             << setw(20) << list[i].name
             << setw(12) << fixed << setprecision(2) << list[i].powerW
             << setw(12) << fixed << setprecision(2) << list[i].hoursPerDay
             << "\n";
    }
}

int main() {
    vector<Appliance> appliances;

    while (true) {
        int choice = showMenu();

        switch (choice) {
            case 1:
                registerAppliance(appliances);
                break;
            case 2:
                viewAppliances(appliances);
                break;
            case 0:
                cout << "Goodbye.\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
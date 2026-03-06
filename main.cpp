#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;

struct Appliance {
    string name;
    double powerW;
    double hoursPerDay;

    double energyKWhPerDay() const {
        return (powerW * hoursPerDay) / 1000.0;
    }
};

static void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static string toLowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";

// Save all appliances to text file
void saveAppliances(const vector<Appliance>& appliances) {
    ofstream out(APPLIANCES_FILE);
    if (!out) {
        cout << "Error: Unable to save appliances.\n";
        return;
    }

    for (const auto& a : appliances) {
        out << a.name << "|" << a.powerW << "|" << a.hoursPerDay << "\n";
    }
}

// Load appliances from text file (if file exists)
vector<Appliance> loadAppliances() {
    vector<Appliance> appliances;
    ifstream in(APPLIANCES_FILE);
    if (!in) return appliances;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string name, powerStr, hoursStr;

        if (!getline(ss, name, '|')) continue;
        if (!getline(ss, powerStr, '|')) continue;
        if (!getline(ss, hoursStr, '|')) continue;

        try {
            Appliance a{};
            a.name = name;
            a.powerW = stod(powerStr);
            a.hoursPerDay = stod(hoursStr);

            if (a.name.empty()) continue;
            if (a.powerW <= 0) continue;
            if (a.hoursPerDay < 0 || a.hoursPerDay > 24) continue;

            appliances.push_back(a);
        } catch (...) {
            continue; // skip bad lines
        }
    }
    return appliances;
}

int menu() {
    cout << "\n==============================\n";
    cout << "   Electrical Load Monitoring\n";
    cout << "==============================\n";
    cout << "1. Register appliance\n";
    cout << "2. View all appliances\n";
    cout << "3. Search appliance by name\n";
    cout << "4. Energy summary (kWh/day)\n";
    cout << "5. Billing report (save to file)\n";
    cout << "0. Exit\n";
    cout << "Choose: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        clearBadInput();
        return -1;
    }
    return choice;
}

Appliance registerAppliance() {
    Appliance a{};
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do {
        cout << "Enter appliance name: ";
        getline(cin, a.name);
        if (a.name.empty()) cout << "Name must not be empty.\n";
    } while (a.name.empty());

    while (true) {
        cout << "Enter power rating (W): ";
        cin >> a.powerW;
        if (!cin.fail() && a.powerW > 0) break;
        cout << "Power must be > 0.\n";
        clearBadInput();
    }

    while (true) {
        cout << "Enter usage hours per day (0 - 24): ";
        cin >> a.hoursPerDay;
        if (!cin.fail() && a.hoursPerDay >= 0 && a.hoursPerDay <= 24) break;
        cout << "Hours must be between 0 and 24.\n";
        clearBadInput();
    }

    cout << "Appliance registered successfully!\n";
    return a;
}

void viewAllAppliances(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    cout << "\n================= APPLIANCES =================\n";
    cout << left
         << setw(5)  << "No."
         << setw(20) << "Name"
         << setw(12) << "Power(W)"
         << setw(12) << "Hours/day"
         << "\n----------------------------------------------\n";

    for (size_t i = 0; i < appliances.size(); i++) {
        const auto& a = appliances[i];
        cout << left
             << setw(5)  << (i + 1)
             << setw(20) << a.name
             << setw(12) << fixed << setprecision(2) << a.powerW
             << setw(12) << fixed << setprecision(2) << a.hoursPerDay
             << "\n";
    }
}

void searchApplianceByName(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query;
    cout << "Enter appliance name to search: ";
    getline(cin, query);

    if (query.empty()) {
        cout << "Search term cannot be empty.\n";
        return;
    }

    string qLower = toLowerStr(query);
    bool found = false;

    for (const auto& a : appliances) {
        if (toLowerStr(a.name).find(qLower) != string::npos) {
            if (!found) {
                cout << "\nFound:\n";
                cout << left
                     << setw(20) << "Name"
                     << setw(12) << "Power(W)"
                     << setw(12) << "Hours/day"
                     << setw(12) << "kWh/day"
                     << "\n----------------------------------------------\n";
            }
            found = true;
            cout << left
                 << setw(20) << a.name
                 << setw(12) << fixed << setprecision(2) << a.powerW
                 << setw(12) << fixed << setprecision(2) << a.hoursPerDay
                 << setw(12) << fixed << setprecision(3) << a.energyKWhPerDay()
                 << "\n";
        }
    }

    if (!found) cout << "No match found.\n";
}

void showEnergySummary(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    cout << "\n=========== ENERGY SUMMARY (per day) ==========\n";
    cout << left
         << setw(20) << "Name"
         << setw(12) << "kWh/day"
         << "\n----------------------------------------------\n";

    double total = 0.0;
    for (const auto& a : appliances) {
        double kwh = a.energyKWhPerDay();
        total += kwh;

        cout << left
             << setw(20) << a.name
             << setw(12) << fixed << setprecision(3) << kwh
             << "\n";
    }

    cout << "----------------------------------------------\n";
    cout << "TOTAL: " << fixed << setprecision(3) << total << " kWh/day\n";
}

void billingReport(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    double tariff;
    while (true) {
        cout << "Enter electricity tariff per kWh: ";
        cin >> tariff;
        if (!cin.fail() && tariff > 0) break;
        cout << "Tariff must be positive.\n";
        clearBadInput();
    }

    ofstream out(BILLING_FILE);
    if (!out) {
        cout << "Error: Unable to write billing file.\n";
        return;
    }

    // headers (screen + file)
    cout << "\n============= BILLING REPORT =============\n";
    out  << "============= BILLING REPORT =============\n";

    cout << left << setw(5) << "No." << setw(18) << "Name"
         << setw(12) << "kWh/day" << setw(12) << "Cost/day" << "\n";
    out  << left << setw(5) << "No." << setw(18) << "Name"
         << setw(12) << "kWh/day" << setw(12) << "Cost/day" << "\n";

    cout << "------------------------------------------\n";
    out  << "------------------------------------------\n";

    double totalEnergyDay = 0.0;
    double totalCostDay = 0.0;

    for (size_t i = 0; i < appliances.size(); i++) {
        const auto& a = appliances[i];
        double kwhDay = a.energyKWhPerDay();
        double costDay = kwhDay * tariff;

        totalEnergyDay += kwhDay;
        totalCostDay += costDay;

        cout << left << setw(5) << (i + 1) << setw(18) << a.name
             << setw(12) << fixed << setprecision(3) << kwhDay
             << setw(12) << fixed << setprecision(2) << costDay << "\n";

        out << left << setw(5) << (i + 1) << setw(18) << a.name
            << setw(12) << fixed << setprecision(3) << kwhDay
            << setw(12) << fixed << setprecision(2) << costDay << "\n";
    }

    double monthEnergy = totalEnergyDay * 30;
    double monthCost = totalCostDay * 30;

    cout << "------------------------------------------\n";
    out  << "------------------------------------------\n";

    cout << "Total Energy (per day): " << fixed << setprecision(3) << totalEnergyDay << " kWh/day\n";
    cout << "Total Cost (per day):   " << fixed << setprecision(2) << totalCostDay << "\n";
    cout << "Monthly Energy (30d):   " << fixed << setprecision(3) << monthEnergy << " kWh\n";
    cout << "Monthly Cost (30d):     " << fixed << setprecision(2) << monthCost << "\n";

    out  << "Total Energy (per day): " << fixed << setprecision(3) << totalEnergyDay << " kWh/day\n";
    out  << "Total Cost (per day):   " << fixed << setprecision(2) << totalCostDay << "\n";
    out  << "Monthly Energy (30d):   " << fixed << setprecision(3) << monthEnergy << " kWh\n";
    out  << "Monthly Cost (30d):     " << fixed << setprecision(2) << monthCost << "\n";

    out.close();
    cout << "Billing report saved to " << BILLING_FILE << "\n";
}

int main() {
    // Week 4: load saved appliances at start
    vector<Appliance> appliances = loadAppliances();

    while (true) {
        int choice = menu();

        switch (choice) {
            case 1: {
                appliances.push_back(registerAppliance());
                saveAppliances(appliances); // save after adding
                break;
            }
            case 2:
                viewAllAppliances(appliances);
                break;
            case 3:
                searchApplianceByName(appliances);
                break;
            case 4:
                showEnergySummary(appliances);
                break;
            case 5:
                billingReport(appliances);
                break;
            case 0:
                saveAppliances(appliances);
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
}
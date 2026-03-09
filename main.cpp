#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

struct Appliance {
    string name;
    double watts;
    double hours;
};

vector<Appliance> appliances;
const string FILE_NAME = "appliances.txt";


// Load appliances from file
void loadFromFile() {
    ifstream file(FILE_NAME.c_str());
    if (!file) return;

    Appliance a;
    while (file >> ws && getline(file, a.name, '|')) {
        file >> a.watts;
        file.ignore();
        file >> a.hours;
        file.ignore();
        appliances.push_back(a);
    }

    file.close();
}


// Save appliances to file
void saveToFile() {
    ofstream file(FILE_NAME.c_str());

    for (int i = 0; i < appliances.size(); i++) {
        file << appliances[i].name << "|"
             << appliances[i].watts << "|"
             << appliances[i].hours << endl;
    }

    file.close();
    cout << "Appliances saved successfully.\n";
}


// Register new appliance
void registerAppliance() {
    Appliance a;

    cout << "Appliance name: ";
    getline(cin, a.name);

    cout << "Power (watts): ";
    cin >> a.watts;

    cout << "Hours used per day: ";
    cin >> a.hours;
    cin.ignore();

    appliances.push_back(a);

    cout << "Appliance added successfully.\n";
}


// View appliances
void viewAppliances() {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    cout << fixed << setprecision(2);

    cout << "\n#  Name                Watts   Hours   kWh/day\n";
    cout << "------------------------------------------------\n";

    for (int i = 0; i < appliances.size(); i++) {
        double kwh = (appliances[i].watts / 1000) * appliances[i].hours;

        cout << i + 1 << ". "
             << setw(18) << left << appliances[i].name
             << setw(8) << appliances[i].watts
             << setw(8) << appliances[i].hours
             << kwh << endl;
    }
}


// Search appliance
void searchAppliance() {
    string search;
    cout << "Enter appliance name to search: ";
    getline(cin, search);

    bool found = false;

    for (int i = 0; i < appliances.size(); i++) {
        if (appliances[i].name.find(search) != string::npos) {
            double kwh = (appliances[i].watts / 1000) * appliances[i].hours;

            cout << appliances[i].name << " | "
                 << appliances[i].watts << "W | "
                 << appliances[i].hours << " hrs | "
                 << kwh << " kWh/day\n";

            found = true;
        }
    }

    if (!found)
        cout << "Appliance not found.\n";
}


// Billing calculation
void calculateBill() {
    if (appliances.empty()) {
        cout << "No appliances available.\n";
        return;
    }

    double tariff;
    cout << "Enter tariff per kWh: ";
    cin >> tariff;
    cin.ignore();

    double totalKwh = 0;

    cout << fixed << setprecision(2);

    cout << "\nAppliance Breakdown:\n";
    cout << "------------------------------------------------\n";

    for (int i = 0; i < appliances.size(); i++) {
        double kwh = (appliances[i].watts / 1000) * appliances[i].hours;
        totalKwh += kwh;

        cout << appliances[i].name << " - "
             << kwh << " kWh/day\n";
    }

    double dailyCost = totalKwh * tariff;
    double monthlyCost = dailyCost * 30;

    cout << "\nTotal Daily Energy: " << totalKwh << " kWh\n";
    cout << "Daily Cost: " << dailyCost << endl;
    cout << "Estimated Monthly Cost (30 days): " << monthlyCost << endl;

    // Ask user to save summary
    char choice;
    cout << "\nDo you want to save this billing summary? (y/n): ";
    cin >> choice;
    cin.ignore();

    if (choice == 'y' || choice == 'Y') {

        ofstream file("billing_summary.txt", ios::app);

        if (!file) {
            cout << "Error opening billing_summary.txt\n";
            return;
        }

        // Get current time
        time_t now = time(0);
        char* dt = ctime(&now);

        file << "\n========================================\n";
        file << "Billing Summary - " << dt;
        file << "----------------------------------------\n";

        for (int i = 0; i < appliances.size(); i++) {
            double kwh = (appliances[i].watts / 1000) * appliances[i].hours;

            file << appliances[i].name << " - "
                 << kwh << " kWh/day\n";
        }

        file << "\nTotal Daily Energy: " << totalKwh << " kWh\n";
        file << "Daily Cost: " << dailyCost << endl;
        file << "Estimated Monthly Cost (30 days): " << monthlyCost << endl;
        file << "========================================\n";

        file.close();

        cout << "Billing summary saved successfully.\n";
    }
}


// Menu
void showMenu() {
    cout << "\n===== Electrical Load Monitoring System =====\n";
    cout << "1. Register Appliance\n";
    cout << "2. View Appliances\n";
    cout << "3. Search Appliance\n";
    cout << "4. Calculate Bill\n";
    cout << "5. Save Appliances\n";
    cout << "6. Exit\n";
    cout << "Choose option: ";
}


int main() {

    loadFromFile();

    int choice;

    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: registerAppliance(); break;
            case 2: viewAppliances(); break;
            case 3: searchAppliance(); break;
            case 4: calculateBill(); break;
            case 5: saveToFile(); break;
            case 6:
                saveToFile();
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}

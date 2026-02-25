#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX_APPLIANCES = 100;

// =============================
// CLASS DEFINITION
// =============================
class Appliance {
private:
    string name;
    double powerWatts;
    double usageHours;

public:
    Appliance() {
        name = "";
        powerWatts = 0;
        usageHours = 0;
    }

    void registerAppliance() {
        cin.ignore();

        cout << "\nEnter Appliance Name: ";
        getline(cin, name);

        while (name.empty()) {
            cout << "Name cannot be empty. Enter again: ";
            getline(cin, name);
        }

        cout << "Enter Power Rating (Watts): ";
        cin >> powerWatts;
        while (powerWatts <= 0) {
            cout << "Power must be greater than 0. Enter again: ";
            cin >> powerWatts;
        }

        cout << "Enter Daily Usage (Hours 0-24): ";
        cin >> usageHours;
        while (usageHours < 0 || usageHours > 24) {
            cout << "Usage must be between 0 and 24. Enter again: ";
            cin >> usageHours;
        }
    }

    double calculateEnergy() const {
        return (powerWatts * usageHours) / 1000.0; // kWh
    }

    void display() const {
        cout << left << setw(20) << name
             << setw(12) << powerWatts
             << setw(12) << usageHours
             << setw(12) << fixed << setprecision(2)
             << calculateEnergy() << endl;
    }

    string getName() const { return name; }
    double getPower() const { return powerWatts; }
    double getHours() const { return usageHours; }

    void load(string n, double p, double h) {
        name = n;
        powerWatts = p;
        usageHours = h;
    }
};

// =============================
// FUNCTION DECLARATIONS
// =============================
void displayMenu();
void viewAppliances(Appliance[], int);
void searchAppliance(Appliance[], int);
void calculateBilling(Appliance[], int);
void saveToFile(Appliance[], int);
int loadFromFile(Appliance[]);

// =============================
// MAIN FUNCTION
// =============================
int main() {

    Appliance appliances[MAX_APPLIANCES];
    int applianceCount = loadFromFile(appliances);
    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            if (applianceCount < MAX_APPLIANCES) {
                appliances[applianceCount].registerAppliance();
                applianceCount++;
                cout << "Appliance Registered Successfully!\n";
            } else {
                cout << "Maximum appliance limit reached.\n";
            }
            break;

        case 2:
            viewAppliances(appliances, applianceCount);
            break;

        case 3:
            searchAppliance(appliances, applianceCount);
            break;

        case 4:
            calculateBilling(appliances, applianceCount);
            break;

        case 5:
            saveToFile(appliances, applianceCount);
            cout << "Data saved successfully.\n";
            break;

        case 6:
            saveToFile(appliances, applianceCount);
            cout << "Program exited safely.\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}

// =============================
// MENU DISPLAY
// =============================
void displayMenu() {
    cout << "\n=====================================\n";
    cout << " ELECTRICAL LOAD MONITORING SYSTEM\n";
    cout << "=====================================\n";
    cout << "1. Register Appliance\n";
    cout << "2. View All Appliances\n";
    cout << "3. Search Appliance\n";
    cout << "4. Generate Billing Summary\n";
    cout << "5. Save Data\n";
    cout << "6. Exit\n";
    cout << "Enter choice: ";
}

// =============================
// VIEW APPLIANCES
// =============================
void viewAppliances(Appliance arr[], int count) {
    if (count == 0) {
        cout << "No appliances registered.\n";
        return;
    }

    cout << "\nRegistered Appliances:\n";
    cout << left << setw(20) << "Name"
         << setw(12) << "Watts"
         << setw(12) << "Hours"
         << setw(12) << "kWh" << endl;

    cout << "-------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        arr[i].display();
    }
}

// =============================
// SEARCH APPLIANCE
// =============================
void searchAppliance(Appliance arr[], int count) {
    if (count == 0) {
        cout << "No appliances available.\n";
        return;
    }

    cin.ignore();
    string searchName;

    cout << "Enter appliance name to search: ";
    getline(cin, searchName);

    for (int i = 0; i < count; i++) {
        if (arr[i].getName() == searchName) {
            cout << "Appliance Found:\n";
            arr[i].display();
            return;
        }
    }

    cout << "Appliance not found.\n";
}

// =============================
// BILLING CALCULATION
// =============================
void calculateBilling(Appliance arr[], int count) {

    if (count == 0) {
        cout << "No appliances registered.\n";
        return;
    }

    double tariff;
    cout << "Enter Electricity Tariff (per kWh): ";
    cin >> tariff;

    while (tariff <= 0) {
        cout << "Tariff must be positive. Enter again: ";
        cin >> tariff;
    }

    double totalEnergy = 0;

    for (int i = 0; i < count; i++) {
        totalEnergy += arr[i].calculateEnergy();
    }

    double totalCost = totalEnergy * tariff;

    cout << "\n========== BILLING SUMMARY ==========\n";
    cout << "Total Energy Consumption: "
         << fixed << setprecision(2)
         << totalEnergy << " kWh\n";

    cout << "Electricity Tariff: GHS "
         << tariff << " per kWh\n";

    cout << "Total Electricity Cost: GHS "
         << totalCost << endl;

    ofstream billFile("billing_summary.txt");
    billFile << "Total Energy: " << totalEnergy << " kWh\n";
    billFile << "Tariff: GHS " << tariff << "\n";
    billFile << "Total Cost: GHS " << totalCost << endl;
    billFile.close();
}

// =============================
// SAVE FILE
// =============================
void saveToFile(Appliance arr[], int count) {
    ofstream file("appliances.txt");

    for (int i = 0; i < count; i++) {
        file << arr[i].getName() << ","
             << arr[i].getPower() << ","
             << arr[i].getHours() << endl;
    }

    file.close();
}

// =============================
// LOAD FILE
// =============================
int loadFromFile(Appliance arr[]) {
    ifstream file("appliances.txt");

    if (!file) return 0;

    int count = 0;
    string name;
    double power, hours;

    while (getline(file, name, ',') &&
           file >> power &&
           file.ignore() &&
           file >> hours) {

        arr[count].load(name, power, hours);
        count++;
        file.ignore();
    }

    file.close();
    return count;
}

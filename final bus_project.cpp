
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <stdexcept>

using namespace std;

// Convert text to lowercase for easy searching
string toLowerCase(string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        text[i] = (char)tolower((unsigned char)text[i]);
    }
    return text;
}

// Read an integer safely
int readInteger(string message) {
    string line;
    int number;
    char extra;

    while (true) {
        cout << message;
        getline(cin, line);

        stringstream input(line);

        if (input >> number && !(input >> extra)) {
            return number;
        }

        cout << "Invalid input! Please enter a number.\n";
    }
}

// Bus class
class Bus {
public:
    int busNumber;
    string source;
    string destination;
    string departureTime;

    Bus(int number, string start, string end, string time) {
        busNumber = number;
        source = start;
        destination = end;
        departureTime = time;
    }

    void display() {
        cout << "\nBus Number    : " << busNumber;
        cout << "\nSource        : " << source;
        cout << "\nDestination   : " << destination;
        cout << "\nDeparture Time: " << departureTime;
        cout << "\n--------------------------------\n";
    }
};

// Save bus records to a text file
bool saveBuses(vector<Bus> &buses) {
    ofstream file("buses.txt");

    if (!file) {
        return false;
    }

    for (int i = 0; i < (int)buses.size(); i++) {
        file << buses[i].busNumber << "|"
             << buses[i].source << "|"
             << buses[i].destination << "|"
             << buses[i].departureTime << "\n";
    }

    file.close();
    return !file.fail();
}

// Load bus records from a text file
bool loadBuses(vector<Bus> &buses) {
    ifstream file("buses.txt");

    if (!file) {
        return false;
    }

    string line;

    while (getline(file, line)) {
        stringstream row(line);

        string numberText, source, destination, time;

        if (getline(row, numberText, '|') &&
            getline(row, source, '|') &&
            getline(row, destination, '|') &&
            getline(row, time)) {

            stringstream convert(numberText);
            int number;
            char extra;

            if (convert >> number && !(convert >> extra) &&
                number > 0 &&
                source != "" &&
                destination != "" &&
                time != "") {

                bool duplicate = false;

                for (int i = 0; i < (int)buses.size(); i++) {
                    if (buses[i].busNumber == number) {
                        duplicate = true;
                        break;
                    }
                }

                if (!duplicate) {
                    buses.push_back(
                        Bus(number, source, destination, time)
                    );
                }
            }
        }
    }

    file.close();
    return !buses.empty();
}

// Check whether a bus number already exists
bool busExists(vector<Bus> &buses, int number) {
    for (int i = 0; i < (int)buses.size(); i++) {
        if (buses[i].busNumber == number) {
            return true;
        }
    }

    return false;
}

// Add a new bus
void addBus(vector<Bus> &buses) {
    try {
        int number = readInteger("Enter bus number: ");

        if (number <= 0) {
            throw runtime_error("Bus number must be positive.");
        }

        if (busExists(buses, number)) {
            throw runtime_error("This bus number already exists.");
        }

        string source, destination, time;

        cout << "Enter source: ";
        getline(cin, source);

        cout << "Enter destination: ";
        getline(cin, destination);

        cout << "Enter departure time: ";
        getline(cin, time);

        if (source.empty() || destination.empty() || time.empty()) {
            throw runtime_error("Bus details cannot be empty.");
        }

        if (source.find('|') != string::npos ||
            destination.find('|') != string::npos ||
            time.find('|') != string::npos) {
            throw runtime_error("The | symbol is not allowed.");
        }

        buses.push_back(Bus(number, source, destination, time));

        if (saveBuses(buses)) {
            cout << "\nBus added and saved successfully!\n";
        } else {
            cout << "\nBus added in memory, but file saving failed.\n";
        }
    }
    catch (const exception &e) {
        cout << "\nError: " << e.what() << "\n";
    }
}

int main() {
    vector<Bus> buses;

    // Load saved records; create sample records on first run
    if (!loadBuses(buses)) {
        buses.push_back(
            Bus(101, "Anand", "Vallabh Vidyanagar", "8:00 AM")
        );
        buses.push_back(
            Bus(102, "Anand", "Nadiad", "9:00 AM")
        );
        buses.push_back(
            Bus(103, "Vadodara", "Anand", "10:00 AM")
        );
        buses.push_back(
            Bus(104, "Anand", "Petlad", "11:00 AM")
        );
        buses.push_back(
            Bus(105, "Nadiad", "Vadodara", "12:00 PM")
        );

        if (saveBuses(buses)) {
            cout << "Sample bus records created.\n";
        } else {
            cout << "Warning: Could not save sample records.\n";
        }
    } else {
        cout << "Saved bus records loaded successfully.\n";
    }

    int choice;

    do {
        cout << "\n====================================\n";
        cout << "     CITY BUS MANAGEMENT SYSTEM\n";
        cout << "====================================\n";
        cout << "1. View All Buses\n";
        cout << "2. Search Bus by Number\n";
        cout << "3. Search Bus by Route\n";
        cout << "4. Add New Bus\n";
        cout << "5. Total Registered Buses\n";
        cout << "6. Exit\n";
        cout << "====================================\n";

        choice = readInteger("Enter your choice: ");

        if (choice == 1) {
            for (int i = 0; i < (int)buses.size(); i++) {
                buses[i].display();
            }
        }

        else if (choice == 2) {
            int number = readInteger("Enter bus number: ");
            bool found = false;

            for (int i = 0; i < (int)buses.size(); i++) {
                if (buses[i].busNumber == number) {
                    buses[i].display();
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Sorry! Bus not found.\n";
            }
        }

        else if (choice == 3) {
            string start, end;

            cout << "Enter source: ";
            getline(cin, start);

            cout << "Enter destination: ";
            getline(cin, end);

            bool found = false;

            for (int i = 0; i < (int)buses.size(); i++) {
                if (toLowerCase(buses[i].source) ==
                        toLowerCase(start) &&
                    toLowerCase(buses[i].destination) ==
                        toLowerCase(end)) {

                    buses[i].display();
                    found = true;
                }
            }

            if (!found) {
                cout << "No bus found for this route.\n";
                cout << "Please check the source and destination.\n";
            }
        }

        else if (choice == 4) {
            addBus(buses);
        }

        else if (choice == 5) {
            cout << "Total registered buses: "
                 << buses.size() << "\n";
        }

        else if (choice == 6) {
            cout << "Thank you for using City Bus Management System!\n";
        }

        else {
            cout << "Invalid choice! Select from 1 to 6.\n";
        }

    } while (choice != 6);

    return 0;
}


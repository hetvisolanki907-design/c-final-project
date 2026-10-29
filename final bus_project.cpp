
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdexcept>

using namespace std;

// Convert text to lowercase for case-insensitive searching
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

// PILLAR 1: ENCAPSULATION
// PILLAR 2: ABSTRACTION
// Transport is an abstract base class.
class Transport {
private:
    int busNumber;
    string source;
    string destination;
    string departureTime;

public:
    Transport(int number, string start,
              string end, string time) {
        busNumber = number;
        source = start;
        destination = end;
        departureTime = time;
    }

    // Public getters provide controlled access to private data
    int getBusNumber() {
        return busNumber;
    }

    string getSource() {
        return source;
    }

    string getDestination() {
        return destination;
    }

    string getDepartureTime() {
        return departureTime;
    }

    // Pure virtual function makes Transport an abstract class
    virtual void display() = 0;

    virtual ~Transport() {
    }
};

// PILLAR 3: INHERITANCE
// Bus inherits the properties and behaviour of Transport.
class Bus : public Transport {
public:
    Bus(int number, string start,
        string end, string time)
        : Transport(number, start, end, time) {
    }

    // PILLAR 4: POLYMORPHISM
    // Bus provides its own version of display().
    virtual void display() {
        cout << "\nBus Number     : " << getBusNumber();
        cout << "\nSource         : " << getSource();
        cout << "\nDestination    : " << getDestination();
        cout << "\nDeparture Time : " << getDepartureTime();
        cout << "\n----------------------------------\n";
    }
};

// Save bus records to a file
bool saveBuses(vector<Bus> &buses) {
    ofstream file("buses.txt");

    if (!file) {
        return false;
    }

    for (int i = 0; i < (int)buses.size(); i++) {
        file << buses[i].getBusNumber() << "|"
             << buses[i].getSource() << "|"
             << buses[i].getDestination() << "|"
             << buses[i].getDepartureTime() << "\n";
    }

    file.close();
    return !file.fail();
}

// Load bus records from a file
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
                number > 0 && source != "" &&
                destination != "" && time != "") {

                bool duplicate = false;

                for (int i = 0; i < (int)buses.size(); i++) {
                    if (buses[i].getBusNumber() == number) {
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

int main() {
    try {
        vector<Bus> buses;

        // Load saved records or create sample records
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
                cout << "Sample bus records created successfully.\n";
            } else {
                cout << "Warning: Could not save bus records.\n";
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
            cout << "4. Total Registered Buses\n";
            cout << "5. Exit\n";
            cout << "====================================\n";

            choice = readInteger("Enter your choice: ");

            if (choice == 1) {
                // Polymorphism: call display through a base-class pointer
                for (int i = 0; i < (int)buses.size(); i++) {
                    Transport *transport = &buses[i];
                    transport->display();
                }
            }

            else if (choice == 2) {
                int number = readInteger("Enter bus number: ");
                bool found = false;

                for (int i = 0; i < (int)buses.size(); i++) {
                    if (buses[i].getBusNumber() == number) {
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
                    if (toLowerCase(buses[i].getSource()) ==
                            toLowerCase(start) &&
                        toLowerCase(buses[i].getDestination()) ==
                            toLowerCase(end)) {

                        buses[i].display();
                        found = true;
                    }
                }

                if (!found) {
                    cout << "No bus found for this route.\n";
                    cout << "Please check the spelling of the locations.\n";
                }
            }

            else if (choice == 4) {
                cout << "Total registered buses: "
                     << buses.size() << "\n";
            }

            else if (choice == 5) {
                cout << "Thank you for using City Bus Management System!\n";
            }

            else {
                cout << "Invalid choice! Please select from 1 to 5.\n";
            }

        } while (choice != 5);
    }
    catch (const exception &e) {
        cout << "An unexpected error occurred: " << e.what() << "\n";
    }

    return 0;
}

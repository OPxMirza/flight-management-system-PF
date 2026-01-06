#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// -------------------- STRUCTS --------------------
struct Flight {
    int id;
    string from;
    string to;
    int seats;
    int price;
    string date;
    string departureTime;
    string arrivalTime;
};

struct Booking {
    int bookingID;         // Random booking id (unique)
    string accountName;
    string phone;
    int flightID;
    string passengerName;
    string status;         // "ACTIVE" or "CANCELLED"
    string paymentMethod;  // "CASH" / "CARD" / "ONLINE"
};

// -------------------- GLOBAL ARRAYS --------------------
Flight flights[50];
int flightCount = 0;

Booking bookings[50];
int bookingCount = 0;

// -------------------- INPUT HELPERS --------------------
int getValidInt(const string &msg) {
    int num;
    while (true) {
        cout << msg;
        if (cin >> num) break;

        cout << "Invalid input! Enter a number.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return num;
}

int getValidChoice(const string &msg, int min, int max) {
    int choice;
    while (true) {
        cout << msg;
        if (cin >> choice) {
            if (choice >= min && choice <= max) return choice;
            cout << "Invalid choice! Please enter a number between " << min << " and " << max << ".\n";
        } else {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

bool isDigitsOnly(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return true;
}

bool isEmailValidBasic(const string &email) {
    // Basic check: must contain one '@' and one '.'
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    return (atPos != string::npos && dotPos != string::npos && atPos < dotPos && atPos > 0 && dotPos < email.size() - 1);
}

// -------------------- BOOKING ID (RANDOM + UNIQUE) --------------------
bool bookingIDExists(int id) {
    Booking* b = bookings;
    for (int i = 0; i < bookingCount; i++) {
        if (b->bookingID == id) return true;
        b++;
    }
    return false;
}

int generateUniqueBookingID() {
    int id;
    do {
        id = 100000 + (rand() % 900000); // 6-digit
    } while (bookingIDExists(id));
    return id;
}

// -------------------- POINTER HELPERS --------------------
Flight* findFlightByID(int id) {
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].id == id) return &flights[i];
    }
    return nullptr;
}

// Only returns true if an ACTIVE booking exists (cancelled bookings do NOT block rebooking)
bool checkActiveBookingExists(const string &name, const string &phone, int flightID) {
    Booking* bookingPtr = bookings;
    for (int i = 0; i < bookingCount; i++) {
        if (bookingPtr->accountName == name &&
            bookingPtr->phone == phone &&
            bookingPtr->flightID == flightID &&
            bookingPtr->status == "ACTIVE") {
            return true;
        }
        bookingPtr++;
    }
    return false;
}

void displayFlight(Flight* flightPtr) {
    if (flightPtr != nullptr) {
        cout << "ID: " << flightPtr->id << " | " << flightPtr->from << " -> " << flightPtr->to
             << " | Seats: " << flightPtr->seats << " | Price: Rs " << flightPtr->price
             << " | Date: " << flightPtr->date
             << " | Departure: " << flightPtr->departureTime
             << " | Arrival: " << flightPtr->arrivalTime << endl;
    }
}

// -------------------- FILE OPERATIONS --------------------
void loadFlights() {
    ifstream file("flights.txt");
    flightCount = 0;

    Flight* flightPtr = flights;
    while (file >> flightPtr->id >> flightPtr->from >> flightPtr->to
           >> flightPtr->seats >> flightPtr->price
           >> flightPtr->date >> flightPtr->departureTime
           >> flightPtr->arrivalTime) {
        flightCount++;
        flightPtr++;
        if (flightCount >= 50) break;
    }
    file.close();
}

void saveFlightsToFile() {
    ofstream file("flights.txt");
    Flight* flightPtr = flights;

    for (int i = 0; i < flightCount; i++) {
        file << flightPtr->id << " " << flightPtr->from << " " << flightPtr->to << " "
             << flightPtr->seats << " " << flightPtr->price << " "
             << flightPtr->date << " " << flightPtr->departureTime << " "
             << flightPtr->arrivalTime << endl;
        flightPtr++;
    }
    file.close();
}

/*
bookings.txt format:
bookingID accountName phone flightID passengerName status paymentMethod
*/
void loadBookings() {
    ifstream file("bookings.txt");
    bookingCount = 0;

    Booking* bookingPtr = bookings;
    while (file >> bookingPtr->bookingID
           >> bookingPtr->accountName >> bookingPtr->phone
           >> bookingPtr->flightID >> bookingPtr->passengerName
           >> bookingPtr->status >> bookingPtr->paymentMethod) {
        bookingCount++;
        bookingPtr++;
        if (bookingCount >= 50) break;
    }
    file.close();
}

void saveBookingsToFile() {
    ofstream file("bookings.txt");
    Booking* bookingPtr = bookings;

    for (int i = 0; i < bookingCount; i++) {
        file << bookingPtr->bookingID << " "
             << bookingPtr->accountName << " " << bookingPtr->phone << " "
             << bookingPtr->flightID << " " << bookingPtr->passengerName << " "
             << bookingPtr->status << " " << bookingPtr->paymentMethod << endl;
        bookingPtr++;
    }
    file.close();
}

// -------------------- ADMIN LOGIN --------------------
bool adminLogin() {
    const string storedUser = "admin";
    const string storedPass = "123";

    string user, pass;

    while (true) {
        cout << "Enter Admin Username: ";
        cin >> user;
        cout << "Enter Password: ";
        cin >> pass;
        cout << endl;

        if (user == storedUser && pass == storedPass) {
            cout << "\nLogin Successful!\n";
            return true;
        } else {
            cout << "Wrong username or password! Try again.\n";
        }
    }
}

// -------------------- PASSENGER SIGNUP/LOGIN (EMAIL BASED) --------------------
/*
passengers.txt format:
name email phone password
*/

bool emailExists(const string &email) {
    ifstream file("passengers.txt");
    string n, e, p, pass;

    while (file >> n >> e >> p >> pass) {
        if (e == email) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void passengerSignup() {
    string name, email, phone, pass;

    cout << "Enter Name: ";
    cin >> name;

    while (true) {
        cout << "Enter Email: ";
        cin >> email;

        if (!isEmailValidBasic(email)) {
            cout << "Invalid email format! Example: user@gmail.com\n";
            continue;
        }

        if (emailExists(email)) {
            cout << "This email already exists! Please use another email.\n";
        } else {
            break;
        }
    }

    cout << "Enter Phone: ";
    cin >> phone;

    cout << "Enter Password: ";
    cin >> pass;
    cout << endl;

    ofstream file("passengers.txt", ios::app);
    file << name << " " << email << " " << phone << " " << pass << endl;

    cout << "Signup Successful!\n";
}

bool passengerLogin(string &name, string &email, string &phone) {
    string inputEmail, inputPass;

    string names[50], emails[50], phones[50], passwords[50];
    int userCount = 0;

    ifstream file("passengers.txt");
    while (file >> names[userCount] >> emails[userCount] >> phones[userCount] >> passwords[userCount]) {
        userCount++;
        if (userCount >= 50) break;
    }
    file.close();

    if (userCount == 0) {
        cout << "No passengers found. Please signup first.\n";
        return false;
    }

    while (true) {
        cout << "Enter Email: ";
        cin >> inputEmail;
        cout << "Enter Password: ";
        cin >> inputPass;
        cout << endl;

        bool emailFound = false;
        bool loginOK = false;

        string* nPtr = names;
        string* ePtr = emails;
        string* phPtr = phones;
        string* passPtr = passwords;

        for (int i = 0; i < userCount; i++) {
            if (inputEmail == *ePtr) {
                emailFound = true;
                if (inputPass == *passPtr) {
                    name = *nPtr;
                    email = *ePtr;
                    phone = *phPtr;
                    loginOK = true;
                }
                break;
            }
            nPtr++; ePtr++; phPtr++; passPtr++;
        }

        if (!emailFound) {
            cout << "User not found! This email is not registered. Please signup.\n";
        } else if (!loginOK) {
            cout << "Wrong password! Try again.\n";
        } else {
            cout << "\nLogin Successful!\n";
            return true;
        }
    }
}

// -------------------- PAYMENT --------------------
string choosePaymentMethod() {
    cout << "\n--- Payment Method ---\n";
    cout << "1. Cash\n2. Card\n3. Online Payment\n";
    int c = getValidChoice("Choose: ", 1, 3);

    if (c == 1) return "CASH";
    if (c == 2) return "CARD";
    return "ONLINE";
}

bool processPayment(const string &method, int amountRs) {
    cout << "\nAmount to Pay: Rs " << amountRs << "\n";

    if (method == "CASH") {
        int paid = getValidInt("Enter cash amount given: ");
        if (paid < amountRs) {
            cout << "Payment failed: not enough cash.\n";
            return false;
        }
        cout << "Payment successful. Change: Rs " << (paid - amountRs) << "\n";
        return true;
    }

    if (method == "CARD") {
        string cardNo, pin;
        cout << "Enter 16-digit Card Number: ";
        cin >> cardNo;
        cout << "Enter 4-digit PIN: ";
        cin >> pin;

        if (cardNo.size() != 16 || !isDigitsOnly(cardNo) || pin.size() != 4 || !isDigitsOnly(pin)) {
            cout << "Payment failed: invalid card details.\n";
            return false;
        }
        cout << "Payment successful via CARD.\n";
        return true;
    }

    if (method == "ONLINE") {
        // Simple online payment simulation (transaction id + otp)
        string transactionID, otp;
        cout << "Enter Transaction ID: ";
        cin >> transactionID;
        cout << "Enter OTP (4 digits): ";
        cin >> otp;

        if (transactionID.size() < 4 || otp.size() != 4 || !isDigitsOnly(otp)) {
            cout << "Payment failed: invalid online payment details.\n";
            return false;
        }
        cout << "Payment successful via ONLINE payment.\n";
        return true;
    }

    cout << "Payment failed: unknown method.\n";
    return false;
}

// -------------------- FLIGHT OPERATIONS --------------------
void viewFlights() {
    cout << "\n--- Available Flights ---\n";
    if (flightCount == 0) {
        cout << "No flights available.\n";
        return;
    }

    Flight* flightPtr = flights;
    for (int i = 0; i < flightCount; i++) {
        displayFlight(flightPtr);
        flightPtr++;
    }
}

void searchFlights() {
    string from, to, date;
    cout << "\n--- Search Flights ---\n";
    cout << "From: "; cin >> from;
    cout << "To: "; cin >> to;
    cout << "Date (YYYY-MM-DD): "; cin >> date;

    bool found = false;
    cout << "\n--- Search Results ---\n";

    Flight* flightPtr = flights;
    for (int i = 0; i < flightCount; i++) {
        if (flightPtr->from == from && flightPtr->to == to && flightPtr->date == date) {
            found = true;
            displayFlight(flightPtr);
        }
        flightPtr++;
    }

    if (!found) cout << "No flights found for the given search criteria.\n";
}

void addFlight() {
    if (flightCount >= 50) {
        cout << "Maximum flights reached!\n";
        return;
    }

    Flight* newFlight = &flights[flightCount];
    newFlight->id = getValidInt("Enter Flight ID: ");
    cout << "From: "; cin >> newFlight->from;
    cout << "To: "; cin >> newFlight->to;
    newFlight->seats = getValidInt("Seats: ");
    newFlight->price = getValidInt("Price: ");
    cout << "Date (YYYY-MM-DD): "; cin >> newFlight->date;
    cout << "Departure Time (HH:MM): "; cin >> newFlight->departureTime;
    cout << "Arrival Time (HH:MM): "; cin >> newFlight->arrivalTime;

    flightCount++;
    saveFlightsToFile();
    cout << "Flight added successfully!\n";
}

void bookTicket(const string &pName, const string &pPhone) {
    searchFlights();

    while (true) {
        int id = getValidInt("\nEnter Flight ID to book: ");
        Flight* flightPtr = findFlightByID(id);

        if (flightPtr == nullptr) {
            cout << "Invalid Flight ID! Please try again.\n";
            continue;
        }

        if (flightPtr->seats <= 0) {
            cout << "No seats available!\n";
            return;
        }

        if (checkActiveBookingExists(pName, pPhone, id)) {
            cout << "You already booked this flight (ACTIVE).\n";
            return;
        }

        if (bookingCount >= 50) {
            cout << "Maximum bookings reached!\n";
            return;
        }

        string passengerName;
        cout << "Enter Passenger Name (who will be travelling): ";
        cin >> passengerName;

        string method = choosePaymentMethod();
        bool paid = processPayment(method, flightPtr->price);
        if (!paid) {
            cout << "Booking cancelled because payment failed.\n";
            return;
        }

        int newBID = generateUniqueBookingID();

        Booking* newBooking = &bookings[bookingCount];
        newBooking->bookingID = newBID;
        newBooking->accountName = pName;
        newBooking->phone = pPhone;
        newBooking->flightID = id;
        newBooking->passengerName = passengerName;
        newBooking->status = "ACTIVE";
        newBooking->paymentMethod = method;

        bookingCount++;
        saveBookingsToFile();

        flightPtr->seats--;
        saveFlightsToFile();

        cout << "\nTICKET BOOKED SUCCESSFULLY!\n";
        cout << "Booking ID: " << newBID
             << "\nBooked by: " << pName
             << "\nPassenger: " << passengerName
             << "\nFlight: " << flightPtr->from << " -> " << flightPtr->to
             << "\nDate: " << flightPtr->date
             << "\nDeparture: " << flightPtr->departureTime
             << " | Arrival: " << flightPtr->arrivalTime
             << "\nPrice: Rs " << flightPtr->price
             << "\nPayment Method: " << method
             << "\nStatus: ACTIVE\n";
        return;
    }
}

void viewMyFlightHistory(const string &pName, const string &pPhone) {
    bool found = false;
    cout << "\n--- Your Flight History (Active + Cancelled) ---\n";

    Booking* bookingPtr = bookings;
    for (int i = 0; i < bookingCount; i++) {
        if (bookingPtr->accountName == pName && bookingPtr->phone == pPhone) {
            found = true;

            Flight* flightPtr = findFlightByID(bookingPtr->flightID);
            if (flightPtr != nullptr) {
                cout << "Booking ID: " << bookingPtr->bookingID
                     << " | Flight ID: " << flightPtr->id
                     << " | Passenger: " << bookingPtr->passengerName
                     << " | " << flightPtr->from << " -> " << flightPtr->to
                     << " | Date: " << flightPtr->date
                     << " | Departure: " << flightPtr->departureTime
                     << " | Arrival: " << flightPtr->arrivalTime
                     << " | Price: Rs " << flightPtr->price
                     << " | Payment: " << bookingPtr->paymentMethod
                     << " | Status: " << bookingPtr->status
                     << endl;
            } else {
                cout << "Booking ID: " << bookingPtr->bookingID
                     << " | Flight ID: " << bookingPtr->flightID
                     << " | Passenger: " << bookingPtr->passengerName
                     << " | Payment: " << bookingPtr->paymentMethod
                     << " | Status: " << bookingPtr->status
                     << " | (Flight info not found)\n";
            }
        }
        bookingPtr++;
    }

    if (!found) cout << "You have no history.\n";
}

void cancelBooking(const string &pName, const string &pPhone) {
    viewMyFlightHistory(pName, pPhone);

    while (true) {
        int bookingID = getValidInt("Enter Booking ID to cancel: ");

        bool cancelled = false;

        Booking* bookingPtr = bookings;
        for (int i = 0; i < bookingCount; i++) {
            if (bookingPtr->accountName == pName &&
                bookingPtr->phone == pPhone &&
                bookingPtr->bookingID == bookingID &&
                bookingPtr->status == "ACTIVE") {

                bookingPtr->status = "CANCELLED";
                cancelled = true;

                Flight* flightPtr = findFlightByID(bookingPtr->flightID);
                if (flightPtr != nullptr) flightPtr->seats++;

                break;
            }
            bookingPtr++;
        }

        if (cancelled) {
            saveBookingsToFile();
            saveFlightsToFile();
            cout << "Booking cancelled successfully! (Saved in history)\n";
            return;
        } else {
            cout << "No ACTIVE booking found with this Booking ID for you. Please try again.\n";
        }
    }
}

// -------------------- MAIN --------------------
int main() {
    srand((unsigned)time(nullptr)); // seed random once

    loadFlights();
    loadBookings();

    bool showMainMenu = true;

    while (showMainMenu) {
        cout << "\n===== WELCOME TO FLIGHT MANAGEMENT SYSTEM =====\n";
        cout << "Select User Type:\n1. Admin\n2. Passenger\n3. Exit\n";
        int userType = getValidChoice("Choice: ", 1, 3);

        if (userType == 1) {
            if (adminLogin()) {
                int choice;
                do {
                    cout << "\n--- ADMIN MENU ---\n";
                    cout << "1. View Flights\n2. Add Flight\n3. Back to Main Menu\n";
                    choice = getValidChoice("Choice: ", 1, 3);

                    switch (choice) {
                        case 1: viewFlights(); break;
                        case 2: addFlight(); break;
                        case 3: break;
                    }
                } while (choice != 3);
            }
        }
        else if (userType == 2) {
            int option = getValidChoice("1. Login\n2. Signup\nChoice: ", 1, 2);

            string name, email, phone;

            if (option == 2) passengerSignup();

            bool ok = passengerLogin(name, email, phone);
            if (!ok) continue;

            int choice;
            do {
                cout << "\n--- PASSENGER MENU ---\n";
                cout << "1. Search Flights\n2. Book Ticket\n3. Cancel Booking\n4. View My Flight History\n5. Back to Main Menu\n";
                choice = getValidChoice("Choice: ", 1, 5);

                switch (choice) {
                    case 1: searchFlights(); break;
                    case 2: bookTicket(name, phone); break;
                    case 3: cancelBooking(name, phone); break;
                    case 4: viewMyFlightHistory(name, phone); break;
                    case 5: break;
                }
            } while (choice != 5);
        }
        else if (userType == 3) {
            cout << "Exiting program...\n";
            showMainMenu = false;
        }
    }

    return 0;
}

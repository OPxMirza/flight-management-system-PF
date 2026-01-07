#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

// -------------------- STRUCTS --------------------
struct Flight {
    int id;                 // Random unique flight ID
    string from;
    string to;
    int seats;
    int price;
    string date;            // YYYY-MM-DD
    string departureTime;   // HH:MM
    string arrivalTime;     // HH:MM
};

struct Booking {
    int bookingID;          // Random unique booking ID
    string accountName;
    string phone;
    int flightID;
    string passengerName;
    string status;          // ACTIVE / CANCELLED
    string paymentMethod;   // CASH / CARD / ONLINE
};

struct Passenger {
    string name;
    string email;
    string phone;
    string password;
};

// -------------------- GLOBAL ARRAYS --------------------
Flight flights[50];
int flightCount = 0;

Booking bookings[50];
int bookingCount = 0;

Passenger passengers[50];
int passengerCount = 0;

// -------------------- I/O HELPERS --------------------
void clearLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string readLineNonEmpty(const string &msg) {
    string s;
    while (true) {
        cout << msg;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "Input cannot be empty.\n";
    }
}

string readTokenOrBack(const string &msg) {
    // reads a single token (no spaces) using >> but allows "0" for back
    string s;
    cout << msg;
    cin >> s;
    return s;
}

// -------------------- BASIC HELPERS --------------------
bool isDigitsOnly(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return true;
}

bool isEmailValidBasic(const string &email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    return (atPos != string::npos && dotPos != string::npos && atPos < dotPos && atPos > 0 && dotPos < email.size() - 1);
}

bool isValidPKPhone(const string &phone) {
    // Strict Pakistan mobile format: 03XXXXXXXXX (11 digits)
    if (phone.size() != 11) return false;
    if (!isDigitsOnly(phone)) return false;
    if (phone[0] != '0' || phone[1] != '3') return false;
    return true;
}

bool isLeapYear(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

bool isValidDate(const string &d) {
    // YYYY-MM-DD, real calendar validation
    if (d.size() != 10) return false;
    if (d[4] != '-' || d[7] != '-') return false;

    string ys = d.substr(0, 4);
    string ms = d.substr(5, 2);
    string ds = d.substr(8, 2);

    if (!isDigitsOnly(ys) || !isDigitsOnly(ms) || !isDigitsOnly(ds)) return false;

    int y = stoi(ys), m = stoi(ms), day = stoi(ds);

    if (y < 2000 || y > 2100) return false;
    if (m < 1 || m > 12) return false;

    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int maxDay = daysInMonth[m];
    if (m == 2 && isLeapYear(y)) maxDay = 29;

    return (day >= 1 && day <= maxDay);
}

bool isValidTimeHHMM(const string &t) {
    // HH:MM 24-hour
    if (t.size() != 5) return false;
    if (t[2] != ':') return false;

    string hs = t.substr(0,2);
    string ms = t.substr(3,2);

    if (!isDigitsOnly(hs) || !isDigitsOnly(ms)) return false;

    int h = stoi(hs);
    int m = stoi(ms);

    return (h >= 0 && h <= 23 && m >= 0 && m <= 59);
}

// -------------------- INPUT HELPERS --------------------
int getValidInt(const string &msg) {
    int num;
    while (true) {
        cout << msg;
        if (cin >> num) return num;

        cout << "Invalid input! Enter a number.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
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

string getValidDatePrompt(const string &msg) {
    string d;
    while (true) {
        cout << msg;
        cin >> d;
        if (isValidDate(d)) return d;
        cout << "Invalid date! Use YYYY-MM-DD (real date).\n";
    }
}

string getValidTimePrompt(const string &msg) {
    string t;
    while (true) {
        cout << msg;
        cin >> t;
        if (isValidTimeHHMM(t)) return t;
        cout << "Invalid time! Use HH:MM (24-hour).\n";
    }
}

string getValidPhonePrompt(const string &msg) {
    string p;
    while (true) {
        cout << msg;
        cin >> p;
        if (isValidPKPhone(p)) return p;
        cout << "Invalid phone! Use Pakistan format: 03XXXXXXXXX (11 digits)\n";
    }
}

// -------------------- FILE OPERATIONS --------------------
void loadFlights() {
    ifstream file("flights.txt");
    flightCount = 0;

    // FORMAT (with spaces allowed):
    // id|from|to|seats|price|date|departure|arrival
    // Using '|' delimiter for safe space support.
    // Example:
    // 123|Karachi|Lahore|20|18000|2026-01-10|09:00|10:30

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (flightCount >= 50) break;

        // parse by '|'
        int partsCount = 0;
        string parts[8];
        string temp;
        for (char c : line) {
            if (c == '|') {
                if (partsCount < 8) parts[partsCount++] = temp;
                temp.clear();
            } else temp.push_back(c);
        }
        if (partsCount < 7) continue;
        parts[partsCount++] = temp;

        if (partsCount != 8) continue;

        flights[flightCount].id = stoi(parts[0]);
        flights[flightCount].from = parts[1];
        flights[flightCount].to = parts[2];
        flights[flightCount].seats = stoi(parts[3]);
        flights[flightCount].price = stoi(parts[4]);
        flights[flightCount].date = parts[5];
        flights[flightCount].departureTime = parts[6];
        flights[flightCount].arrivalTime = parts[7];

        flightCount++;
    }
    file.close();
}

void saveFlightsToFile() {
    ofstream file("flights.txt");
    for (int i = 0; i < flightCount; i++) {
        file << flights[i].id << "|"
             << flights[i].from << "|"
             << flights[i].to << "|"
             << flights[i].seats << "|"
             << flights[i].price << "|"
             << flights[i].date << "|"
             << flights[i].departureTime << "|"
             << flights[i].arrivalTime << "\n";
    }
    file.close();
}

void loadBookings() {
    ifstream file("bookings.txt");
    bookingCount = 0;

    // FORMAT:
    // bookingID|accountName|phone|flightID|passengerName|status|paymentMethod
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (bookingCount >= 50) break;

        int partsCount = 0;
        string parts[7];
        string temp;

        for (char c : line) {
            if (c == '|') {
                if (partsCount < 7) parts[partsCount++] = temp;
                temp.clear();
            } else temp.push_back(c);
        }
        if (partsCount < 6) continue;
        parts[partsCount++] = temp;

        if (partsCount != 7) continue;

        bookings[bookingCount].bookingID = stoi(parts[0]);
        bookings[bookingCount].accountName = parts[1];
        bookings[bookingCount].phone = parts[2];
        bookings[bookingCount].flightID = stoi(parts[3]);
        bookings[bookingCount].passengerName = parts[4];
        bookings[bookingCount].status = parts[5];
        bookings[bookingCount].paymentMethod = parts[6];

        bookingCount++;
    }
    file.close();
}

void saveBookingsToFile() {
    ofstream file("bookings.txt");
    for (int i = 0; i < bookingCount; i++) {
        file << bookings[i].bookingID << "|"
             << bookings[i].accountName << "|"
             << bookings[i].phone << "|"
             << bookings[i].flightID << "|"
             << bookings[i].passengerName << "|"
             << bookings[i].status << "|"
             << bookings[i].paymentMethod << "\n";
    }
    file.close();
}

void loadPassengers() {
    ifstream file("passengers.txt");
    passengerCount = 0;

    // FORMAT:
    // name|email|phone|password
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (passengerCount >= 50) break;

        int partsCount = 0;
        string parts[4];
        string temp;

        for (char c : line) {
            if (c == '|') {
                if (partsCount < 4) parts[partsCount++] = temp;
                temp.clear();
            } else temp.push_back(c);
        }
        if (partsCount < 3) continue;
        parts[partsCount++] = temp;

        if (partsCount != 4) continue;

        passengers[passengerCount].name = parts[0];
        passengers[passengerCount].email = parts[1];
        passengers[passengerCount].phone = parts[2];
        passengers[passengerCount].password = parts[3];

        passengerCount++;
    }
    file.close();
}

bool emailExists(const string &email) {
    loadPassengers();
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].email == email) return true;
    }
    return false;
}

// -------------------- POINTER HELPERS --------------------
Flight* findFlightByID(int id) {
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].id == id) return &flights[i];
    }
    return nullptr;
}

void displayFlight(Flight* f) {
    if (!f) return;
    cout << "ID: " << f->id << " | " << f->from << " -> " << f->to
         << " | Seats: " << f->seats << " | Price: Rs " << f->price
         << " | Date: " << f->date
         << " | Departure: " << f->departureTime
         << " | Arrival: " << f->arrivalTime << "\n";
}

// -------------------- RANDOM UNIQUE IDS --------------------
bool bookingIDExists(int id) {
    for (int i = 0; i < bookingCount; i++) if (bookings[i].bookingID == id) return true;
    return false;
}

int generateUniqueBookingID() {
    int id;
    do { id = 100000 + (rand() % 900000); } while (bookingIDExists(id));
    return id;
}

bool flightIDExists(int id) {
    for (int i = 0; i < flightCount; i++) if (flights[i].id == id) return true;
    return false;
}

int generateUniqueFlightID() {
    int id;
    do { id = 100 + (rand() % 900); } while (flightIDExists(id));
    return id;
}

bool checkActiveBookingExists(const string &name, const string &phone, int flightID) {
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].accountName == name &&
            bookings[i].phone == phone &&
            bookings[i].flightID == flightID &&
            bookings[i].status == "ACTIVE") {
            return true;
        }
    }
    return false;
}

// -------------------- ADMIN LOGIN --------------------
bool adminLogin() {
    const string storedUser = "admin";
    const string storedPass = "123";

    while (true) {
        cout << "\n--- Admin Login ---\n";
        cout << "1. Login\n2. Back\n";
        int c = getValidChoice("Choice: ", 1, 2);
        if (c == 2) return false;

        string user, pass;
        cout << "Enter Admin Username: ";
        cin >> user;
        cout << "Enter Password: ";
        cin >> pass;

        if (user == storedUser && pass == storedPass) {
            cout << "\nLogin Successful!\n";
            return true;
        }
        cout << "Wrong username or password!\n";
    }
}

// -------------------- PASSENGER SIGNUP/LOGIN --------------------
bool passengerSignupAndAutoLogin(string &name, string &email, string &phone) {
    cout << "\n--- Passenger Signup ---\n";

    clearLine();
    name = readLineNonEmpty("Enter Full Name: ");

    while (true) {
        cout << "Enter Email: ";
        cin >> email;

        if (!isEmailValidBasic(email)) {
            cout << "Invalid email format! Example: user@gmail.com\n";
            continue;
        }
        if (emailExists(email)) {
            cout << "This email already exists! Use another email.\n";
            continue;
        }
        break;
    }

    phone = getValidPhonePrompt("Enter Phone (03XXXXXXXXX): ");

    string pass;
    cout << "Enter Password: ";
    cin >> pass;

    // Save with delimiter '|'
    ofstream file("passengers.txt", ios::app);
    file << name << "|" << email << "|" << phone << "|" << pass << "\n";

    cout << "Signup Successful! Redirecting to Passenger Home...\n";
    return true;
}

bool passengerLogin(string &name, string &email, string &phone) {
    loadPassengers();
    if (passengerCount == 0) {
        cout << "No passengers found. Please signup first.\n";
        return false;
    }

    while (true) {
        cout << "\n--- Passenger Login ---\n";
        cout << "Enter Email (or 0 to go back): ";
        cin >> email;
        if (email == "0") return false;

        string pass;
        cout << "Enter Password: ";
        cin >> pass;

        bool emailFound = false;
        for (int i = 0; i < passengerCount; i++) {
            if (passengers[i].email == email) {
                emailFound = true;
                if (passengers[i].password == pass) {
                    name = passengers[i].name;
                    phone = passengers[i].phone;
                    cout << "\nLogin Successful!\n";
                    return true;
                } else {
                    cout << "Wrong password!\n";
                }
                break;
            }
        }

        if (!emailFound) cout << "User not found! This email is not registered.\n";
    }
}

// -------------------- PAYMENT --------------------
int choosePaymentMethod(string &methodOut) {
    cout << "\n--- Payment Method ---\n";
    cout << "1. Cash\n2. Card\n3. Online Payment\n4. Back\n";
    int c = getValidChoice("Choose: ", 1, 4);

    if (c == 4) return 0;
    if (c == 1) methodOut = "CASH";
    else if (c == 2) methodOut = "CARD";
    else methodOut = "ONLINE";
    return 1;
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
        string txn, otp;
        cout << "Enter Transaction ID: ";
        cin >> txn;
        cout << "Enter OTP (4 digits): ";
        cin >> otp;

        if (txn.size() < 4 || otp.size() != 4 || !isDigitsOnly(otp)) {
            cout << "Payment failed: invalid online payment details.\n";
            return false;
        }
        cout << "Payment successful via ONLINE payment.\n";
        return true;
    }

    return false;
}

// -------------------- PASSENGER FUNCTIONS --------------------
void viewAllFlightsPassenger() {
    cout << "\n--- All Available Flights ---\n";
    if (flightCount == 0) {
        cout << "No flights available.\n";
        return;
    }
    for (int i = 0; i < flightCount; i++) displayFlight(&flights[i]);
}

void searchFlights() {
    clearLine();
    string from = readLineNonEmpty("\nFrom: ");
    string to   = readLineNonEmpty("To: ");
    string date = getValidDatePrompt("Date (YYYY-MM-DD): ");

    bool found = false;
    cout << "\n--- Search Results ---\n";
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].from == from && flights[i].to == to && flights[i].date == date) {
            displayFlight(&flights[i]);
            found = true;
        }
    }
    if (!found) cout << "No flights found.\n";
}

void bookTicket(const string &pName, const string &pPhone) {
    searchFlights();

    int id = getValidInt("\nEnter Flight ID to book (0 to go back): ");
    if (id == 0) return;

    Flight* f = findFlightByID(id);
    if (!f) { cout << "Invalid Flight ID!\n"; return; }
    if (f->seats <= 0) { cout << "No seats available!\n"; return; }
    if (checkActiveBookingExists(pName, pPhone, id)) { cout << "You already booked this flight (ACTIVE).\n"; return; }
    if (bookingCount >= 50) { cout << "Maximum bookings reached!\n"; return; }

    clearLine();
    string traveller = readLineNonEmpty("Enter Traveller Full Name: ");

    string method;
    if (!choosePaymentMethod(method)) {
        cout << "Booking cancelled (Back).\n";
        return;
    }

    if (!processPayment(method, f->price)) {
        cout << "Booking cancelled because payment failed.\n";
        return;
    }

    int newBID = generateUniqueBookingID();
    bookings[bookingCount] = {newBID, pName, pPhone, id, traveller, "ACTIVE", method};
    bookingCount++;
    saveBookingsToFile();

    f->seats--;
    saveFlightsToFile();

    cout << "\nTICKET BOOKED SUCCESSFULLY!\n";
    cout << "Booking ID: " << newBID
         << "\nFlight ID: " << id
         << "\nBooked by: " << pName
         << "\nTraveller: " << traveller
         << "\nFlight: " << f->from << " -> " << f->to
         << "\nDate: " << f->date
         << "\nDeparture: " << f->departureTime
         << " | Arrival: " << f->arrivalTime
         << "\nPrice: Rs " << f->price
         << "\nPayment Method: " << method
         << "\nStatus: ACTIVE\n";
}

void viewMyFlightHistory(const string &pName, const string &pPhone) {
    cout << "\n--- Your Flight History (Active + Cancelled) ---\n";
    bool found = false;

    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].accountName == pName && bookings[i].phone == pPhone) {
            found = true;
            Flight* f = findFlightByID(bookings[i].flightID);

            cout << "Booking ID: " << bookings[i].bookingID
                 << " | Flight ID: " << bookings[i].flightID
                 << " | Traveller: " << bookings[i].passengerName
                 << " | Payment: " << bookings[i].paymentMethod
                 << " | Status: " << bookings[i].status;

            if (f) {
                cout << " | " << f->from << " -> " << f->to
                     << " | Date: " << f->date
                     << " | Dep: " << f->departureTime
                     << " | Arr: " << f->arrivalTime
                     << " | Price: Rs " << f->price;
            } else {
                cout << " | (Flight info not found)";
            }
            cout << "\n";
        }
    }
    if (!found) cout << "You have no history.\n";
}

void cancelBooking(const string &pName, const string &pPhone) {
    viewMyFlightHistory(pName, pPhone);

    int bookingID = getValidInt("Enter Booking ID to cancel (0 to go back): ");
    if (bookingID == 0) return;

    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].accountName == pName && bookings[i].phone == pPhone &&
            bookings[i].bookingID == bookingID && bookings[i].status == "ACTIVE") {

            bookings[i].status = "CANCELLED";

            Flight* f = findFlightByID(bookings[i].flightID);
            if (f) f->seats++;

            saveBookingsToFile();
            saveFlightsToFile();
            cout << "Booking cancelled successfully! (Saved in history)\n";
            return;
        }
    }
    cout << "No ACTIVE booking found with this Booking ID.\n";
}

// -------------------- ADMIN FUNCTIONS --------------------
void viewFlightsAdmin() {
    cout << "\n--- Flights (Admin View) ---\n";
    if (flightCount == 0) { cout << "No flights available.\n"; return; }
    for (int i = 0; i < flightCount; i++) displayFlight(&flights[i]);
}

void addFlight() {
    if (flightCount >= 50) { cout << "Maximum flights reached!\n"; return; }

    Flight* f = &flights[flightCount];
    f->id = generateUniqueFlightID();
    cout << "\nGenerated Flight ID: " << f->id << "\n";

    clearLine();
    f->from = readLineNonEmpty("From (city/airport): ");
    f->to   = readLineNonEmpty("To (city/airport): ");
    f->seats = getValidInt("Seats: ");
    f->price = getValidInt("Price: ");
    f->date  = getValidDatePrompt("Date (YYYY-MM-DD): ");
    f->departureTime = getValidTimePrompt("Departure Time (HH:MM): ");
    f->arrivalTime   = getValidTimePrompt("Arrival Time (HH:MM): ");

    flightCount++;
    saveFlightsToFile();
    cout << "Flight added successfully!\n";
}

void editFlight() {
    viewFlightsAdmin();
    if (flightCount == 0) return;

    int id = getValidInt("\nEnter Flight ID to edit (0 to go back): ");
    if (id == 0) return;

    Flight* f = findFlightByID(id);
    if (!f) { cout << "Flight not found!\n"; return; }

    int choice;
    do {
        cout << "\n--- Edit Menu (Flight ID: " << f->id << ") ---\n";
        cout << "1. Edit From\n2. Edit To\n3. Edit Seats\n4. Edit Price\n5. Edit Date\n"
             << "6. Edit Departure\n7. Edit Arrival\n8. Back\n";
        choice = getValidChoice("Choice: ", 1, 8);

        switch (choice) {
            case 1: clearLine(); f->from = readLineNonEmpty("New From: "); break;
            case 2: clearLine(); f->to   = readLineNonEmpty("New To: "); break;
            case 3: f->seats = getValidInt("New Seats: "); break;
            case 4: f->price = getValidInt("New Price: "); break;
            case 5: f->date = getValidDatePrompt("New Date (YYYY-MM-DD): "); break;
            case 6: f->departureTime = getValidTimePrompt("New Departure (HH:MM): "); break;
            case 7: f->arrivalTime   = getValidTimePrompt("New Arrival (HH:MM): "); break;
            case 8: break;
        }
    } while (choice != 8);

    saveFlightsToFile();
    cout << "Flight updated successfully!\n";
}

void deleteFlight() {
    viewFlightsAdmin();
    if (flightCount == 0) return;

    int id = getValidInt("\nEnter Flight ID to delete (0 to go back): ");
    if (id == 0) return;

    int index = -1;
    for (int i = 0; i < flightCount; i++) if (flights[i].id == id) { index = i; break; }
    if (index == -1) { cout << "Flight not found!\n"; return; }

    for (int i = index; i < flightCount - 1; i++) flights[i] = flights[i + 1];
    flightCount--;
    saveFlightsToFile();
    cout << "Flight deleted successfully!\n";
}

void viewPassengers() {
    loadPassengers();
    cout << "\n--- Registered Passengers ---\n";
    if (passengerCount == 0) { cout << "No passengers found.\n"; return; }

    for (int i = 0; i < passengerCount; i++) {
        cout << (i + 1) << ". Name: " << passengers[i].name
             << " | Email: " << passengers[i].email
             << " | Phone: " << passengers[i].phone << "\n";
    }
}

void viewAllBookings() {
    cout << "\n--- All Bookings (Active + Cancelled) ---\n";
    if (bookingCount == 0) { cout << "No bookings found.\n"; return; }

    for (int i = 0; i < bookingCount; i++) {
        Flight* f = findFlightByID(bookings[i].flightID);
        cout << "Booking ID: " << bookings[i].bookingID
             << " | Account: " << bookings[i].accountName
             << " | Phone: " << bookings[i].phone
             << " | Traveller: " << bookings[i].passengerName
             << " | FlightID: " << bookings[i].flightID
             << " | Payment: " << bookings[i].paymentMethod
             << " | Status: " << bookings[i].status;
        if (f) cout << " | " << f->from << " -> " << f->to << " | Date: " << f->date;
        else cout << " | (Flight info not found)";
        cout << "\n";
    }
}

// -------------------- MAIN --------------------
int main() {
    srand((unsigned)time(nullptr));

    loadFlights();
    loadBookings();

    while (true) {
        cout << "\n===== WELCOME TO FLIGHT MANAGEMENT SYSTEM =====\n";
        cout << "1. Admin\n2. Passenger\n3. Exit\n";
        int userType = getValidChoice("Choice: ", 1, 3);

        // ADMIN
        if (userType == 1) {
            if (!adminLogin()) continue;

            while (true) {
                cout << "\n--- ADMIN MENU ---\n";
                cout << "1. View Flights\n2. Add Flight (Random ID)\n3. Edit Flight\n4. Delete Flight\n5. View Passengers\n6. View All Bookings\n7. Back\n";
                int c = getValidChoice("Choice: ", 1, 7);
                if (c == 7) break;

                switch (c) {
                    case 1: viewFlightsAdmin(); break;
                    case 2: addFlight(); break;
                    case 3: editFlight(); break;
                    case 4: deleteFlight(); break;
                    case 5: viewPassengers(); break;
                    case 6: viewAllBookings(); break;
                }
            }
        }

        // PASSENGER
        else if (userType == 2) {
            while (true) {
                cout << "\n--- PASSENGER ---\n";
                cout << "1. Login\n2. Signup\n3. Back\n";
                int opt = getValidChoice("Choice: ", 1, 3);
                if (opt == 3) break;

                string name, email, phone;
                bool loggedIn = false;

                if (opt == 1) loggedIn = passengerLogin(name, email, phone);
                else loggedIn = passengerSignupAndAutoLogin(name, email, phone);

                if (!loggedIn) continue;

                while (true) {
                    cout << "\n--- PASSENGER MENU ---\n";
                    cout << "1. View All Flights\n2. Search Flights\n3. Book Ticket\n4. Cancel Booking\n5. View My Flight History\n6. Back\n";
                    int c = getValidChoice("Choice: ", 1, 6);
                    if (c == 6) break;

                    switch (c) {
                        case 1: viewAllFlightsPassenger(); break;
                        case 2: searchFlights(); break;
                        case 3: bookTicket(name, phone); break;
                        case 4: cancelBooking(name, phone); break;
                        case 5: viewMyFlightHistory(name, phone); break;
                    }
                }
            }
        }

        // EXIT
        else {
            cout << "Exiting program...\n";
            break;
        }
    }

    return 0;
}

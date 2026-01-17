#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

// ===================== STRUCTS =====================
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
    string travellerName;
    string status;          // PENDING / ACTIVE / CANCELLED
    string paymentMethod;   // CASH / CARD / ONLINE
};

struct Passenger {
    string name;
    string email;
    string phone;
    string password;
};

// ===================== GLOBAL ARRAYS =====================
Flight flights[50];     int flightCount = 0;
Booking bookings[50];   int bookingCount = 0;
Passenger passengers[50];int passengerCount = 0;

// ===================== CONSTANTS =====================
const string ACTIVE="ACTIVE", CANCELLED="CANCELLED", PENDING="PENDING";
const string CASH="CASH", CARD="CARD", ONLINE="ONLINE";

// ===================== SMALL HELPERS =====================
void clearLine() { cin.ignore(numeric_limits<streamsize>::max(), '\n'); }

bool isDigitsOnly(const string &s){
    if (s.empty()) return false;
    for(char c: s) if(c<'0' || c>'9') return false;
    return true;
}

bool isEmailValidBasic(const string &email){
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    return (atPos != string::npos && dotPos != string::npos &&
            atPos < dotPos && atPos > 0 && dotPos < email.size()-1);
}

bool isValidPKPhone(const string &phone){
    return phone.size()==11 && isDigitsOnly(phone) && phone[0]=='0' && phone[1]=='3';
}

bool isLeapYear(int y){
    return (y%400==0) || (y%4==0 && y%100!=0);
}

bool isValidDate(const string &d){
    if(d.size()!=10 || d[4]!='-' || d[7]!='-') return false;
    string ys=d.substr(0,4), ms=d.substr(5,2), ds=d.substr(8,2);
    if(!isDigitsOnly(ys)||!isDigitsOnly(ms)||!isDigitsOnly(ds)) return false;
    int y=stoi(ys), m=stoi(ms), day=stoi(ds);
    if(y<2000 || y>2100) return false;
    if(m<1 || m>12) return false;
    int daysInMonth[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    int maxDay=daysInMonth[m];
    if(m==2 && isLeapYear(y)) maxDay=29;
    return (day>=1 && day<=maxDay);
}

bool isValidTimeHHMM(const string &t){
    if(t.size()!=5 || t[2]!=':') return false;
    string hs=t.substr(0,2), ms=t.substr(3,2);
    if(!isDigitsOnly(hs)||!isDigitsOnly(ms)) return false;
    int h=stoi(hs), m=stoi(ms);
    return (h>=0 && h<=23 && m>=0 && m<=59);
}

int getValidInt(const string &msg){
    int num;
    while(true){
        cout<<msg;
        if(cin>>num) return num;
        cout<<"Invalid input! Enter a number.\n";
        cin.clear(); cin.ignore(1000,'\n');
    }
}

int getValidChoice(const string &msg,int mn,int mx){
    int c;
    while(true){
        cout<<msg;
        if(cin>>c){
            if(c>=mn && c<=mx) return c;
            cout<<"Invalid choice! ("<<mn<<"-"<<mx<<")\n";
        }else{
            cout<<"Invalid input! Enter a number.\n";
            cin.clear(); cin.ignore(1000,'\n');
        }
    }
}

// ===================== REQUIRED MINIMIZERS =====================

int runMenu(const string &title, const string options[], int n){
    cout << "\n--- " << title << " ---\n";
    for(int i=0;i<n;i++) cout << (i+1) << ". " << options[i] << "\n";
    return getValidChoice("Choice: ", 1, n);
}

string promptUntil(const string &msg, bool (*valid)(const string&), const string &error){
    string x;
    while(true){
        cout << msg;
        cin >> x;
        if(x=="0") return "0";
        if(valid(x)) return x;
        cout << error << "\n";
    }
}

void printBooking(const Booking &b){
    Flight *f = nullptr;
    for(int i=0;i<flightCount;i++) if(flights[i].id==b.flightID){ f=&flights[i]; break; }

    cout << "BookingID: " << b.bookingID
         << " | Account: " << b.accountName
         << " | Phone: " << b.phone
         << " | FlightID: " << b.flightID
         << " | Traveller: " << b.travellerName
         << " | Payment: " << b.paymentMethod
         << " | Status: " << b.status;

    if(f){
        cout << " | " << f->from << " -> " << f->to
             << " | Date: " << f->date
             << " | Dep: " << f->departureTime
             << " | Arr: " << f->arrivalTime
             << " | Price: Rs " << f->price;
    }
    cout << "\n";
}

void displayFlight(const Flight &f){
    cout << "ID: " << f.id 
         << " | " << f.from << " -> " << f.to
         << " | Seats Available: " << f.seats
         << " | Price: Rs " << f.price
         << " | Date: " << f.date
         << " | Dep: " << f.departureTime
         << " | Arr: " << f.arrivalTime << "\n";
}


void viewAllFlights(){
    cout << "\n--- Available Flights ---\n";
    if(flightCount==0){ cout<<"No flights available.\n"; return; }
    for(int i=0;i<flightCount;i++) displayFlight(flights[i]);
}

Flight* findFlightByID(int id){
    for(int i=0;i<flightCount;i++) if(flights[i].id==id) return &flights[i];
    return nullptr;
}

bool bookingIDExists(int id){
    for(int i=0;i<bookingCount;i++) if(bookings[i].bookingID==id) return true;
    return false;
}
int generateUniqueBookingID(){
    int id;
    do{ id=100000 + (rand()%900000); }while(bookingIDExists(id));
    return id;
}

bool flightIDExists(int id){
    for(int i=0;i<flightCount;i++) if(flights[i].id==id) return true;
    return false;
}
int generateUniqueFlightID(){
    int id;
    do{ id=100 + (rand()%900); }while(flightIDExists(id));
    return id;
}

bool checkActiveBookingExists(const string &name,const string &phone,int flightID){
    for(int i=0;i<bookingCount;i++){
        if(bookings[i].accountName==name &&
           bookings[i].phone==phone &&
           bookings[i].flightID==flightID &&
           bookings[i].status==ACTIVE) return true;
    }
    return false;
}

// ===================== FILE OPS =====================
void loadFlights(){
    ifstream file("flights.txt");
    flightCount=0;
    string line;
    while(getline(file,line)){
        if(line.empty() || flightCount>=50) continue;
        string parts[8], temp; int pc=0;
        for(char c: line){
            if(c=='|'){ if(pc<8) parts[pc++]=temp; temp.clear(); }
            else temp.push_back(c);
        }
        if(pc<7) continue;
        parts[pc++]=temp;
        if(pc!=8) continue;

        flights[flightCount].id=stoi(parts[0]);
        flights[flightCount].from=parts[1];
        flights[flightCount].to=parts[2];
        flights[flightCount].seats=stoi(parts[3]);
        flights[flightCount].price=stoi(parts[4]);
        flights[flightCount].date=parts[5];
        flights[flightCount].departureTime=parts[6];
        flights[flightCount].arrivalTime=parts[7];
        flightCount++;
    }
}

void saveFlights(){
    ofstream file("flights.txt");
    for(int i=0;i<flightCount;i++){
        file<<flights[i].id<<"|"<<flights[i].from<<"|"<<flights[i].to<<"|"
            <<flights[i].seats<<"|"<<flights[i].price<<"|"<<flights[i].date<<"|"
            <<flights[i].departureTime<<"|"<<flights[i].arrivalTime<<"\n";
    }
}

void loadBookings(){
    ifstream file("bookings.txt");
    bookingCount=0;
    string line;
    while(getline(file,line)){
        if(line.empty() || bookingCount>=50) continue;
        string parts[7], temp; int pc=0;
        for(char c: line){
            if(c=='|'){ if(pc<7) parts[pc++]=temp; temp.clear(); }
            else temp.push_back(c);
        }
        if(pc<6) continue;
        parts[pc++]=temp;
        if(pc!=7) continue;

        bookings[bookingCount].bookingID=stoi(parts[0]);
        bookings[bookingCount].accountName=parts[1];
        bookings[bookingCount].phone=parts[2];
        bookings[bookingCount].flightID=stoi(parts[3]);
        bookings[bookingCount].travellerName=parts[4];
        bookings[bookingCount].status=parts[5];
        bookings[bookingCount].paymentMethod=parts[6];
        bookingCount++;
    }
}

void saveBookings(){
    ofstream file("bookings.txt");
    for(int i=0;i<bookingCount;i++){
        file<<bookings[i].bookingID<<"|"<<bookings[i].accountName<<"|"<<bookings[i].phone<<"|"
            <<bookings[i].flightID<<"|"<<bookings[i].travellerName<<"|"<<bookings[i].status<<"|"
            <<bookings[i].paymentMethod<<"\n";
    }
}

void loadPassengers(){
    ifstream file("passengers.txt");
    passengerCount=0;
    string line;
    while(getline(file,line)){
        if(line.empty() || passengerCount>=50) continue;
        string parts[4], temp; int pc=0;
        for(char c: line){
            if(c=='|'){ if(pc<4) parts[pc++]=temp; temp.clear(); }
            else temp.push_back(c);
        }
        if(pc<3) continue;
        parts[pc++]=temp;
        if(pc!=4) continue;

        passengers[passengerCount].name=parts[0];
        passengers[passengerCount].email=parts[1];
        passengers[passengerCount].phone=parts[2];
        passengers[passengerCount].password=parts[3];
        passengerCount++;
    }
}

bool emailExists(const string &email){
    loadPassengers();
    for(int i=0;i<passengerCount;i++) if(passengers[i].email==email) return true;
    return false;
}

// ===================== ADMIN FUNCTIONS =====================
bool adminLogin(){
    const string storedUser="admin", storedPass="123";
    const string opts[]={"Login","Back"};
    while(true){
        int c=runMenu("Admin Login", opts, 2);
        if(c==2) return false;

        string u,p;
        cout<<"Username: "; cin>>u;
        cout<<"Password: "; cin>>p;
        if(u==storedUser && p==storedPass){ cout<<"\nLogin Successful!\n"; return true; }
        cout<<"Wrong username or password!\n";
    }
}

void addFlight(){
    if(flightCount>=50){ cout<<"Maximum flights reached!\n"; return; }

    Flight f;
    f.id=generateUniqueFlightID();
    cout<<"\nGenerated Flight ID: "<<f.id<<"\n";

    clearLine();
    cout<<"From (single word or use _ for spaces): "; getline(cin, f.from);
    cout<<"To   (single word or use _ for spaces): "; getline(cin, f.to);

    f.seats=getValidInt("Seats: ");
    f.price=getValidInt("Price: ");
    f.date=promptUntil("Date (YYYY-MM-DD) (0 back): ", isValidDate, "Invalid date!");
    if(f.date=="0") return;

    f.departureTime=promptUntil("Departure (HH:MM) (0 back): ", isValidTimeHHMM, "Invalid time!");
    if(f.departureTime=="0") return;

    f.arrivalTime=promptUntil("Arrival   (HH:MM) (0 back): ", isValidTimeHHMM, "Invalid time!");
    if(f.arrivalTime=="0") return;

    flights[flightCount++]=f;
    saveFlights();
    cout<<"Flight added successfully!\n";
}

void editFlight(){
    viewAllFlights();
    if(flightCount==0) return;

    int id=getValidInt("Enter Flight ID to edit (0 back): ");
    if(id==0) return;

    Flight* f=findFlightByID(id);
    if(!f){ cout<<"Flight not found!\n"; return; }

    const string opts[]={
        "Edit From","Edit To","Edit Seats","Edit Price","Edit Date",
        "Edit Departure","Edit Arrival","Back"
    };

    while(true){
        int c=runMenu("Edit Flight", opts, 8);
        if(c==8) break;

        switch(c){
            case 1: clearLine(); cout<<"New From: "; getline(cin,f->from); break;
            case 2: clearLine(); cout<<"New To: "; getline(cin,f->to); break;
            case 3: f->seats=getValidInt("New Seats: "); break;
            case 4: f->price=getValidInt("New Price: "); break;
            case 5:{
                string d=promptUntil("New Date (YYYY-MM-DD) (0 back): ", isValidDate, "Invalid date!");
                if(d!="0") f->date=d;
                break;
            }
            case 6:{
                string t=promptUntil("New Departure (HH:MM) (0 back): ", isValidTimeHHMM, "Invalid time!");
                if(t!="0") f->departureTime=t;
                break;
            }
            case 7:{
                string t=promptUntil("New Arrival (HH:MM) (0 back): ", isValidTimeHHMM, "Invalid time!");
                if(t!="0") f->arrivalTime=t;
                break;
            }
        }
        saveFlights();
        cout<<"Updated!\n";
    }
}

void deleteFlight(){
    viewAllFlights();
    if(flightCount==0) return;

    int id=getValidInt("Enter Flight ID to delete (0 back): ");
    if(id==0) return;

    int idx=-1;
    for(int i=0;i<flightCount;i++) if(flights[i].id==id){ idx=i; break; }
    if(idx==-1){ cout<<"Flight not found!\n"; return; }

    for(int i=idx;i<flightCount-1;i++) flights[i]=flights[i+1];
    flightCount--;
    saveFlights();
    cout<<"Flight deleted successfully!\n";
}

void viewPassengers(){
    loadPassengers();
    cout<<"\n--- Registered Passengers ---\n";
    if(passengerCount==0){ cout<<"No passengers found.\n"; return; }
    for(int i=0;i<passengerCount;i++){
        cout<<(i+1)<<". "<<passengers[i].name
            <<" | "<<passengers[i].email
            <<" | "<<passengers[i].phone<<"\n";
    }
}

void editPassenger(){
    loadPassengers();
    if(passengerCount==0){ cout<<"No passengers available.\n"; return; }

    for(int i=0;i<passengerCount;i++){
        cout<<(i+1)<<". "<<passengers[i].name<<" | "<<passengers[i].email<<"\n";
    }

    int idx = getValidChoice("Select passenger to edit: ", 1, passengerCount) - 1;
    clearLine();

    cout<<"New Name: "; getline(cin, passengers[idx].name);
    passengers[idx].phone = promptUntil("New Phone (03XXXXXXXXX): ", isValidPKPhone, "Invalid phone!");
    cout<<"New Password: "; cin>>passengers[idx].password;

    ofstream file("passengers.txt");
    for(int i=0;i<passengerCount;i++){
        file<<passengers[i].name<<"|"<<passengers[i].email<<"|"<<passengers[i].phone<<"|"<<passengers[i].password<<"\n";
    }
    cout<<"Passenger updated successfully!\n";
}

void viewAllBookings(){
    cout<<"\n--- All Bookings (Active + Cancelled + Pending) ---\n";
    if(bookingCount==0){ cout<<"No bookings found.\n"; return; }
    for(int i=0;i<bookingCount;i++) printBooking(bookings[i]);
}

void adminSearchFlights(){
    clearLine();
    string from, to, date;

    cout << "\nFrom: ";
    getline(cin, from);
    cout << "To: ";
    getline(cin, to);

    date = promptUntil("Date (YYYY-MM-DD) (0 back): ", isValidDate, "Invalid date!");
    if(date == "0") return;

    bool found = false;
    cout << "\n--- Admin Flight Search Results ---\n";
    for(int i = 0; i < flightCount; i++){
        if(flights[i].from == from &&
           flights[i].to == to &&
           flights[i].date == date){
            displayFlight(flights[i]);
            found = true;
        }
    }
    if(!found) cout << "No flights found.\n";
}

void approvePendingBookings(){
    cout << "\n--- Pending Bookings ---\n";
    bool found = false;
    for(int i=0;i<bookingCount;i++){
        if(bookings[i].status==PENDING){
            printBooking(bookings[i]);
            found = true;
        }
    }
    if(!found){ cout<<"No pending bookings.\n"; return; }

    int bid = getValidInt("Enter Booking ID to approve (0 back): ");
    if(bid==0) return;

    for(int i=0;i<bookingCount;i++){
        if(bookings[i].bookingID==bid && bookings[i].status==PENDING){
            Flight *f = findFlightByID(bookings[i].flightID);
            if(!f || f->seats<=0){
                cout<<"Cannot approve. No seats available.\n";
                return;
            }

            bookings[i].status = ACTIVE;
            f->seats--;
            saveBookings();
            saveFlights();
            cout<<"Booking approved successfully!\n";
            return;
        }
    }
    cout<<"Pending booking not found.\n";
}

// ===================== PASSENGER FUNCTIONS =====================
bool passengerSignupAndAutoLogin(string &name,string &email,string &phone){
    clearLine();
    cout<<"\n--- Passenger Signup ---\n";
    cout<<"Full Name: "; getline(cin, name);

    while(true){
        cout<<"Email (0 back): ";
        cin>>email;
        if(email=="0") return false;
        if(!isEmailValidBasic(email)){ cout<<"Invalid email format!\n"; continue; }
        if(emailExists(email)){ cout<<"Email already exists!\n"; continue; }
        break;
    }

    phone = promptUntil("Phone 03XXXXXXXXX (0 back): ", isValidPKPhone, "Invalid phone!");
    if(phone=="0") return false;

    string pass;
    cout<<"Password: "; cin>>pass;

    ofstream file("passengers.txt", ios::app);
    file<<name<<"|"<<email<<"|"<<phone<<"|"<<pass<<"\n";

    cout<<"\nSignup Successful! Redirecting to Passenger Home...\n";
    return true;
}

bool passengerLogin(string &name,string &email,string &phone){
    loadPassengers();
    if(passengerCount==0){ cout<<"No passengers found. Please signup first.\n"; return false; }

    while(true){
        cout<<"\n--- Passenger Login ---\n";
        cout<<"Email (0 back): "; cin>>email;
        if(email=="0") return false;

        string pass;
        cout<<"Password: "; cin>>pass;

        bool found=false;
        for(int i=0;i<passengerCount;i++){
            if(passengers[i].email==email){
                found=true;
                if(passengers[i].password==pass){
                    name=passengers[i].name;
                    phone=passengers[i].phone;
                    cout<<"\nLogin Successful!\n";
                    return true;
                }else{
                    cout<<"Wrong password!\n";
                }
                break;
            }
        }
        if(!found) cout<<"User not found!\n";
    }
}

int choosePaymentMethod(string &methodOut){
    const string opts[]={"Cash","Card","Online Payment","Back"};
    int c=runMenu("Payment Method", opts, 4);
    if(c==4) return 0;
    methodOut = (c==1?CASH:(c==2?CARD:ONLINE));
    return 1;
}

bool processPayment(const string &method,int amountRs){
    cout<<"\nAmount to Pay: Rs "<<amountRs<<"\n";

    if(method==CASH){
        int paid;
        while(true){
            paid = getValidInt("Enter cash amount: ");
            if(paid < amountRs){
                cout << "Insufficient cash. Please enter at least Rs " << amountRs << "\n";
            }else{
                cout << "Payment successful. Change: Rs " << (paid - amountRs) << "\n";
                return true;
            }
        }
    }

    if(method==CARD){
        string cardNo,pin;
        cout<<"Card Number (16 digits): "; cin>>cardNo;
        cout<<"PIN (4 digits): "; cin>>pin;
        if(cardNo.size()!=16 || !isDigitsOnly(cardNo) || pin.size()!=4 || !isDigitsOnly(pin)){
            cout<<"Payment failed (invalid card details).\n"; return false;
        }
        cout<<"Payment successful via CARD.\n";
        return true;
    }

    if(method==ONLINE){
        string txn,otp;
        cout<<"Transaction ID: "; cin>>txn;
        cout<<"OTP (4 digits): "; cin>>otp;
        if(txn.size()<4 || otp.size()!=4 || !isDigitsOnly(otp)){
            cout<<"Payment failed (invalid online details).\n"; return false;
        }
        cout<<"Payment successful via ONLINE.\n";
        return true;
    }
    return false;
}

void searchFlights(){
    clearLine();
    string from,to;
    cout<<"\nFrom: "; getline(cin, from);
    cout<<"To: "; getline(cin, to);

    string date = promptUntil("Date (YYYY-MM-DD) (0 back): ", isValidDate, "Invalid date!");
    if(date=="0") return;

    bool found=false;
    cout<<"\n--- Search Results ---\n";
    for(int i=0;i<flightCount;i++){
        if(flights[i].from==from && flights[i].to==to && flights[i].date==date){
            displayFlight(flights[i]);
            found=true;
        }
    }
    if(!found) cout<<"No flights found.\n";
}

int selectFlightIDFromList(){
    viewAllFlights();
    if(flightCount==0) return 0;

    int id=getValidInt("Enter Flight ID to book (0 back): ");
    if(id==0) return 0;

    if(!findFlightByID(id)){ cout<<"Invalid Flight ID!\n"; return -1; }
    return id;
}

string chooseTraveller(const string &accountName){
    const string opts[]={"Myself","Someone Else"};
    int c=runMenu("Who will be travelling?", opts, 2);
    if(c==1) return accountName;

    clearLine();
    string t;
    cout<<"Traveller Full Name: ";
    getline(cin, t);
    if(t.empty()) return accountName; // fallback
    return t;
}

void bookTicket(const string &pName,const string &pPhone){
    int id=selectFlightIDFromList();
    if(id==0 || id==-1) return;

    Flight* f=findFlightByID(id);
    if(!f) return;

    if(f->seats<=0){ cout<<"No seats available!\n"; return; }
    if(checkActiveBookingExists(pName,pPhone,id)){ cout<<"You already booked this flight (ACTIVE).\n"; return; }
    if(bookingCount>=50){ cout<<"Maximum bookings reached!\n"; return; }

    string traveller = chooseTraveller(pName);

    string method;
    if(!choosePaymentMethod(method)){ cout<<"Booking cancelled (Back).\n"; return; }
    if(!processPayment(method, f->price)){ cout<<"Booking cancelled because payment failed.\n"; return; }

    int bid=generateUniqueBookingID();
    bookings[bookingCount++] = {bid, pName, pPhone, id, traveller, PENDING, method};
    saveBookings();

    cout<<"\nBOOKING CREATED AND PENDING ADMIN APPROVAL!\n";
    printBooking(bookings[bookingCount-1]);
}

void viewMyFlightHistory(const string &pName,const string &pPhone){
    cout<<"\n--- Your Flight History (Active + Cancelled + Pending) ---\n";
    bool found=false;
    for(int i=0;i<bookingCount;i++){
        if(bookings[i].accountName==pName && bookings[i].phone==pPhone){
            printBooking(bookings[i]);
            found=true;
        }
    }
    if(!found) cout<<"No history.\n";
}

void cancelBooking(const string &pName,const string &pPhone){
    viewMyFlightHistory(pName,pPhone);
    int bid=getValidInt("Enter Booking ID to cancel (0 back): ");
    if(bid==0) return;

    for(int i=0;i<bookingCount;i++){
        if(bookings[i].accountName==pName && bookings[i].phone==pPhone &&
           bookings[i].bookingID==bid && bookings[i].status==ACTIVE){

            bookings[i].status=CANCELLED;
            Flight* f=findFlightByID(bookings[i].flightID);
            if(f) f->seats++;

            saveBookings();
            saveFlights();
            cout<<"Booking cancelled successfully!\n";
            return;
        }
    }
    cout<<"No ACTIVE booking found with this Booking ID.\n";
}

// ===================== MAIN =====================
int main(){
    srand((unsigned)time(nullptr));
    loadFlights();
    loadBookings();

    const string mainOpts[]={"Admin","Passenger","Exit"};

    while(true){
        int userType = runMenu("WELCOME TO FLIGHT MANAGEMENT SYSTEM", mainOpts, 3);

        if(userType==1){
            if(!adminLogin()) continue;

            const string adminOpts[]={
                "View Flights","Search Flights","Add Flight (Random ID)","Edit Flight","Delete Flight",
                "View Passengers","Edit Passenger","View All Bookings","Approve Pending Bookings","Back"
            };

            while(true){
                int c=runMenu("ADMIN MENU", adminOpts, 10);
                if(c==10) break;
                if(c==1) viewAllFlights();
                else if(c==2) adminSearchFlights();
                else if(c==3) addFlight();
                else if(c==4) editFlight();
                else if(c==5) deleteFlight();
                else if(c==6) viewPassengers();
                else if(c==7) editPassenger();
                else if(c==8) viewAllBookings();
                else if(c==9) approvePendingBookings();
            }
        }
        else if(userType==2){
            const string passEntry[]={"Login","Signup","Back"};
            while(true){
                int opt = runMenu("PASSENGER", passEntry, 3);
                if(opt==3) break;

                string name,email,phone;
                bool loggedIn = (opt==1) ? passengerLogin(name,email,phone)
                                        : passengerSignupAndAutoLogin(name,email,phone);
                if(!loggedIn) continue;

                const string passOpts[]={
                    "View All Flights","Search Flights","Book Ticket","Cancel Booking",
                    "View My Flight History","Back"
                };

                while(true){
                    int c=runMenu("PASSENGER MENU", passOpts, 6);
                    if(c==6) break;
                    if(c==1) viewAllFlights();
                    else if(c==2) searchFlights();
                    else if(c==3) bookTicket(name,phone);
                    else if(c==4) cancelBooking(name,phone);
                    else if(c==5) viewMyFlightHistory(name,phone);
                }
            }
        }
        else{
            cout<<"Exiting program...\n";
            break;
        }
    }
    return 0;
}

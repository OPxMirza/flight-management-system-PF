
# ✈️ Flight Management System (C++)

A **console-based Flight Management System** developed in **C++** as an academic project.  
This version is a **refactored and minimized implementation** that provides the **same full functionality** as the original code, but with **cleaner structure, reusable functions, and reduced repetition**.

🔗 **GitHub Repository**  
https://github.com/OPxMirza/flight-management-system-cpp

---

## 📌 Key Highlights

- Same features as the original long version
- Reduced code duplication
- Reusable menu, validation, and printing helpers
- Fully menu-driven with Back / Exit options everywhere
- File-based persistence using `|` delimiter
- Suitable for **PF / Programming Fundamentals** courses

---

## 👨‍💼 Admin Features

- Secure admin login
- View all flights
- Add flights with **random unique Flight ID**
- Edit flight details:
  - From
  - To
  - Seats
  - Price
  - Date
  - Departure & Arrival time
- Delete flights
- View all registered passengers
- View all bookings (ACTIVE + CANCELLED)
- Back option available in every menu

---

## 🧑‍✈️ Passenger Features

### Authentication
- Signup with:
  - Full Name (supports spaces)
  - Email (validated & unique)
  - Phone number (Pakistan format `03XXXXXXXXX`)
  - Password
- Email-based login
- Signup automatically logs the user in and opens Passenger Home
- Exit/Back option during login

### Flights
- View all available flights
- Search flights by:
  - From
  - To
  - Date (validated real calendar date)

### Booking
- Book ticket by selecting Flight ID
- Choose **who will be travelling**:
  - Myself
  - Someone else
- Choose payment method:
  - Cash
  - Card
  - Online Payment
- Payment validation for each method
- Random **6-digit Booking ID**
- Seat count decreases after successful booking

### Booking Management
- View complete flight history (ACTIVE + CANCELLED)
- Cancel booking using **Booking ID**
- Cancelled bookings remain in history
- Seats increase back on cancellation

---

## 🛠️ Technical Concepts Used

- C++
- File handling (`fstream`)
- Arrays & pointers
- Input validation
- Menu-driven programming
- Random ID generation
- Function reuse and refactoring
- Delimiter-based file storage

---

## 🧠 Code Minimization Strategy

The code was minimized using:

### ✅ `runMenu()`
- Handles all menu printing and input
- Removes repeated menu logic

### ✅ `promptUntil()`
- Generic input + validation function
- Replaces multiple date/phone/time prompt functions

### ✅ `printBooking()`
- Single booking display function
- Used by both Admin and Passenger views

⚠️ No `split()` function was used, to keep the code simple and PF-friendly.

---

## 📁 Project Structure

```text
flight-management-system-cpp/
│
├── flight.cpp
├── flights.txt
├── passengers.txt
├── bookings.txt
├── README.md
└── .gitignore
````

---

## 📄 File Formats (Pipe `|` Delimiter)

### ✈️ `flights.txt`

```
flightID|from|to|seats|price|date|departureTime|arrivalTime
```

Example:

```
312|Karachi|Lahore|20|18000|2026-01-10|09:00|10:30
```

---

### 👤 `passengers.txt`

```
name|email|phone|password
```

Example:

```
Ali Raza|ali@gmail.com|03001234567|ali123
```

---

### 🎫 `bookings.txt`

```
bookingID|accountName|phone|flightID|travellerName|status|paymentMethod
```

Example:

```
345901|Ali Raza|03001234567|312|Ali Raza|ACTIVE|CARD
```

---

## 💳 Payment Methods

* **Cash**

  * Validates paid amount
  * Calculates change
* **Card**

  * 16-digit card number
  * 4-digit PIN
* **Online Payment**

  * Transaction ID
  * 4-digit OTP

---

## ▶️ How to Compile & Run

### Compile

```bash
g++ flight.cpp -o flight
```

### Run (macOS / Linux)

```bash
./flight
```

### Run (Windows)

```bash
g++ flight.cpp -o flight.exe
flight.exe
```

---

## 🔐 Default Admin Credentials

* **Username:** `admin`
* **Password:** `123`

---

## 🚀 Future Improvements

* Password hashing
* Database integration (MySQL / SQLite)
* Seat selection
* Case-insensitive search
* GUI or Web-based version

---

## 👨‍💻 Author

**OPxMirza**
GitHub: [https://github.com/OPxMirza](https://github.com/OPxMirza)

---

## 📜 License

This project is intended for **educational purposes**.
You are free to use, modify, and extend it with proper credit.


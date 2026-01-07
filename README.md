
# ✈️ Flight Management System (C++)

A **console-based Flight Management System** developed in **C++**, designed as an academic / learning project.  
The system supports **Admin** and **Passenger** roles with full booking, payment, validation, and file-based persistence.

This project demonstrates strong use of **file handling, pointers, input validation, menu-driven design, and real-world logic**.

🔗 **GitHub Repository**  
https://github.com/OPxMirza/flight-management-system-cpp

---

## 📌 Key Features

### 👨‍💼 Admin
- Secure admin login
- View all flights
- Add flights with **random unique Flight ID**
- Edit flights (city, seats, price, date, time)
- Delete flights
- View all registered passengers
- View all bookings (ACTIVE + CANCELLED)

---

### 🧑‍✈️ Passenger
- Signup with:
  - Full Name (supports spaces)
  - Email (unique, validated)
  - Phone number (Pakistan format: `03XXXXXXXXX`)
  - Password
- Email-based login
- Automatically redirected to **Passenger Home** after login
- View all available flights
- Search flights by **From, To, Date**
- Book ticket with:
  - Random **6-digit Booking ID**
  - Traveller name (supports spaces)
  - Payment method:
    - Cash
    - Card
    - Online Payment
- Payment validation for each method
- Cancel booking (status changes to `CANCELLED`)
- View complete **Flight History** (ACTIVE + CANCELLED)
- Exit / Back option available at every menu

---

## 🛠️ Technologies Used
- **C++**
- File Handling (`fstream`)
- Pointers & Arrays
- Random ID generation
- Input validation (email, phone, date, time)
- Menu-driven console UI

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

## 📄 Data File Formats

All files use the **`|` (pipe) delimiter** to support spaces in names and cities.

---

### ✈️ `flights.txt`

**Format**

```
flightID|from|to|seats|price|date|departureTime|arrivalTime
```

**Example**

```
312|Karachi|Lahore|20|18000|2026-01-10|09:00|10:30
478|Karachi|Islamabad|15|22000|2026-01-10|11:00|12:50
```

---

### 👤 `passengers.txt`

**Format**

```
name|email|phone|password
```

**Example**

```
Ali Raza|ali@gmail.com|03001234567|ali123
Sara Khan|sara@gmail.com|03124567890|sara123
```

---

### 🎫 `bookings.txt`

**Format**

```
bookingID|accountName|phone|flightID|passengerName|status|paymentMethod
```

**Example**

```
345901|Ali Raza|03001234567|312|Ali Raza|ACTIVE|CARD
221004|Ali Raza|03001234567|478|Ali Raza|CANCELLED|CASH
```

---

## 💳 Payment Methods

* **Cash**

  * Validates paid amount
* **Card**

  * 16-digit card number
  * 4-digit PIN
* **Online Payment**

  * Transaction ID
  * 4-digit OTP

---

## 📅 Validations Implemented

* Email format validation
* Unique email check on signup
* Phone number validation (`03XXXXXXXXX`)
* Real date validation (`YYYY-MM-DD`)
* Time validation (`HH:MM`, 24-hour)
* Seat availability check
* Duplicate active booking prevention

---

## ▶️ How to Run

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

* Password encryption
* Database integration (MySQL / SQLite)
* Seat selection
* GUI or Web version
* Role-based access control
* Report generation

---

## 👨‍💻 Author

**OPxMirza**
GitHub: [https://github.com/OPxMirza](https://github.com/OPxMirza)

---

## 📜 License

This project is for **educational and academic purposes**.
You are free to use, modify, and improve it with proper credit.

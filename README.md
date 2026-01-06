
# ✈️ Flight Management System (C++ Console Application)

A complete **console-based Flight Management System** developed in **C++**, designed to demonstrate real-world programming concepts such as **file handling, pointers, authentication, booking workflows, and payment simulation**.

This project supports **Admin** and **Passenger** roles and maintains persistent data using text files.

🔗 **GitHub Repository:**  
https://github.com/OPxMirza/flight-management-system-cpp

---

## 📌 Features

### 👨‍💼 Admin
- Secure admin login
- View all available flights
- Add new flights
- Flight data stored persistently in `flights.txt`

---

### 🧑‍✈️ Passenger
- Signup using **Name, Email, Phone, Password**
- Email-based login (validates if user exists)
- Search flights by **From, To, Date**
- Book flight tickets with:
  - 🎟️ **Random unique Booking ID**
  - 💳 **Payment method selection**:
    - Cash  
    - Card  
    - EasyPaisa  
    - JazzCash
- Cancel bookings (status changes to **CANCELLED**, history preserved)
- View complete **Flight History** (ACTIVE + CANCELLED)
- All data stored using file handling

---

## 🛠️ Technologies Used
- **C++**
- **File Handling (`fstream`)**
- **Pointers & Arrays**
- **Random Number Generation**
- **Menu-driven Console UI**

---

## 📁 Project Structure

```text
flight-management-system-cpp/
│
├── main.cpp
├── flights.txt
├── passengers.txt
├── bookings.txt
└── README.md
````

---

## 📄 Data File Formats

### ✈️ `flights.txt`

**Format:**

```
id from to seats price date departureTime arrivalTime
```

Example:

```
101 Karachi Lahore 20 18000 2026-01-10 09:00 10:30
102 Karachi Islamabad 15 22000 2026-01-10 11:00 12:50
```

---

### 👤 `passengers.txt`

**Format:**

```
name email phone password
```

Example:

```
Ali ali@gmail.com 03011234567 ali123
Sara sara@gmail.com 03124567890 sara123
```

---

### 🎫 `bookings.txt`

**Format:**

```
bookingID accountName phone flightID passengerName status paymentMethod
```

Example:

```
345901 Ali 03011234567 101 Ali ACTIVE CARD
221004 Ali 03011234567 102 Ali CANCELLED CASH
```

---

## 🔄 Booking Workflow

1. Passenger logs in using **email + password**
2. Searches available flights
3. Selects flight by ID
4. Enters passenger (traveller) name
5. Chooses payment method
6. System validates payment input
7. Random **Booking ID** is generated
8. Seat count updates automatically
9. Booking is saved to file

---

## ❌ Cancellation Logic

* Booking is **not deleted**
* Status changes from `ACTIVE` → `CANCELLED`
* Flight seat count is restored
* Cancelled bookings remain visible in history

---

## ▶️ How to Run

### Compile

```bash
g++ main.cpp -o flight_system
```

### Run (Linux / macOS)

```bash
./flight_system
```

### Run (Windows)

```bash
g++ main.cpp -o flight_system.exe
flight_system.exe
```

---

## 🔐 Default Admin Credentials

* **Username:** admin
* **Password:** 123

---

## 🚀 Future Improvements

* Replace text files with database (MySQL / SQLite)
* Encrypt passwords
* Add seat selection
* Add admin booking management
* Convert to GUI or web-based application

---

## 👨‍💻 Author

**OPxMirza**
GitHub: [https://github.com/OPxMirza](https://github.com/OPxMirza)

---

## 📜 License

This project is intended for **learning and academic purposes**.
You are free to modify and reuse it with proper credit.

```



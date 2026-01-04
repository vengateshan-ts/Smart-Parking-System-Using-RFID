# 🚗 Smart Parking System (ESP32 + RFID + SQL Server)

## 📌 Project Overview
The Smart Parking System is an IoT-based solution that tracks vehicle entry and exit using RFID technology.  
An ESP32 microcontroller reads RFID cards and sends real-time parking data to a Flask server, which securely stores the data in a Microsoft SQL Server database.

This system eliminates manual logging and provides accurate, real-time parking records.

---

## ⚙️ Technologies Used

### Hardware
- ESP32
- RFID Reader (MFRC522)
- RFID Cards
- Buzzer

### Software
- Arduino IDE
- Python (Flask)
- Microsoft SQL Server (SSMS)
- pyodbc
- Wi-Fi (HTTP REST API)

---

## 🔐 Security
- API Key authentication is implemented
- Only authorized ESP32 devices can send data to the server

---

## 🗄️ Database Structure

**Table: `parking_log`**

| Column | Type |
|------|------|
| UID | VARCHAR |
| Name | VARCHAR |
| Age | INT |
| Entry_Time | DATETIME |
| Exit_Time | DATETIME |
| Status | VARCHAR |

---

## 🔁 Working Flow
1. RFID card is tapped
2. ESP32 reads UID
3. Entry or Exit is detected
4. Data is sent to Flask server in real time
5. Server authenticates the request
6. Data is stored in SQL Server instantly

---

## ▶️ How to Run the Project

### 1️⃣ Database Setup
- Create database `SmartParkingDB`
- Create table `dbo.parking_log`

### 2️⃣ Run Server
```bash
pip install -r requirements.txt
python server1.py



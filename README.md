# 🚗 Smart Parking System

An automated, IoT-based smart parking system using **ESP32**, **RFID (MFRC522)**, **Flask (Python)**, and **SQL Server**. This system tracks vehicle entry and exit times automatically using RFID cards and records the logs in a centralized database.

## 🌟 Features

- **IoT Integration**: ESP32 microcontroller handles real-time RFID scanning.
- **Automated Entry/Exit Logging**: Detects entry and exit automatically by toggling user state.
- **RESTful API Backend**: A Python Flask server that receives HTTP POST requests from the ESP32.
- **Database Storage**: Uses Microsoft SQL Server to securely log timestamps, user details, and parking status.
- **Auditory Feedback**: Different buzzer tones for entry and exit events.
- **Time Synchronization**: NTP server synchronization ensures accurate timestamps.

## 📂 Project Structure

```
Smart-Parking-System/
├── backend/
│   ├── server.py       # Flask REST API server for processing ESP32 requests
│   └── database.sql    # SQL script to set up the database and tables
├── esp32/
│   └── esp32.ino       # C++ code for the ESP32 microcontroller
└── README.md           # Project documentation
```

## 🛠️ Hardware Requirements

- **ESP32 Development Board**
- **MFRC522 RFID Reader**
- **Passive Buzzer**
- Jumper Wires & Breadboard
- RFID Cards/Tags

### 🔌 Pin Configuration (ESP32)

| MFRC522 | ESP32 Pin | Buzzer | ESP32 Pin |
|---------|-----------|--------|-----------|
| SDA/SS  | GPIO 5    | Signal | GPIO 27   |
| SCK     | GPIO 18   | VCC    | 3.3V/5V   |
| MOSI    | GPIO 23   | GND    | GND       |
| MISO    | GPIO 19   |        |           |
| RST     | GPIO 22   |        |           |
| 3.3V    | 3.3V      |        |           |
| GND     | GND       |        |           |

## 🚀 Setup & Installation

### 1. Database Setup
1. Ensure **Microsoft SQL Server** is installed (e.g., SQLEXPRESS).
2. Create a database named `SmartParkingDB`.
3. Create the required `parking_log` table by referencing the `backend/database.sql` file layout (make sure the table exists with `UID`, `Name`, `Age`, `Entry_Time`, `Exit_Time`, and `Status`).

### 2. Backend Setup
1. Navigate to the `backend/` directory.
2. Install the required Python packages:
   ```bash
   pip install flask pyodbc
   ```
3. Update the `CONNECTION_STRING` in `server.py` to match your SQL Server instance.
4. Run the Flask server:
   ```bash
   python server.py
   ```
   *Note: Ensure your PC and ESP32 are connected to the same WiFi network, and update the IP address in the ESP32 code.*

### 3. Hardware Setup (ESP32)
1. Open `esp32/esp32.ino` in the Arduino IDE.
2. Install necessary libraries: `MFRC522` by GithubCommunity.
3. Update the following constants in the `.ino` file:
   - `ssid`: Your WiFi network name.
   - `password`: Your WiFi password.
   - `serverURL`: Replace `192.168.0.106` with your PC's local IP address.
4. Flash the code to your ESP32 board.

## 🛡️ Security

- **API Key**: Communication between the ESP32 and the Flask backend is authenticated using a predefined API key (`SMARTPARK_ESP32_KEY_2026`).

## 📸 Project Showcase

Here are some pictures of the live project setup:

### Project Implementation
![Proof 1](proof1.jpeg)
![Proof 2](proof2.jpeg)
![Proof 3](proof3.jpeg)
![Proof 4](proof4.jpeg)
![Proof 5](proof5.jpeg)
![Proof 6](proof6.jpeg)

## 👨‍💻 Contributors

- **Vengateshan T S** & Team

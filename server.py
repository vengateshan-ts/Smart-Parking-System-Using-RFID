from flask import Flask, request, jsonify
import pyodbc

app = Flask(__name__)

# ---------------- API KEY ----------------
API_KEY = "SMARTPARK_ESP32_KEY_2026"

# ---------------- SQL SERVER CONNECTION ----------------
CONNECTION_STRING = (
    "DRIVER={ODBC Driver 17 for SQL Server};"
    "SERVER=MSI\\SQLEXPRESS;"
    "DATABASE=SmartParkingDB;"
    "Trusted_Connection=yes;"
)

# ---------------- RFID ENDPOINT ----------------
@app.route("/rfid", methods=["POST"])
def rfid():
    try:
        data = request.get_json(force=True)
        print("RECEIVED:", data)

        # API key validation
        if data.get("api_key") != API_KEY:
            return jsonify({"error": "Unauthorized"}), 401

        uid = data["uid"]
        name = data["name"]
        age = int(data["age"])
        time_stamp = data["time"]
        event = data["event"]   # ENTRY or EXIT

        print("SQL WRITE:", uid, name, age, time_stamp, event)

        # Connect with autocommit
        conn = pyodbc.connect(CONNECTION_STRING, autocommit=True)
        cursor = conn.cursor()

        if event == "ENTRY":
            cursor.execute("""
                INSERT INTO dbo.parking_log
                (UID, Name, Age, Entry_Time, Status)
                VALUES (?, ?, ?, ?, ?)
            """, uid, name, age, time_stamp, "INSIDE")

        elif event == "EXIT":
            cursor.execute("""
                UPDATE dbo.parking_log
                SET Exit_Time = ?, Status = ?
                WHERE UID = ? AND Status = 'INSIDE'
            """, time_stamp, "OUT", uid)

        cursor.close()
        conn.close()

        return jsonify({"status": "success"}), 200

    except Exception as e:
        print("SERVER ERROR:", e)
        return jsonify({"error": str(e)}), 500


# ---------------- RUN SERVER ----------------
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)





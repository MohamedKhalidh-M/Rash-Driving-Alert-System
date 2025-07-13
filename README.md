# Vehicle Accident and Rash Driving Detection and Alert System using NodeMCU

**Introduction:**
     This project presents a standalone and robust IoT-based accident detection and alert system, using NodeMCU (ESP8266), MPU6050 (Accelerometer + Gyroscope), and the Neo-6M GPS module. Unlike most generic IoT accident detection projects, this design includes fully independent circuit integration, real-time data acquisition, and a SMTP-based email alert system with GPS location.

## 🔧 Components Used

| Component             | Quantity |
|-----------------------|----------|
| NodeMCU ESP8266       | 1        |
| MPU6050 Module        | 1        |
| Neo-6M GPS Module     | 1        |
| 16x2 LCD Display      | 1        |
| 10k Potentiometer     | 1        |
| Breadboard            | 1        |
| Jumper Wires          | As needed |
| 220Ω Resistor         | 1        |

---


**📦 System Overview**
MPU6050 (Accelerometer + Gyroscope)
The MPU6050 sensor combines a 3-axis gyroscope and a 3-axis accelerometer. It detects abrupt changes in acceleration or angular momentum, essential for identifying crashes or rash driving. It communicates with NodeMCU over I2C (D1 = SCL, D2 = SDA).

**Key Features:**

3-Axis Accelerometer and 3-Axis Gyroscope

Selectable sensitivity: ±2g to ±16g, ±250°/s to ±2000°/s

Built-in 16-bit ADC and temperature sensor

**Neo-6M GPS Module**
The Ublox NEO-6M is a high-performance GPS receiver module that provides real-time geographic coordinates in NMEA format.

**Specifications:**

**Interface:** UART (9600 bps default)

**Voltage: ** 3.3V–5V

**Output:** Latitude, Longitude, Time

**NodeMCU ESP8266**
Acts as the central controller. It reads data from the MPU6050 and GPS modules and sends an email alert over WiFi via SMTP protocol when an accident is detected.

**16x2 LCD**
Displays system messages such as:

Driving status (Normal Driving / High Disturbance)

Acceleration values

**📡 How It Works**
**Data Monitoring:** Continuously reads acceleration and gyroscopic data from MPU6050.

**Disturbance Detection:** Compares the change in readings over time. If values exceed a threshold for a fixed duration, it assumes an accident or rash driving scenario.

**Location Fetching:** Retrieves real-time latitude and longitude from the GPS module.

**Email Alert: **Sends an email with a clickable Google Maps link using SMTP protocol.


## 🔌 Circuit Connections

### MPU6050 ↔ NodeMCU

| NodeMCU | MPU6050 |
|---------|---------|
| D1      | SCL     |
| D2      | SDA     |
| 3.3V    | VCC     |
| G       | GND     |

### Neo-6M GPS ↔ NodeMCU

| NodeMCU | GPS Module |
|---------|------------|
| D7 (Rx) | Tx         |
| D8 (Tx) | Rx         |
| 3.3V    | VCC        |
| G       | GND        |

### 16x2 LCD ↔ NodeMCU

| LCD Pin | Connection           |
|---------|----------------------|
| VDD     | 5V                   |
| VSS     | GND                  |
| Vo      | Center pin of 10k pot|
| RS      | D0                   |
| E       | D3                   |
| D4      | D4                   |
| D5      | D5                   |
| D6      | D6                   |
| D7      | D7                   |
| A       | 5V (with 220Ω resistor)|
| K       | GND                  |

---

## 🧠 Working Principle

1. **Sensor Monitoring:** Constantly reads acceleration and gyro data from MPU6050.
2. **Disturbance Detection:** Compares changes in values across X, Y, Z axes to detect high disturbance.
3. **Location Fetching:** Gets GPS coordinates using the TinyGPS++ library.
4. **Email Alert:** Sends a real-time email via SMTP with a clickable location map.

---

## 💻 Code Highlights

### WiFi Credentials
```cpp
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

## ✅ Future Improvements
Add GSM module as backup for areas with no WiFi

Integrate buzzer and LED alert for local warning

Real-time dashboard with location logs

****📌 Notes****
Ensure MPU6050 is calibrated properly

Use App Passwords for Gmail SMTP

Disconnect SCL, SDA, TX, RX while uploading code

****✅ Conclusion****
This project successfully demonstrates a practical and functional Vehicle Accident and Rash Driving Detection and Alert System using NodeMCU (ESP8266), MPU6050, and Neo-6M GPS module. The integration of real-time motion analysis and GPS-based tracking provides an effective mechanism to identify abnormal driving behavior or accidents.

Unlike other implementations that rely heavily on third-party platforms like MQTT or IFTTT, this system operates independently by using the SMTP protocol to send email alerts with live GPS coordinates, ensuring more reliability, security, and customization.

The project can be further enhanced by:

Adding a GSM module for SMS alerts when Wi-Fi is unavailable.

Integrating vehicle ignition control during extreme disturbances.

Logging data to a cloud service or database for analysis and reports.

Enhancing the user interface using mobile apps or dashboards.

Overall, the system is a robust, scalable, and low-cost solution that can contribute to smart transportation systems, driver safety, and emergency response automation.


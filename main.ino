Code:

#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// Wi-Fi credentials
#define WIFI_SSID "Airtel_naje_4589"
#define WIFI_PASSWORD "Airtel@456"

// Email credentials
#define AUTHOR_EMAIL "eceproject74@gmail.com"
#define AUTHOR_PASSWORD "vuna feol ahxt borv"
#define RECIPIENT_EMAIL "abdul7200hameed46@gmail.com"

// SMTP server settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

// I2C settings for MPU6050
const uint8_t MPU6050SlaveAddress = 0x68;
const uint8_t scl = D1;
const uint8_t sda = D2;
const uint16_t AccelScaleFactor = 16384;

TinyGPSPlus gps;
LiquidCrystal lcd(D0, D3, D4, D5, D6, D7);

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;
int dcount = 0;
double MaxValue = 0.2;  // Decreased sensitivity threshold
double MinValue = -0.2; // Decreased sensitivity threshold

unsigned long lastEmailTime = 0; // Timer to control email frequency

WiFiClient client;
SMTPSession smtp;

// Variables for GPS
double latitude, longitude;
String lat_str, lng_str;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // Set up I2C and MPU6050
  Wire.begin(sda, scl);
  MPU6050_Init();
  delay(1500);
  lcd.clear();

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
}

void loop() {
  double Ax, Ay, Az;
  double xvalue, yvalue, zvalue;
  double xvalue1, yvalue1, zvalue1;
  double xvalue2, yvalue2, zvalue2;
  double dx, dy, dz;

  // Read accelerometer values
  Read_RawValue(MPU6050SlaveAddress, 0x3B);
  Ax = (double)AccelX / AccelScaleFactor;
  Ay = (double)AccelY / AccelScaleFactor;
  Az = (double)AccelZ / AccelScaleFactor;
  xvalue = Ax - 1.03;
  yvalue = Ay + 0.06;
  zvalue = Az - 0.07;

  // Store values for comparison
  if (dcount % 2 == 0) {
    xvalue1 = xvalue;
    yvalue1 = yvalue;
    zvalue1 = zvalue;
  } else {
    xvalue2 = xvalue;
    yvalue2 = yvalue;
    zvalue2 = zvalue;

    dx = xvalue2 - xvalue1;
    dy = yvalue2 - yvalue1;
    dz = zvalue2 - zvalue1;

    // Debugging output
    Serial.print("dx: ");
    Serial.print(dx);
    Serial.print(", dy: ");
    Serial.print(dy);
    Serial.print(", dz: ");
    Serial.println(dz);

    // Display values on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Normal Driving");
    lcd.setCursor(0, 1);
    lcd.print(dx);
    lcd.setCursor(6, 1);
    lcd.print(dy);
    lcd.setCursor(11, 1);
    lcd.print(dz);
    delay(500);
            // Get GPS data before sending the email
    gps_123();

    // Display GPS values on the LCD (for debugging)



    // Check for disturbance
    if ((dx < MinValue || dx > MaxValue || dy < MinValue || dy > MaxValue || dz < MinValue || dz > MaxValue) 
        && millis() - lastEmailTime > 12000) {
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("High Disturbance");

      // Get GPS data
      gps_123();

      // Send email
      sendMail();

      // Update last email time
      lastEmailTime = millis();
    }
  }
  dcount++;
}

// Initialize MPU6050
void MPU6050_Init() {
  delay(150);
  I2C_Write(MPU6050SlaveAddress, 0x19, 0x07);  // Sample Rate
  I2C_Write(MPU6050SlaveAddress, 0x6B, 0x01);  // Wake up MPU
  I2C_Write(MPU6050SlaveAddress, 0x6C, 0x00);  // Disable Sleep Mode
  I2C_Write(MPU6050SlaveAddress, 0x1A, 0x00);  // No DLPF
  I2C_Write(MPU6050SlaveAddress, 0x1B, 0x00);  // Gyro full scale +/- 250 degrees/second
  I2C_Write(MPU6050SlaveAddress, 0x1C, 0x00);  // Accelerometer full scale +/- 2g
}

// Write data to MPU6050
void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// Read accelerometer and gyro values
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelY = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelZ = (((int16_t)Wire.read() << 8) | Wire.read());
  Temperature = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroX = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroY = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroZ = (((int16_t)Wire.read() << 8) | Wire.read());
}

// Get GPS data
void gps_123() {
  while (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        lat_str = String(latitude, 6);
        longitude = gps.location.lng();
        lng_str = String(longitude, 6);
      }
    }
  }
}

// Send email alert
void sendMail() {
  SMTP_Message message;
  message.sender.name = "ESP8266";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Disturbance Alert";
  message.addRecipient("Recipient", RECIPIENT_EMAIL);

  // Prepare email content with GPS coordinates
  String messageContent = "Disturbance detected!\n";
  messageContent += "Latitude: " + lat_str + "\nLongitude: " + lng_str;
  messageContent += "\nLocation: http://maps.google.com/maps?q=" + lat_str + "," + lng_str;
  message.text.content = messageContent;

  // Setup email session
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  if (!smtp.connect(&session)) {
    Serial.println("Error connecting to SMTP server");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending email: " + smtp.errorReason());
  } else {
    Serial.println("Email sent successfully!");
  }
}

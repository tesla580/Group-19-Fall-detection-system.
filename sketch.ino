#define BLYNK_TEMPLATE_ID "TMPL2-YUe1Tjn"
#define BLYNK_TEMPLATE_NAME "fall detection"
#define BLYNK_AUTH_TOKEN "OJDbEVc4CV09x4iY0Sgw7eVybOjgP6hz"

#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

MPU6050 mpu;
BlynkTimer timer;

const int buzzer = 4;  
const int ledPin = 5;   

char ssid[] = "Wokwi-GUEST";  
char pass[] = "";            

bool fallDetected = false;
bool impactDetected = false;
bool checkingStillness = false;
unsigned long fallStartTime = 0;
const int alertDuration = 3000;  // 6 seconds for stillness check
unsigned long lastBlynkAlertTime = 0;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);

    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ Connected to WiFi!");

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    Wire.begin();
    mpu.initialize();

    pinMode(buzzer, OUTPUT);
    pinMode(ledPin, OUTPUT);

    Serial.println("MPU6050 Initialized.");
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("⚠️ WiFi Disconnected! Reconnecting...");
        WiFi.begin(ssid, pass);
    }
    
    if (!Blynk.connected()) {
        Serial.println("⚠️ Blynk Disconnected! Reconnecting...");
        Blynk.connect();
    }

    Blynk.run();  

    if (checkingStillness) {
        if (millis() - fallStartTime >= alertDuration) {
            checkStillness();
        }
        return;
    }

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getAcceleration(&ax, &ay, &az);
    mpu.getRotation(&gx, &gy, &gz);

    float accelMagnitude = sqrt(pow(ax / 16384.0, 2) + pow(ay / 16384.0, 2) + pow(az / 16384.0, 2));
    float gyroMagnitude = sqrt(pow(gx / 131.0, 2) + pow(gy / 131.0, 2) + pow(gz / 131.0, 2));

    Serial.print("Accel: ");
    Serial.print(accelMagnitude);
    Serial.print(" | Gyro: ");
    Serial.println(gyroMagnitude);

    // Step 1: Free fall detection
    if (!fallDetected && accelMagnitude < 0.3 && gyroMagnitude < 10) {
        Serial.println("⚠️ Free fall detected! Checking for impact...");
        fallDetected = true;
        fallStartTime = millis();
    }

    // Step 2: Impact detection
    if (fallDetected && millis() - fallStartTime >= 1000) {  
        mpu.getAcceleration(&ax, &ay, &az);
        mpu.getRotation(&gx, &gy, &gz);
        accelMagnitude = sqrt(pow(ax / 16384.0, 2) + pow(ay / 16384.0, 2) + pow(az / 16384.0, 2));
        gyroMagnitude = sqrt(pow(gx / 131.0, 2) + pow(gy / 131.0, 2) + pow(gz / 131.0, 2));

        if (accelMagnitude > 2.0 || gyroMagnitude > 75) {
            Serial.println("🚨 Impact detected! Checking for stillness...");
            digitalWrite(ledPin, HIGH);
            Blynk.virtualWrite(V0, 1);  
            tone(buzzer, 1000, 500);
            delay(1000);
            fallStartTime = millis();
            impactDetected = true;
            checkingStillness = true;
        } else {
            fallDetected = false;
        }
    }

    delay(100);
}

// Step 3: Stillness check
void checkStillness() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getAcceleration(&ax, &ay, &az);
    mpu.getRotation(&gx, &gy, &gz);
    float accelMagnitude = sqrt(pow(ax / 16384.0, 2) + pow(ay / 16384.0, 2) + pow(az / 16384.0, 2));
    float gyroMagnitude = sqrt(pow(gx / 131.0, 2) + pow(gy / 131.0, 2) + pow(gz / 131.0, 2));

    if (accelMagnitude < 0.5 && gyroMagnitude < 20) {
        Serial.println("✅ Fall confirmed! Triggering alert...");
        alert();
    } else {
        Serial.println("❌ False alarm! Person is moving.");
        digitalWrite(ledPin, LOW);
        Blynk.virtualWrite(V0, 0);  
    }
    fallDetected = false;
    impactDetected = false;
    checkingStillness = false;
}

// 🚨 Blynk Notification Alert 🚨
void alert() {
    if (millis() - lastBlynkAlertTime > 5000) {  // Limit alerts to 5s interval
        Serial.println("📢 Emergency alert triggered!");
        Blynk.logEvent("fall_detected", "🚨 Fall detected! Please check on the patient.");
        lastBlynkAlertTime = millis();
    } else {
        Serial.println("⏳ Alert skipped to prevent spam.");
    }

    for (int i = 0; i < 3; i++) {
        tone(buzzer, 1000, 500);
        delay(1000);
    }

    digitalWrite(ledPin, LOW);
    Blynk.virtualWrite(V0, 0);  
}

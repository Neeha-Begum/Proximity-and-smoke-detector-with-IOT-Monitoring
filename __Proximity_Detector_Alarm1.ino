#define BLYNK_TEMPLATE_ID "TMPL3evAHj1li"
#define BLYNK_TEMPLATE_NAME "Proximity Detector Alarm"
#define BLYNK_AUTH_TOKEN "c13v_tbE4A4Uu-Rg3L6F_nVZRdqY60lT"  // Replace with your Auth Token

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

// Wi-Fi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Meherunnisa Manzil";
char pass[] = "Yasneesha@8826";

// Define pins
#define trigPin D1
#define echoPin D2
#define redPin D5
#define greenPin D6
#define bluePin D7
#define buzzerPin D8

// Variables
long duration;
int distance;
int smokeThreshold = 250;  // Adjust as needed
int smokeValue;
bool smokeAlertTriggered = false;  // Track smoke alert
bool proximityAlertTriggered = false;  // Track proximity alert

void setup() {
    // Blynk setup
    Blynk.begin(auth, ssid, pass);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    Serial.begin(115200);
}

void loop() {
    Blynk.run();  // Keeps NodeMCU connected to Blynk

    // Ultrasonic sensor to calculate distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    // Smoke sensor reading
    smokeValue = analogRead(A0);

    // Send data to Blynk app
    Blynk.virtualWrite(V1, distance);      // Display distance on V1
    Blynk.virtualWrite(V2, smokeValue);    // Display smokeValue on V2

    // LED and Buzzer based on distance and smoke level
    if (smokeValue > smokeThreshold && !smokeAlertTriggered) {  // Smoke detected
        digitalWrite(buzzerPin, HIGH);
        setRGB(255, 0, 0);                  // Red for smoke alert
        Blynk.virtualWrite(V3, 255);        // LED Widget ON for red
        Blynk.virtualWrite(V4, "Alert! Smoke detected!"); // Send smoke notification
        smokeAlertTriggered = true;  // Set flag to true after sending notification
    } else if (distance < 20 && !proximityAlertTriggered) {  // Object too close
        digitalWrite(buzzerPin, HIGH);
        setRGB(255, 0, 0);                  // Red for proximity alert
        Blynk.virtualWrite(V3, 255);        // LED Widget ON for red
        Blynk.virtualWrite(V4, "Alert! Object too close!"); // Send proximity notification
        proximityAlertTriggered = true;  // Set flag to true after sending notification
    } else if (distance < 40) {  // Caution state (Yellow)
        digitalWrite(buzzerPin, LOW);
        setRGB(255, 255, 0);                // Yellow for caution
        Blynk.virtualWrite(V3, 127);        // LED Widget ON for yellow
        smokeAlertTriggered = false;  // Reset smoke alert flag
        proximityAlertTriggered = false;  // Reset proximity alert flag
    } else {  // Safe state (Green)
        digitalWrite(buzzerPin, LOW);
        setRGB(0, 255, 0);                  // Green for normal operation
        Blynk.virtualWrite(V3, 0);          // LED Widget OFF
        smokeAlertTriggered = false;  // Reset smoke alert flag
        proximityAlertTriggered = false;  // Reset proximity alert flag
    }

    // Display on Serial for debugging
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Smoke Value: ");
    Serial.println(smokeValue);

    delay(100);  // Short delay to prevent WDT reset
}

// Function to set RGB LED color
void setRGB(int red, int green, int blue) {
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
    yield();  // Allow ESP8266 to reset watchdog
}


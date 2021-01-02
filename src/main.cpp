#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <ESP8266WiFi.h>

#include "credentials.h"

Credentials cred;

const char* auth = cred.auth;      // alternate -> char auth[] = "abc123"

const char* ssid = cred.ssid;      // alternate -> char ssid[] = "abc123"
const char* pass = cred.pass;      // alternate -> char pass[] = "abc123"

#define dhtType DHT11

// Define relay pins
#define relay1 D1
#define relay2 D3
#define relay3 D6
#define relay4 D7

// define dht pin
#define dhtPin D2

DHT dht(dhtPin, dhtType);

/*
 -> Virtual Pins

  * V1 - LED 1
  * V2 - LED 2
  * V3 - LED 3
  * V4 - LED 4

  * V10 - Btn 1
  * V11 - Btn 2
  * V12 - Btn 3
  * V13 - Btn 4

  * V6 - Device Stats
  * V20 - Temp
  * V11 - Humidity
*/

// LED pins on Blynk app
WidgetLED led1(V1);
WidgetLED led2(V2);
WidgetLED led3(V3);
WidgetLED led4(V4);

// LED states
int led1_state = 0;
int led2_state = 0;
int led3_state = 0;
int led4_state = 0;

// count of total active (turned on) LEDs
int total_led;

BlynkTimer timer;

BLYNK_WRITE(V10) {
    int pinValue = param.asInt();

    if (pinValue == 1) {
        digitalWrite(relay1, 1);
        led1.on();
        led1_state = 1;
    } else {
        digitalWrite(relay1, 0);
        led1.off();
        led1_state = 0;
    }
}

BLYNK_WRITE(V11) {
    int pinValue = param.asInt();

    if (pinValue == 1) {
        digitalWrite(relay2, 1);
        led2.on();
        led2_state = 1;
    } else {
        digitalWrite(relay2, 0);
        led2.off();
        led2_state = 0;
    }
}

BLYNK_WRITE(V12) {
    int pinValue = param.asInt();

    if (pinValue == 1) {
        digitalWrite(relay3, 1);
        led3.on();
        led3_state = 1;
    } else {
        digitalWrite(relay3, 0);
        led3.off();
        led3_state = 0;
    }
}

BLYNK_WRITE(V13) {
    int pinValue = param.asInt();

    if (pinValue == 1) {
        digitalWrite(relay4, 1);
        led4.on();
        led4_state = 1;
    } else {
        digitalWrite(relay4, 0);
        led4.off();
        led4_state = 0;
    }
}

void getDhtValues() {
    // delay(2000);
    float h = dht.readHumidity();         // humidity
    float t = dht.readTemperature();      // temp (Celcius)

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Blynk.virtualWrite(V20, t);
    Blynk.virtualWrite(V21, h);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" \t");
    Serial.print("Temp: ");
    Serial.println(t);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Tasnim's Home Automation");
    Blynk.begin(auth, ssid, pass);
    dht.begin();

    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);

    timer.setInterval(10000L, getDhtValues);
}

void loop() {
    Blynk.run();
    timer.run();
    total_led = led1_state + led2_state + led3_state + led4_state;
    Blynk.virtualWrite(V6, total_led);
}
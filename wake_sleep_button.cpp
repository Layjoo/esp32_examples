#include <esp_sleep.h>
#include <Arduino.h>

#define uS_TO_S_FACTOR 1000000 // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP 10       // Time ESP32 will go to sleep (in seconds)
#define BUTTON_PIN 33          // Use a pin that supports RTC functions

const int LEDPin = 23;

// Function to print the reason by which ESP32 has been woken up from sleep
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH);
  pinMode(BUTTON_PIN, INPUT);
  delay(1000); // Give time for the serial monitor to connect

  // Print wakeup reason
  print_wakeup_reason();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
}

void loop()
{

  Serial.println("Going to sleep now");
  delay(3000);            // Allow time to print message before sleeping
  esp_deep_sleep_start(); // Enter deep sleep
}

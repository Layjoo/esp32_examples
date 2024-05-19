#include <esp_sleep.h>
#include <Arduino.h>

#define BUTTON_PIN 33          

enum class DeviceStage
{
  PowerOn,
  PowerOff
};

const int LEDPin = 23;
unsigned long powerPressTime = 0;
const unsigned long powerHoldDuration = 2000; // hold duration
DeviceStage device = DeviceStage::PowerOff;
bool detectingHold = true;

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
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(1000);

  print_wakeup_reason();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
}

void loop()
{
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW)
  {

    if (powerPressTime == 0)
    {
      powerPressTime = millis();
    }

    if (detectingHold && millis() - powerPressTime >= powerHoldDuration)
    {
      switch (device)
      {
      case DeviceStage::PowerOn:
        device = DeviceStage::PowerOff;
        detectingHold = false; //tell esp32 to stop deteching button hold
        digitalWrite(LEDPin, LOW);
        break;
      case DeviceStage::PowerOff:
        device = DeviceStage::PowerOn;
        detectingHold = false; // tell esp32 to stop detecting button hold
        digitalWrite(LEDPin, HIGH);
        break;
      }
    }
  }
  else
  {
    powerPressTime = 0;
    detectingHold = true;

    if (device == DeviceStage::PowerOff)
    {
      Serial.println("Going to sleep now");
      delay(1000);
      esp_deep_sleep_start();
    }
  }
}

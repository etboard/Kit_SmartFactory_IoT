/******************************************************************************************
 * FileName     : etboard_com.cpp
 * Description  : ETboard 공통
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    :
 * Modified     :
 * Modified     :
******************************************************************************************/

#include "etboard_com.h"

//=================================================================================
// for Board Operation LED
//=================================================================================
#define LED_BLINK_INTERVAL 500
#define LED_BUILTIN 5

//=================================================================================
ETBOARD_COM::ETBOARD_COM()
//=================================================================================
{

}

//=================================================================================
void ETBOARD_COM::setup(void)
//=================================================================================
{
  pinMode(LED_BUILTIN, OUTPUT);  digitalWrite(LED_BUILTIN, LOW);
  pinMode(D2, OUTPUT); digitalWrite(D2, LOW);
  pinMode(D3, OUTPUT); digitalWrite(D3, LOW);
  pinMode(D4, OUTPUT); digitalWrite(D4, LOW);
  pinMode(D5, OUTPUT); digitalWrite(D5, LOW);

  Serial.begin(115200);
}

//=================================================================================
void ETBOARD_COM::fast_blink_led(void)
//=================================================================================
{
  for(int i=0; i<10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                         // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(50);                         // wait for a second
  }
}

//==============================================================================
void ETBOARD_COM::normal_blink_led(void)
//==============================================================================
{
  static uint8_t ledState = LOW;             // ledState used to set the LED
  static unsigned long previousMillis = 0;   // will store last time LED was updated

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= LED_BLINK_INTERVAL) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
  }
}

//=================================================================================
void ETBOARD_COM::wifi_setup_start_led(void)
//=================================================================================
{
  pinMode(D4, OUTPUT); digitalWrite(D3, HIGH);    // Green LED ON
}

//=================================================================================
void ETBOARD_COM::wifi_setup_end_led(void)
//=================================================================================
{
  pinMode(D4, OUTPUT); digitalWrite(D3, LOW);     // Green LED OFF
}

//=================================================================================
void ETBOARD_COM::print_board_information(const char* hardware_version, const char* firmware_verion)
//=================================================================================
{
  Serial.print("\n\n");
  Serial.println("################################################################");
  Serial.print(" Welcome to ");
  Serial.print(hardware_version);
  Serial.println(" !!!");
  Serial.print(" Firmware version is ");
  Serial.println(firmware_verion);
  Serial.println("################################################################");
}

//=================================================================================
double ETBOARD_COM::round2(double value)
//=================================================================================
{
   return (int)(value * 100 + 0.5) / 100.0;       // 소수점 2자리로 반올림
}

//=================================================================================
// End of Line
//=================================================================================

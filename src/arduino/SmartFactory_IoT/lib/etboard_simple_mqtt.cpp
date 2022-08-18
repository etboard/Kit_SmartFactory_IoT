/******************************************************************************************
 * FileName     : etboard_simple_mqtt.cpp
 * Description  : ETboard Simple MQTT
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    :
 * Modified     :
 * Modified     :
******************************************************************************************/

#include "etboard_simple_mqtt.h"
#include "EspMQTTClient.h"

/*
EspMQTTClient client(
  "broker.hivemq.com",  // MQTT Broker server ip
  1883,              // The MQTT port, default to 1883. this line can be omitted
  "",               // Can be omitted if not needed
  "",               // Can be omitted if not needed
  ""                // Client name that uniquely identify your device
);
*/

#define MAX_ANALOG 8
//int analogs[MAX_ANALOG]={36, 39, 32, 33, 34, 35, 25, 26};

#define MAX_DIGITAL 10           // 0  1   2   3   4   5   6   7   8   9
int digitals[MAX_DIGITAL]      = {-1, -1, D2, D3, D4, D5, D6, D7, D8, D9};
int digitals_mode[MAX_DIGITAL] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
int digitals_value[MAX_DIGITAL]= {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};


//=================================================================================
ETBOARD_SIMPLE_MQTT::ETBOARD_SIMPLE_MQTT()
//=================================================================================
{
  mqtt_simple_prefix = "et/smpl";
  mqtt_simple_cmnd = "cmnd";
  mqtt_simple_tele = "tele";
  mqtt_simple_stat = "stat";
  mac_address = "";
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::setup(
    const char* mqttServerIp,
    const short mqttServerPort,
    const char* mqttUsername,
    const char* mqttPassword,
    const char* mqttClientName)
//=================================================================================
{
  Serial.println("\n");
  Serial.println("================================================================");
  Serial.println(" MQTT setup starting..... ");
  Serial.println("================================================================");

  mac_address = WiFi.macAddress();

	randomSeed(micros());
	String str_client_name = mac_address.substring(9);
  str_client_name += "_" + String(random(0xffff), HEX);
  int str_len = str_client_name.length() + 1;
  str_client_name.toCharArray(ch_client_name, str_len);
  client.setMqttClientName(ch_client_name);

  client.setMqttServer(mqttServerIp, mqttUsername, mqttPassword, mqttServerPort);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  //client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  //client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  //client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true

  Serial.println("================================================================");
  Serial.println(" MQTT setup ending ..... ");
  Serial.println("================================================================");
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::setup_with_wifi(
    const char* wifiSsid,
    const char* wifiPassword,
    const char* mqttServerIp,
    const char* mqttUsername,
    const char* mqttPassword,
    const char* mqttClientName,
    const short mqttServerPort)
//=================================================================================
{
  Serial.println("\n");
  Serial.println("================================================================");
  Serial.println(" MQTT setup starting..... ");
  Serial.println("================================================================");

  client.setWifiCredentials(wifiSsid, wifiPassword);
  client.setMqttServer(mqttServerIp, mqttUsername, mqttPassword, mqttServerPort);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  //client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  //client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  //client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true

  Serial.println("================================================================");
  Serial.println(" MQTT setup ending ..... ");
  Serial.println("================================================================");
}

//=================================================================================
String ETBOARD_SIMPLE_MQTT::get_topic_prefix(void)
//=================================================================================
{
  String prefix = "";
  prefix = mac_address.substring(9) + "/" + mqtt_simple_prefix;
  return prefix;
}

//=================================================================================
String ETBOARD_SIMPLE_MQTT::get_cmnd_prefix(void)
//=================================================================================
{
  String prefix = "";
  prefix = get_topic_prefix() + "/" + mqtt_simple_cmnd;
  return prefix;
}

//=================================================================================
String ETBOARD_SIMPLE_MQTT::get_stat_prefix(void)
//=================================================================================
{
  String prefix = "";
  prefix = get_topic_prefix() + "/" + mqtt_simple_stat;
  return prefix;
}

//=================================================================================
String ETBOARD_SIMPLE_MQTT::get_tele_prefix(void)
//=================================================================================
{
  String prefix = "";
  prefix = get_topic_prefix() + "/" + mqtt_simple_tele;
  return prefix;
}

//=================================================================================
// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
//=================================================================================
void ETBOARD_SIMPLE_MQTT::onConnectionEstablished(void)
//=================================================================================
{

  /*
  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish(get_topic_prefix()
        + "/info/version",  String(board_hardware_verion) + "/" + String(board_firmware_verion));
  });
  */

  //mac_address = WiFi.macAddress();

  recv_digital();
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::send_test_analog_a0(void)
//=================================================================================
{
  int sensorValue = 0;
  sensorValue = analogRead(A0);
  client.publish("etboard/a0", String(sensorValue));
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::publish_tele(const String &topic, const String &payload)
//=================================================================================
{
  client.publish(get_tele_prefix() + topic, payload);
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::send_analog(void)
//=================================================================================
{
  StaticJsonDocument<300> doc;
  JsonArray data = doc.createNestedArray("values");

  int sensorValue = 0;
  sensorValue = analogRead(A0); data.add(sensorValue);
  sensorValue = analogRead(A1); data.add(sensorValue);
  sensorValue = analogRead(A2); data.add(sensorValue);
  sensorValue = analogRead(A3); data.add(sensorValue);
  sensorValue = analogRead(A4); data.add(sensorValue);
  sensorValue = analogRead(A5); data.add(sensorValue);

  String output;
  serializeJson(doc, output);

  if (client.publish(get_topic_prefix() + "/analog", output) == true) {
    //Serial.println("Success sending message");
  } else {
    //Serial.println("Error sending message");
  }
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::send_digital(void)
//=================================================================================
{
  StaticJsonDocument<300> doc;
  JsonArray data = doc.createNestedArray("values");

  for (int pinNumber = 0; pinNumber < MAX_DIGITAL; pinNumber++) {
    data.add(digitals_value[pinNumber]);
  }

  String output;
  serializeJson(doc, output);

  if (client.publish(get_tele_prefix() + "/digital", output) == true) {
    //Serial.println("Success sending message");
  } else {
    //Serial.println("Error sending message");
  }
}


//=================================================================================
void ETBOARD_SIMPLE_MQTT::recv_digital(void)
//=================================================================================
{
  client.subscribe(get_cmnd_prefix() + "/D2", [](const String & payload) {
  pinMode(D2, OUTPUT);
  if (payload == "0") digitalWrite(D2, LOW);
    else digitalWrite(D2, HIGH);
  });

  client.subscribe(get_cmnd_prefix() + "/D3", [](const String & payload) {
  pinMode(D3, OUTPUT);
  if (payload == "0") digitalWrite(D3, LOW);
    else digitalWrite(D3, HIGH);
  });

  client.subscribe(get_cmnd_prefix() + "/D4", [](const String & payload) {
  pinMode(D4, OUTPUT);
  if (payload == "0") digitalWrite(D4, LOW);
    else digitalWrite(D4, HIGH);
  });

  client.subscribe(get_cmnd_prefix() + "/D5", [](const String & payload) {
  pinMode(D5, OUTPUT);
  if (payload == "0") digitalWrite(D5, LOW);
    else digitalWrite(D5, HIGH);
  });

}

//=================================================================================
boolean ETBOARD_SIMPLE_MQTT::is_changed_digital(void)
//=================================================================================
{
  // check if different current and previous value
  boolean bFound = false;
  for (int pinNumber = 6; pinNumber < MAX_DIGITAL; pinNumber++) {
      int pin = digitals[pinNumber];
      if(pin >= 0) {
        pinMode(pin,INPUT);
        int val = digitalRead(pin);

        // 2018.10.15 : SCS : To reverse 4 buttons's value on board is pull-up circuit
        if( pinNumber >=6 && pinNumber <= 9)
        {
          val = !val;
        }

        if (digitals_value[pinNumber] != val) {
          digitals_value[pinNumber] = val;
          bFound = true;
        }
     }
  }
  return bFound;

}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::loop(void)
//=================================================================================
{
  client.loop();
}

//==========================================================================================
void ETBOARD_SIMPLE_MQTT::dg_Write(int pin, int value)
//==========================================================================================
{
  switch (pin) {
    case D2: current_digital_value[2] = value; break;
    case D3: current_digital_value[3] = value; break;
    case D4: current_digital_value[4] = value; break;
    case D5: current_digital_value[5] = value; break;
    case D6: current_digital_value[6] = value; break;
    case D7: current_digital_value[7] = value; break;
    case D8: current_digital_value[8] = value; break;
    case D9: current_digital_value[9] = value; break;
    default:Serial.println("dg_write error"); break;
  }
}

//==========================================================================================
void ETBOARD_SIMPLE_MQTT::update_digital_value(void)
//==========================================================================================
{
  previous_digital_value[2] = current_digital_value[2];
  previous_digital_value[3] = current_digital_value[3];
  previous_digital_value[4] = current_digital_value[4];
  previous_digital_value[5] = current_digital_value[5];
  previous_digital_value[6] = current_digital_value[6];
  previous_digital_value[7] = current_digital_value[7];
  previous_digital_value[8] = current_digital_value[8];
  previous_digital_value[9] = current_digital_value[9];
}

//==========================================================================================
bool ETBOARD_SIMPLE_MQTT::isChanged_digital_value(void)
//==========================================================================================
{
  for(int i=2; i<MAX_DIGITAL; i++) {
    if (previous_digital_value[i] != current_digital_value[i]) {
      return true;
    }
  }

  return false;
}

//==========================================================================================
void ETBOARD_SIMPLE_MQTT::initailize_digital_value()
//==========================================================================================
{
  for(int i=2; i<MAX_DIGITAL; i++) {
    previous_digital_value[i] = 0;
    current_digital_value[i] = 0;
  }
}

//==========================================================================================
int ETBOARD_SIMPLE_MQTT::dg_Read(int pin)
//==========================================================================================
{
  int value = 0;
  switch (pin) {
    case D2: value = current_digital_value[2]; break;
    case D3: value = current_digital_value[3]; break;
    case D4: value = current_digital_value[4]; break;
    case D5: value = current_digital_value[5]; break;
    case D6: value = current_digital_value[6]; break;
    case D7: value = current_digital_value[7]; break;
    case D8: value = current_digital_value[8]; break;
    case D9: value = current_digital_value[9]; break;
    default:Serial.println("get_digital_value error"); break;
  }
  return value;
}


//=================================================================================
// End of Line
//=================================================================================

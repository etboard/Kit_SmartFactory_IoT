/******************************************************************************************
 * FileName     : etboard_wifi.cpp
 * Description  : ETboard WiFi Manager
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    :
 * Modified     :
 * Modified     :
******************************************************************************************/

#include "etboard_wifi.h"
#include <wifi.h>

WiFiManagerParameter * custom_mqtt_server;
WiFiManagerParameter * custom_mqtt_port;
WiFiManagerParameter * custom_mqtt_user;
WiFiManagerParameter * custom_mqtt_pass;

bool ETBOARD_WIFI::shouldSaveConfig = false;

#define TRIGGER_PIN D9

WiFiManager wifiManager;

//=================================================================================
//callback notifying us of the need to save config
//=================================================================================
void ETBOARD_WIFI::save_config_cb()
//=================================================================================
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//=================================================================================
// for Board Operation LED
//=================================================================================
#define LED_BLINK_INTERVAL 500
#define LED_BUILTIN 5

//=================================================================================
ETBOARD_WIFI::ETBOARD_WIFI()
//=================================================================================
{
  custom_mqtt_server = new WiFiManagerParameter("server", "mqtt server", mqtt_server, 40);
  custom_mqtt_port = new WiFiManagerParameter("port", "mqtt port", mqtt_port, 6);
  custom_mqtt_user = new WiFiManagerParameter("user", "mqtt user", mqtt_user, 20);
  custom_mqtt_pass = new WiFiManagerParameter("pass", "mqtt pass", mqtt_pass, 20);

  shouldSaveConfig = false;
}

//=================================================================================
void ETBOARD_WIFI::setup(void)
//=================================================================================
{
  Serial.println("\n");
  Serial.println("================================================================");
  Serial.println(" WiFi setup starting..... ");
  Serial.println("================================================================");

  checkButton();

  load_config();

  wifi_config();

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    //read updated parameters
    strcpy(mqtt_server, custom_mqtt_server->getValue());
    strcpy(mqtt_port, custom_mqtt_port->getValue());
    strcpy(mqtt_user, custom_mqtt_user->getValue());
    strcpy(mqtt_pass, custom_mqtt_pass->getValue());

    save_config();
  }

  Serial.println("");
  Serial.println("----------------------------------------------------------------");
  Serial.print(" ETboard local ip address is [");
  Serial.print(WiFi.localIP());
  Serial.println("]");
  Serial.println("----------------------------------------------------------------");
  Serial.println("");
  Serial.println("================================================================");
  Serial.println(" WiFi setup ending ..... ");
  Serial.println("================================================================");

}

//=================================================================================
void ETBOARD_WIFI::checkButton()
//=================================================================================
{
  pinMode(TRIGGER_PIN, INPUT);

  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        digitalWrite(D2, HIGH); // RED LED ON
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wifiManager.resetSettings();
        // 2022.08.06
        SPIFFS.format();
        ESP.restart();
      }
    }
  }
}

//=================================================================================
void ETBOARD_WIFI::load_config()
//=================================================================================
{
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        if (!error) {
          Serial.println("\nparsed json");
          serializeJsonPretty(json, Serial);
          Serial.println("\n");
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}

//=================================================================================
void ETBOARD_WIFI::save_config()
//=================================================================================
{
    Serial.println("saving config");
    StaticJsonDocument<512> json;
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_pass"] = mqtt_pass;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    // Serialize JSON data to write to file
    serializeJsonPretty(json, Serial);
    if (serializeJson(json, configFile) == 0)
    {
      // Error writing file
      Serial.println(F("Failed to write to file"));
    }
    // Close file
    configFile.close();
}

//=================================================================================
void ETBOARD_WIFI::wifi_config()
//=================================================================================
{
  // wifi config start: BLEU LED ON
  pinMode(D3, OUTPUT);
  digitalWrite(D3, HIGH);

  //set config save notify callback
  wifiManager.setSaveConfigCallback(save_config_cb);

  //add all your parameters here
  wifiManager.addParameter(custom_mqtt_server);
  wifiManager.addParameter(custom_mqtt_port);
  wifiManager.addParameter(custom_mqtt_user);
  wifiManager.addParameter(custom_mqtt_pass);

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);
  wifiManager.setTimeout(30);

	String str_wifi_ap_name = "ETAP_" + WiFi.macAddress().substring(9);
  int str_len = str_wifi_ap_name.length() + 1;
  char ch_wifi_ap_name[20];
  str_wifi_ap_name.toCharArray(ch_wifi_ap_name, str_len);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  //if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
  if (!wifiManager.autoConnect(ch_wifi_ap_name, "12341234")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  digitalWrite(D3, LOW);
}

//=================================================================================
// End of Line
//=================================================================================

/******************************************************************************************
 * FileName     : etboard_simple_mqtt.h
 * Description  : ETboard Simple MQTT
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    : 
 * Modified     : 
 * Modified     : 
******************************************************************************************/

#ifndef ETBOARD_SIMPLE_MQTT_H
#define ETBOARD_SIMPLE_MQTT_H

#include <Arduino.h>
//#include <String>
#include <ArduinoJson.h>
#include "EspMQTTClient.h"

class ETBOARD_SIMPLE_MQTT {

  private:	
    String get_topic_prefix(void);
    static const char MAX_DIGITAL = 11;
    int previous_digital_value[MAX_DIGITAL];
    
  
  public:
    EspMQTTClient client;

    String mqtt_simple_prefix;
    String mqtt_simple_cmnd;
    String mqtt_simple_tele;
    String mqtt_simple_stat;

    String mac_address;
    char   ch_client_name[20];
    int current_digital_value[MAX_DIGITAL];
    
  	ETBOARD_SIMPLE_MQTT();
    
    void setup(
      const char* mqttServerIp,
      const short mqttServerPort,
      const char* mqttUsername,
      const char* mqttPassword,
      const char* mqttClientName);  
      
    void setup_with_wifi(
      const char* wifiSsid,
      const char* wifiPassword,
      const char* mqttServerIp,
      const char* mqttUsername,
      const char* mqttPassword,
      const char* mqttClientName,
      const short mqttServerPort);  
    
    String get_cmnd_prefix(void);
    String get_stat_prefix(void);
    String get_tele_prefix(void);
    void onConnectionEstablished(void);
    void send_test_analog_a0(void);
    void publish_tele(const String &topic, const String &payload);
    void send_analog(void);
    void send_digital(void);
    void recv_digital(void);
    boolean is_changed_digital(void);
    void loop(void);

    void dg_Write(int pin, int value);
    void update_digital_value();
    bool isChanged_digital_value(void);
    void initailize_digital_value(void);
    int  dg_Read(int pin);
};

#endif

//=================================================================================
// End of Line
//=================================================================================


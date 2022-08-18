/******************************************************************************************
 * FileName     : etboard_wifi.h
 * Description  : ETboard WiFi Manager
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    : 
 * Modified     : 
 * Modified     : 
******************************************************************************************/

#ifndef ETBOARD_WIFI_H
#define ETBOARD_WIFI_H

//#include <Arduino.h>
#include <SPIFFS.h>              //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

class ETBOARD_WIFI {

  private:	
    //String lineString[3];
  
  public:
    //flag for saving data
    static bool shouldSaveConfig;
    
    char mqtt_server[40] =    "broker.hivemq.com";
    char mqtt_port[6] =       "1883";
    char mqtt_user[16] =      "";
    char mqtt_pass[16] =      "";
    
  	ETBOARD_WIFI();
  	void setup(void);    
    void checkButton(void);    
    static void save_config_cb();
    void load_config();
    void save_config();
    void wifi_config();
};

#endif

//=================================================================================
// End of Line
//=================================================================================

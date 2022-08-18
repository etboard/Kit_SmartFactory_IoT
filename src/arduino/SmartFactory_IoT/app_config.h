/******************************************************************************************
 * FileName     : app_config.h
 * Description  : 응용 프로그램 구성 
 * Author       : SCS
 * Created Date : 2022.08.08
 * Reference    : 
 * Modified     : 
 * Modified     : 
******************************************************************************************/

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "lib/etboard_com.h"
#include "lib/etboard_simple_mqtt.h"
#include "lib/etboard_oled_u8g2.h"
#include "lib/etboard_wifi.h"

extern const char* board_hardware_verion;
extern const char* board_firmware_verion;

//==========================================================================================
class APP_CONFIG 
//==========================================================================================
{

  private:      
  
  public:          
    //const char* board_hardware_verion = "ETBoard_V1.1";
    //const char* board_firmware_verion = "smartLgt_v0.9";
    unsigned lastMillis;
    String operation_mode = "automatic";
    bool bDigitalChanged = false;
    
    ETBOARD_OLED_U8G2 oled;
    ETBOARD_COM etboard;
    ETBOARD_SIMPLE_MQTT mqtt; 
    ETBOARD_WIFI wifi;   
    
    APP_CONFIG();
    void setup(void);    
    void fast_blink_led(void);
    void normal_blink_led(void); 
    void display_BI(void);

    void dg_Write(int pin, int value);
    void update_digital_value();
    bool isChanged_digital_value(void);
    void initailize_digital_value(void);
    int  dg_Read(int pin);
    
};

#endif

//==========================================================================================
// End of Line
//==========================================================================================

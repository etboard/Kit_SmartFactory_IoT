/******************************************************************************************
 * FileName     : etboard_com.h
 * Description  : ETboard 공통
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    :
 * Modified     :
 * Modified     :
******************************************************************************************/

#ifndef ETBOARD_COM_H
#define ETBOARD_COM_H

#include <Arduino.h>
//#include <String>

class ETBOARD_COM {

  private:
    //String lineString[3];

  public:
  	ETBOARD_COM();
  	void setup(void);
  	void fast_blink_led(void);
    void normal_blink_led(void);
    void wifi_setup_start_led(void);
    void wifi_setup_end_led(void);
    void print_board_information(const char* hardware_version, const char* firmware_verion);
    double round2(double value);

};

#endif

//=================================================================================
// End of Line
//=================================================================================

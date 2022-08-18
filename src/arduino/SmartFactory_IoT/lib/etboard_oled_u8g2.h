/******************************************************************************************
 * FileName     : etboard oled_u8g2.h
 * Description  : ETboard SS1306 OLED
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    : 
 * Modified     : 
 * Modified     : 
******************************************************************************************/

#ifndef ETBOARD_OLED_U8G2_H
#define ETBOARD_OLED_U8G2_H

// 2022.08.06 : SCS
#include <Arduino.h>
//#include <String>

class ETBOARD_OLED_U8G2{

private:	
  String lineString[3];

public:
	ETBOARD_OLED_U8G2();
  
	void setup(void);
	void setLine(int line, String buffer);
	void display();	
};

#endif

//=================================================================================
// End of Line
//=================================================================================

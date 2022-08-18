/******************************************************************************************
 * FileName     : etboard_oled_u8g2.cpp
 * Description  : ETboard SS1306 OLED
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    : 
 * Modified     : 
 * Modified     : 
******************************************************************************************/

#include "etboard_oled_u8g2.h"

#include <Arduino.h>
#include <U8g2lib.h>
#include <String>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// 2018.09.05 : SCS
// U8g2 Contructor (Frame Buffer)

// Slow
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE);
// Fast
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// End of constructor

//=================================================================================
ETBOARD_OLED_U8G2::ETBOARD_OLED_U8G2() 
//=================================================================================	
{
	lineString[0] = "";	
	lineString[1] = "";	
	lineString[2] = "";		
}

//=================================================================================
void ETBOARD_OLED_U8G2::setup(void) 
	//=================================================================================
{
  u8g2.begin();
  u8g2.clearBuffer();          // clear the internal memory
}

//=================================================================================
void ETBOARD_OLED_U8G2::setLine(int line, String buffer)
//=================================================================================
{  
  if (line < 1 || line > 3) return;
  lineString[line - 1] = buffer;  
}

//=================================================================================
void ETBOARD_OLED_U8G2::display() 
//=================================================================================
{ 
	// Clear 
  u8g2.clearBuffer();          // clear the internal memory
  
  // First line
  u8g2.setFont(u8g2_font_9x15B_tf);	
  
  u8g2.setCursor(0,10);
  u8g2.print(lineString[0]);

  // Second line
  u8g2.setFont(u8g2_font_logisoso18_tf ); 
  
  u8g2.setCursor(0,35);
  u8g2.print(lineString[1]);  

  // Third line
  u8g2.setCursor(0,60);
  u8g2.print(lineString[2]);
  
  // Display
  u8g2.sendBuffer();					// transfer internal memory to the display
}
 
//=================================================================================
// End of Line
//=================================================================================

/******************************************************************************************
 * FileName     : SmartFactory_IoT.ino
 * Description  : 이티보드 스마트 팩토리 코딩 키트(IoT)
 * Author       : SCS
 * Created Date : 2022.08.18
 * Reference    : 
 * Modified     : 2022.08.19 : LCS
 * Modified     : 
******************************************************************************************/
const char* board_hardware_verion = "ETBoard_V1.1";
const char* board_firmware_verion = "smartLgt_0.91";

//================================================-=========================================
// 응용 프로그램 구성 사용하기                       
//==========================================================================================
#include "app_config.h"
APP_CONFIG app;


//==========================================================================================
// 상수 정의                                       
//==========================================================================================
// 메시지 송신 주기 : 주의!!!! 너무 빨리 또는 많이 보내면 서버에서 거부할 수 있음(Banned)
//------------------------------------------------------------------------------------------
#define NORMAL_SEND_INTERVAL  (1000 * 1)          // 권장 5초 (단위: 초/1000)


//==========================================================================================
// 전역 변수 선언                                   
//==========================================================================================
int TRIG = D9;                                    // 초음파 송신 핀
int ECHO = D8;                                    // 초음파 수신 핀
int RESET_PIN = D7;                               // 카운트 리셋핀 (D7 = 파란 버튼)

int Count = 0;                                    // 카운터용 변수
float distance_value;                             // 초음파 센서 값(거리)
char temp_buffer[255] = {0, };                    // 포멧팅용 임시 버퍼
int pre_time = 0;                                 // 이전에 물건이 지나간 시간


//==========================================================================================
void setup()                                      // 설정 함수 
//==========================================================================================
// (권장 사항) 이 함수에서는 코딩하지 마십시오. custom_setup()에 코딩하십시오.
//------------------------------------------------------------------------------------------
{
  app.setup();                                    // 응용 프로그램 기본 설정
  
  custom_setup();                                 // 사용자 맞춤형 설정
  app.oled.setup();
  
  oled_show(Count);
}


//==========================================================================================
void custom_setup()                               // 사용자 맞춤형 설정 함수
//==========================================================================================
{
  //----------------------------------------------------------------------------------------
  // 여기에 사용자 맞춤형 설정을 코딩하세요.
  //----------------------------------------------------------------------------------------
  // 초음파 센서 핀 설정                            
  //----------------------------------------------------------------------------------------
  pinMode(TRIG, OUTPUT);                          // 초음파 송신 핀을 출력 모드로 설정
  pinMode(ECHO, INPUT);                           // 초음파 수신 핀을 입력 모드로 설정
  pinMode(RESET_PIN, INPUT);                      // 리셋버튼, 핀모드설정

  pinMode(SDA, INPUT);                            // OLED 핀 모드 설정
  pinMode(SCL, INPUT);     
}


//==========================================================================================
void loop()                                       // 반복 루틴
//==========================================================================================
//  (권장 사항) 이 함수를 가능하면 수정하지 마십시오 !!! 
//  do_sensing_process(), do_automatic_process(), send_sensor_value(), 
//------------------------------------------------------------------------------------------
{
  //----------------------------------------------------------------------------------------
  // MQTT 백그라운드 동작 
  //----------------------------------------------------------------------------------------
  app.mqtt.loop();

  //----------------------------------------------------------------------------------------
  // 센싱 처리
  //----------------------------------------------------------------------------------------       
  do_sensing_process();                             

  //----------------------------------------------------------------------------------------
  // 동작 모드가 automatic 일 경우 자동화 처리
  //----------------------------------------------------------------------------------------      
  if(app.operation_mode == "automatic") {         // 수정 금지
    do_automatic_process();                       // 자동화 처리    
  }
    
  //----------------------------------------------------------------------------------------
  // 주기적으로 메시지 전송 처리
  //----------------------------------------------------------------------------------------
  if (millis() - app.lastMillis > NORMAL_SEND_INTERVAL) {  
    send_sensor_value();                          // 센서 값 송신
    app.lastMillis = millis();                    // 현재 시각 업데이트
  }  

  //----------------------------------------------------------------------------------------
  // 동작 상태 LED 깜밖이기
  //----------------------------------------------------------------------------------------  
  app.etboard.normal_blink_led();               
}


//==========================================================================================
void do_sensing_process()                         // 센싱 처리 함수
//==========================================================================================
{ 
  //----------------------------------------------------------------------------------------
  // 거리 값 센싱하기; 초음파 센서
  //----------------------------------------------------------------------------------------  
  delay(10);                                      // 10/1000초 만큼 대기
  
  //----------------------------------------------------------------------------------------
  // 초음파 측정
  //----------------------------------------------------------------------------------------  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  //----------------------------------------------------------------------------------------
  // 초음파 수신 값으로 거리 계산
  //----------------------------------------------------------------------------------------  
  float duration = pulseIn (ECHO, HIGH);      
  distance_value = duration * 17 / 1000;  
  //Serial.println(distance_value);
}

//==========================================================================================
void oled_show(int Count)
//==========================================================================================
{
  delay(10);
  sprintf(temp_buffer, "count : %d", Count);
  app.oled.setLine(1, "*Smart Factory");          // OLED 첫 번째 줄 : 시스템 이름
  app.oled.setLine(2, temp_buffer);               // OLED 두 번째 줄 : 지나간 물체 갯수
  app.oled.setLine(3, "-------------");           // OLED 세 번째 줄 : ---------------
  app.oled.display();
}

//==========================================================================================
void do_automatic_process()                       // 자동화 처리 함수
//==========================================================================================
// 여기에 자동화 처리를 코딩하세요.
//------------------------------------------------------------------------------------------
{  
  //----------------------------------------------------------------------------------------  
  // 스마트 펙토리 시스템
  //----------------------------------------------------------------------------------------  
  pinMode(D2, OUTPUT);                            // D2핀을 출력 모드로 설정
  pinMode(D3, OUTPUT);                            // D3핀을 출력 모도로 설정
  
  if(distance_value < 30)
  {
    int now_time = millis();
    if(now_time - pre_time > 500)
    {
      Count += 1;

      oled_show(Count);
      delay(1000);

      pre_time = now_time;
    }
  }

  if(digitalRead(RESET_PIN) == LOW)
  {
    Serial.println("reset count");
    Count = 0;
    oled_show(Count);
  }
}


//==========================================================================================
void send_sensor_value()                          // 센서 값 송신 함수
//==========================================================================================
{ 
  // 예시 {"Count":18}
  
  DynamicJsonDocument doc(256);                   // json 
  doc["Count"] = Count;                           // 지나간 물체 갯수

  String output;                                  // 문자열 변수
  serializeJson(doc, output);                     // json을 문자열로 변환
  app.mqtt.publish_tele("/sensor", output);       // 송신
}

//==========================================================================================
void onConnectionEstablished()                    // MQTT 연결되었을 때 동작하는 함수
//==========================================================================================
{
  app.mqtt.onConnectionEstablished();             // MQTT 연결되었을 때 동작
  recv_automatic_mode();                          // 동작 모스 수신 설정
}


//==========================================================================================
void recv_automatic_mode(void)                    // 동작 모드 수신 함수
//==========================================================================================
{
  app.mqtt.client.subscribe(
    app.mqtt.get_cmnd_prefix() + "/operation_mode", // operation_mode 명령을 수신
    [&](const String & payload) {                 // 명령 내용을 payload에 저장       
      pinMode(D5, OUTPUT);                        // D5 핀을 출력 모드로 설정
      if (payload == "automatic"){                // 자동모드 설정 명령이면
        app.operation_mode = "automatic";         // 자동모드로 저장
        digitalWrite(D5, LOW);                    // D5 핀 LED 끄기
      }
      else{
        app.operation_mode = "manual";            // 수동모드로 저장
        digitalWrite(D5, HIGH);                   // D5 핀 LED 켜기
        }
      });
}


//==========================================================================================
//                                                    
// (주)한국공학기술연구원 http://et.ketri.re.kr       
//                                                    
//==========================================================================================

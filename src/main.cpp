#include <Arduino.h>
#include <Ticker.h>
#include "manager.hpp"


/** 设置定时器：每5分钟向串口发送数据 */
Ticker timer_sendDate(sendData, 300000,INT16_MAX, MILLIS);

void setup()
{
  initKey();                 //初始化按键
  display.setBrightness(0x0f);  //设置数码管亮度
  display.clear();            //清空数码管内容
  initClock<ThreeWire>(Rtc);  //初始化外部时钟
  /* 绑定中断函数 */
  attachInterrupt(digitalPinToInterrupt(KEY4),setTime, RISING);
  attachInterrupt(digitalPinToInterrupt(KEY3),setAlrm, RISING);
  /* 设置定时器中断，每隔5分钟发送当前时钟和闹钟设置到串口 */
  timer_sendDate.start();
  
}

void loop()
{
  noInterrupts(); //关闭中断
  if(is_alarm_on){
    if(!alarm_status){
      if(array_alarm_time[1] == Rtc.GetDateTime().Hour() &&
            array_alarm_time[0] == Rtc.GetDateTime().Minute()){
        alarm_status = true;
        Serial.println("[Note]:The alarm is ringing!"); 
      }
    }   

  }
  interrupts(); //启动中断
  if(alarm_status){
    playSong();
  }
  showTime(display,Rtc.GetDateTime());
  if(scanKey() == KEY1_PRES){
    showDate(display,Rtc.GetDateTime());
    delay(3000);
  }
  timer_sendDate.update();
}

/** 发送数据 */
void sendData(){
  char datestring[10];
  const char * alarm_status = is_alarm_on ? "ON" : "OFF";
  Serial.println("+-----Setting Begin-----+");
  Serial.print("RTC Time: ");
  printDateTime(Rtc.GetDateTime());
  Serial.print("Alarm Time: ");
  snprintf(datestring,
             GET_SIZE(datestring),
             PSTR("%02u:%02u"),
             array_alarm_time[1],
             array_alarm_time[0]);
  Serial.println(datestring);
  Serial.print("Alarm Status: ");
  Serial.println(alarm_status);
  Serial.println("+----Setting end--------+");
}



/** 设置闹钟 */
void setAlrm(){
  //撤销中断
  detachInterrupt(KEY4);
  detachInterrupt(KEY3);
  delay_in_interrupts(200);
  //记录按键是否按下次数
  uint32_t press_key2_conut = 0;
  while (true)
  {
      uint8_t key_status = scanKey();
      //是否退出当前模式
      bool isExit = false;
      switch (key_status)
      {
      case KEY1_PRES:
        array_alarm_time[press_key2_conut & 1]++;
        if(array_alarm_time[0] >= 60){
          array_alarm_time[0] = 0;
        }
        if(array_alarm_time[1] >= 24){
          array_alarm_time[1] = 0;
        }
        break;
      case KEY2_PRES:
        press_key2_conut++;
        break;
      case KEY3_PRES:
        is_alarm_on = true;
        isExit = true;
        Serial.println("[Note]:The alarm settings is Changed!");
      break;
      case KEY4_PRES:
        isExit = true;
        break;
      default:
        break;
      }
      if((press_key2_conut & 1) == 1){
        display.showNumberDec(array_alarm_time[press_key2_conut & 1], false, 2);
      }else
      {
        display.showNumberDec(array_alarm_time[press_key2_conut & 1], false);
      }
      if(isExit){
          /* 绑定中断函数 */
          attachInterrupt(digitalPinToInterrupt(KEY4),setTime, RISING);
          attachInterrupt(digitalPinToInterrupt(KEY3),setAlrm, RISING);
          break;
      }
  }
}

/** 设置时间 */
void setTime(){
  //撤销中断
  detachInterrupt(KEY4);
  detachInterrupt(KEY3);
  delay_in_interrupts(200);
  //记录分钟
  volatile uint8_t array_time[] = {0,0};
  //记录按键是否按下
  volatile uint32_t press_key2_conut = 0;
  while (true)
  {
      uint8_t key_status = scanKey();
      //是否退出当前模式
      bool isExit = false;
      switch (key_status)
      {
      case KEY1_PRES:
        array_time[press_key2_conut & 1]++;
        if(array_time[0] >= 60){
          array_time[0] = 0;
        }
        if(array_time[1] >= 24){
          array_time[1] = 0;
        }
        break;
      case KEY2_PRES:
        press_key2_conut++;
        break;
      case KEY3_PRES:
        Rtc.SetDateTime(RtcDateTime(Rtc.GetDateTime().Year(),
                        Rtc.GetDateTime().Month(),
                        Rtc.GetDateTime().Day(),
                        array_time[1],array_time[0],
                        Rtc.GetDateTime().Second()));
        isExit = true;
        Serial.println("[Note]:The time is Changed!");
      break;
      case KEY4_PRES:
        isExit = true;
        break;
      default:
        break;
      }
      if((press_key2_conut & 1) == 1){
        display.showNumberDec(array_time[press_key2_conut & 1], false, 2);
      }else
      {
        display.showNumberDec(array_time[press_key2_conut & 1], false);
      }
      if(isExit){
          /* 绑定中断函数 */
          attachInterrupt(digitalPinToInterrupt(KEY4),setTime, RISING);
          attachInterrupt(digitalPinToInterrupt(KEY3),setAlrm, RISING);
          break;
      }
  }
}
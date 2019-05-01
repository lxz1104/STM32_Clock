#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <stdio.h>

/* CLK for DS1302 */
#define CLK PB7
/* DAT/IO for DS1302 */
#define DAT PB6
/* RST for DS1302 */
#define RST PB5
/* ms per min */
#define SEC 10000

//初始化外部时钟引脚
ThreeWire DSWire(DAT, CLK, RST);  

/** 
 * 测量数组长度 
 * 参数：int*,int[]
*/
#define GET_SIZE(a) (sizeof(a) / sizeof(a[0]))

/** 
 * 打印日期和时间信息 
 * 参数：RtcDateTime对象
 * 输出格式:Month-Day-Year Hour:Minute:Second
*/
void printDateTime(const RtcDateTime &dt)
{
  char datestring[30];
  snprintf(datestring,
             GET_SIZE(datestring),
             PSTR("%02u-%02u-%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.println(datestring);
}

/** 
 * 初始化外部时钟 
 * 参数：RtcDS1302<className>对象的引用
*/
template <class T>
void initClock(RtcDS1302<T> & Rtc)
{ 
  Serial.begin(9600);  //开启串口并设置波特率为9600

  /** 打印编译完成时间 */
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.print("\t");
  Serial.println(__TIME__);

  Rtc.Begin();  //启动外部时钟

  /* 代码编译完成时间 */
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Serial.print("Local time: ");
  printDateTime(compiled);
  Serial.println();
  /** 给外部时钟初始化 */
  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));  
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (This is not expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
  Serial.println("RTC init success!");
  Serial.println("+------Welcome to use my alarm clock!------+");
}


#endif //_CLOCK_H_
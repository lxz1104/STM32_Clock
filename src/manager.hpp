#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "clock.hpp"
#include "dispaly.hpp"
#include "key.hpp"
#include "music.hpp"

enum Mode{
    SHOW_TIME,SET_TIME
};

/* 外部时钟对象 */
RtcDS1302<ThreeWire> Rtc(DSWire);
/* 数码管对象 */
TM1637Display display(DCLK, DIO);
/* 当前所处模式 */
volatile Mode mode = SHOW_TIME;
//记录分钟
volatile uint8_t array_alarm_time[] = {0,0};
//闹钟是否打开
volatile bool is_alarm_on = false;
//闹钟状态
volatile bool alarm_status = false;

/** 设置时间 */
void setTime();
/** 设置闹钟 */
void setAlrm();
/** 发送数据 */
void sendData();

/** 中断函数内使用的延迟函数 
 * 参数：毫秒数
*/
void delay_in_interrupts(int ms){
    for(int i=0;i< ms;i++){
        delayMicroseconds(1000);
    }
}

/** 
 * 显示时间 
 * 参数:
 *      @parm1：TM1637Display对象；
 *      @parm2：RtcDateTime对象
 * 
*/
void showTime(TM1637Display & display,const RtcDateTime & now){
    int now_time =  now.Hour() * 100 + now.Minute();
	for(int i = 0; i <= 4; ++i) {
		display.showNumberDecEx(now_time, (0x80 >> i), true);
		delay(200);
	}
}

/** 
 * 显示日期
 * 参数:
 *      @parm1：TM1637Display对象；
 *      @parm2：RtcDateTime对象
 * 
*/
void showDate(TM1637Display & display,const RtcDateTime & now){
    int now_time =  now.Month() * 100 + now.Day();
    display.showNumberDecEx(now_time);
}
/** 
 * 扫描按键
 * 返回值：被按下的按键
*/
uint8_t scanKey(){
    /* 按键松开的标志 */
    static uint8_t key_up=1;
	if(key_up && 
            (KEY1_STATUS == HIGH || KEY2_STATUS == HIGH || 
                KEY3_STATUS == HIGH || KEY4_STATUS == HIGH))
	{
		delay(10);  //去抖动 
		key_up = 0;
		if(KEY1_STATUS == HIGH)return KEY1_PRES;
		else if(KEY2_STATUS == HIGH)return KEY2_PRES;
		else if(KEY3_STATUS == HIGH)return KEY3_PRES;
		else if(KEY4_STATUS == HIGH)return KEY4_PRES;
	}else if(KEY1_STATUS==LOW && KEY2_STATUS==LOW && 
                KEY3_STATUS==LOW && KEY4_STATUS==LOW){
                    key_up=1; 
    }
                   	     
	return 0;// 无按键按下
}

/** 播放音乐 */
void playSong(){
  detachInterrupt(KEY4);
  detachInterrupt(KEY3);
  for (size_t x = 0; x < GET_SIZE(tune); x++) //循环音符的次数
  {
    if(scanKey() == KEY2_PRES){
        is_alarm_on = false;
        alarm_status = false;
        /* 绑定中断函数 */
        attachInterrupt(digitalPinToInterrupt(KEY4),setTime, RISING);
        attachInterrupt(digitalPinToInterrupt(KEY3),setAlrm, RISING);
        break;
    }
    tone(BEEP, tune[x]); //依次播放tune数组元素，即每个音符
    delay(400 * duration[x]); //每个音符持续的时间，即节拍duration，400是调整时间的越大，曲子速度越慢，越小曲子速度越快
    noTone(BEEP);//停止当前音符，进入下一音符
  }
}

#endif //_MANAGER_H_
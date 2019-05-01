#ifndef _KEY_H_
#define _KEY_H_

/* 按键1 */
#define KEY1 PA5
/* 按键2 */
#define KEY2 PA6
/* 按键3 */
#define KEY3 PA7
/* 按键1 */
#define KEY4 PB0
/* 按键1按下 */
#define KEY1_PRES 1
/* 按键2按下 */
#define KEY2_PRES 2
/* 按键3按下 */
#define KEY3_PRES 3
/* 按键4按下 */
#define KEY4_PRES 4
/* 按键1状态 */
#define KEY1_STATUS digitalRead(KEY1)
/* 按键2状态 */
#define KEY2_STATUS digitalRead(KEY2)
/* 按键3状态 */
#define KEY3_STATUS digitalRead(KEY3)
/* 按键4状态 */
#define KEY4_STATUS digitalRead(KEY4)
/* 蜂鸣器 */
#define BEEP PC13

/** 初始化按键 */
#define initKey() { \
    pinMode(BEEP,OUTPUT); \
    pinMode(KEY1,INPUT); \
    pinMode(KEY2,INPUT); \
    pinMode(KEY3,INPUT); \
    pinMode(KEY4,INPUT); \
    digitalWrite(BEEP,HIGH);    \
}  \



#endif //_KEY_H_
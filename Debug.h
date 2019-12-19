#ifndef __DEBUG_H__
#define __DEBUG_H__ 

#define DEBUG

#ifdef DEBUG

#define DEBUG_INIT() Serial.begin(9600)
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)

#else

#define DEBUG_INIT()
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)

#endif

#endif

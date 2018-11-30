

#ifdef HW_GPS_DEF
	#define HW_GPS_GLOBE
#else
	#define HW_GPS_GLOBE		extern
#endif

#include <bsp_gpio.h>


#define     HW_GPS_CTRL_PIN_PORT         GPIOA
#define     HW_GPS_RST_PIN              GPIO_Pin_11
#define     HW_GPS_CTRL_PIN             GPIO_Pin_12

HW_GPS_GLOBE void HW_GPS_Init(void);

HW_GPS_GLOBE void HW_GPS_On(void);

HW_GPS_GLOBE void HW_GPS_Off(void);

HW_GPS_GLOBE void HW_GPS_Rst(void);


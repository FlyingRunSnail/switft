

#ifdef HW_BATTERY_DEF
	#define HW_BATTERY_GLOBE
#else
	#define HW_BATTERY_GLOBE		extern
#endif

#include <bsp_gpio.h>


#define		HW_BATTERY_READ_PIN_PORT         GPIOB
#define		HW_BATTERY_READ_PIN              GPIO_Pin_12


typedef enum
{
    eBattery_in = 0,
    eBattery_ext,

    eBattery_cnt
}battery_id_t;


HW_BATTERY_GLOBE void HW_BATTERY_Init(void);

HW_BATTERY_GLOBE void HW_BATTERY_Read(INT8U *buf, INT32U len);


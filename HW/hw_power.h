

#ifdef HW_POWER_DEF
	#define HW_POWER_GLOBE
#else
	#define HW_POWER_GLOBE		extern
#endif

#include <bsp_gpio.h>


#define     HW_12V_PIN_PORT        GPIOF
#define     HW_12V_PIN             GPIO_Pin_6

#define     HW_BAT_PIN_PORT        GPIOF
#define     HW_BAT_PIN             GPIO_Pin_7

#define     HW_AP_PIN_PORT         GPIOA
#define     HW_AP_PIN              GPIO_Pin_3

#define		HW_4G_PIN_PORT         GPIOB
#define		HW_4G_PIN              GPIO_Pin_13

#define		HW_MESH_PIN_PORT       GPIOD
#define		HW_MESH_PIN            GPIO_Pin_7

HW_POWER_GLOBE void HW_POWER_Init(void);

HW_POWER_GLOBE void HW_12V_On(void);

HW_POWER_GLOBE void HW_12V_Off(void);

HW_POWER_GLOBE void HW_BAT_On(void);

HW_POWER_GLOBE void HW_BAT_Off(void);

HW_POWER_GLOBE void HW_AP_On(void);

HW_POWER_GLOBE void HW_AP_Off(void);

HW_POWER_GLOBE void HW_4G_On(void);

HW_POWER_GLOBE void HW_4G_Off(void);

HW_POWER_GLOBE void HW_MESH_On(void);

HW_POWER_GLOBE void HW_MESH_Off(void);


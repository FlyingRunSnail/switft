

#ifdef HW_LED_DEF
	#define HW_LED_GLOBE
#else
	#define HW_LED_GLOBE		extern
#endif

#include <bsp_gpio.h>

// LED1
#define		HW_LED1_PIN_PORT			GPIOB
#define		HW_LED1_PIN					GPIO_Pin_8

//LED2
#define		HW_LED2_PIN_PORT			GPIOB
#define		HW_LED2_PIN					GPIO_Pin_9


HW_LED_GLOBE void HW_LED_Init(void);

HW_LED_GLOBE void HW_LED1_OFF(void);

HW_LED_GLOBE void HW_LED2_OFF(void);

HW_LED_GLOBE void HW_LED1_ON(void);

HW_LED_GLOBE void HW_LED2_ON(void);

HW_LED_GLOBE void HW_LED1_Flick(void);

HW_LED_GLOBE void HW_LED2_Flick(void);


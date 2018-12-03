

#ifdef HW_PHY_DEF
	#define HW_PHY_GLOBE
#else
	#define HW_PHY_GLOBE		extern
#endif

#include <bsp_gpio.h>


#define     HW_PHY_RST_PIN_PORT         GPIOF
#define     HW_PHY_RST_PIN              GPIO_Pin_9

HW_PHY_GLOBE void HW_PHY_Rst_Init(void);

HW_PHY_GLOBE void HW_PHY_Rst(void);


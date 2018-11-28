

#ifdef HW_BUZZER_DEF
	#define HW_BUZZER_GLOBE
#else
	#define HW_BUZZER_GLOBE		extern
#endif

#include <bsp_gpio.h>


#define		HW_BUZZER_PIN_PORT			GPIOB
#define		HW_BUZZER_PIN				GPIO_Pin_14



HW_BUZZER_GLOBE void HW_BUZZER_Init(void);

HW_BUZZER_GLOBE void HW_BUZZER_On(void);

HW_BUZZER_GLOBE void HW_BUZZER_Off(void);

HW_BUZZER_GLOBE void HW_BUZZER_Flick(void);



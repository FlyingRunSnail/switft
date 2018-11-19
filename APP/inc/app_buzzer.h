

#ifdef APP_BUZZER_DEF
	#define APP_BUZZER_GLOBE
#else
	#define APP_BUZZER_GLOBE		extern
#endif

#include <hw_buzzer.h>
#include <devdef.h>




APP_BUZZER_GLOBE void APP_BUZZER_Init(void);

APP_BUZZER_GLOBE void APP_BUZZER_On(void);

APP_BUZZER_GLOBE void APP_BUZZER_Off(void);

APP_BUZZER_GLOBE void APP_BUZZER_Flick(void);

APP_BUZZER_GLOBE void APP_BUZZER_BeepOpt(INT16U beep_ms,INT16U period_ms,INT8U flick_flag,INT32U cnt);

APP_BUZZER_GLOBE void APP_BUZZER_SingleBeep(INT16U beep_ms);

APP_BUZZER_GLOBE void APP_BUZZER_ContinualBeep(INT16U beep_ms,INT16U period_ms,INT32U beep_cnt);

APP_BUZZER_GLOBE void APP_BUZZER_Deal(INT32U period_ms,INT16U *delay_ms);


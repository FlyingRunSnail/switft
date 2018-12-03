

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


typedef struct st_battery_base_info
{
    INT16U total_voltage;
    INT16S current;
    INT16U remainder_capcity;
    INT16U std_capcity;
    INT16U recycle_times;
    INT16U manufacture_date;
    INT16U proportionate_status;
    INT16U proportionate_status_h;
    INT16U protect_status;
    INT8U  software_ver;
    INT8U  rsoc;
    INT8U  fet_ctrl_status;
    INT8U  bat_cnt;
    INT8U  ntc_cnt; 

}battery_base_info_t;


HW_BATTERY_GLOBE void HW_BATTERY_Init(void);

HW_BATTERY_GLOBE void HW_BATTERY_Read(INT8U *buf, INT32U len);


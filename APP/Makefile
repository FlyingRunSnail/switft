BINPATH=
OUTPATH=

CC=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
#override FLOAT_TYPE = hard
override FLOAT_TYPE = soft
endif
endif

vpath %.c src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m3

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iinc -I$(BASEDIR)/Lib/CMSIS/ -I$(BASEDIR)/Lib/CMSIS/DeviceSupport/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/ARM-Cortex-M3/GNU/
CFLAGS += -I$(BASEDIR)/APP/inc/
CFLAGS += -I$(BASEDIR)/HW/
CFLAGS += -I$(BASEDIR)/uCOS/uC-LIB/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2xx_StdPeriph_Driver/inc/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2x7_ETH_Driver/inc/
CFLAGS += -I$(BASEDIR)/BSP/inc/
CFLAGS += -I$(BASEDIR)/BSP/bsp_stm32F2X7/inc/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Source/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Ports/ARM-Cortex-M3/Generic/GNU/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-SHELL/inc/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/port/STM32/include/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/src/include/lwip/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/src/include/ipv4/lwip/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/src/include/netif/

# Default to STM32F40_41xxx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F2xx
endif

CFLAGS += -D$(DEVICE_DEF)

# Sources
SRCS = app_beacon.c \
	app_buzzer.c \
	app_datetime.c \
	app_eleccoder.c \
	app_gps.c \
	app_log.c \
	app_monitor.c \
	app_shell.c \
	app_tool.c \
	cmd_d.c \
	cmd_help.c \
	cmd_m.c \
	cmd_reset.c \
	cmd_ver.c \
	lwip.c \
	main.c \
	os_app_hooks.c

OBJS = $(SRCS:.c=.o)
LIBNAME = $(OUTPATH)/libapp.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $(addprefix $(OUTPATH)/, $(notdir $@)) $^

$(LIBNAME): $(OBJS) $(S_OBJS)
	$(AR) -r $@ $(addprefix $(OUTPATH)/, $(OBJS))

clean:
	rm -f $(LIBNAME)
	rm -f $(addprefix $(OUTPATH)/,$(OBJS))
	

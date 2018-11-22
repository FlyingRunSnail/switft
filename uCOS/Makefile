BINPATH=
OUTPATH=

CC=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar
AS=$(BINPATH)arm-none-eabi-as

CPU = -mcpu=cortex-m3 -mthumb

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

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iport/STM32/include/
CFLAGS += -Iport/STM32/include/arch/
CFLAGS += -Isrc/include/
CFLAGS += -Isrc/include/lwip/
CFLAGS += -Isrc/include/ipv4/
CFLAGS += -Isrc/include/ipv4/lwip/
CFLAGS += -Isrc/include/netif/
CFLAGS += -I$(BASEDIR)/Lib/CMSIS/ 
CFLAGS += -I$(BASEDIR)/Lib/CMSIS/DeviceSupport/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/ARM-Cortex-M3/GNU/
CFLAGS += -I$(BASEDIR)/APP/inc/
CFLAGS += -I$(BASEDIR)/uCOS/uC-LIB/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2xx_StdPeriph_Driver/inc/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2x7_ETH_Driver/inc/
CFLAGS += -I$(BASEDIR)/BSP/inc/
CFLAGS += -I$(BASEDIR)/BSP/bsp_stm32F2X7/inc/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Source/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Ports/ARM-Cortex-M3/Generic/GNU/

# Default to STM32F40_41xxx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F2xx
endif

CFLAGS += -D$(DEVICE_DEF)

# Sources
SRCS = uC-CPU/ARM-Cortex-M3/GNU/cpu_c.c \
	uC-CPU/cpu_core.c \
	uC-LIB/lib_ascii.c \
	uC-LIB/lib_math.c \
	uC-LIB/lib_mem.c \
	uC-LIB/lib_str.c \
	uCOS-III/Ports/ARM-Cortex-M3/Generic/GNU/os_cpu_c.c \
	uCOS-III/Source/os_cfg_app.c \
	uCOS-III/Source/os_core.c \
	uCOS-III/Source/os_dbg.c \
	uCOS-III/Source/os_flag.c \
	uCOS-III/Source/os_int.c \
	uCOS-III/Source/os_mem.c \
	uCOS-III/Source/os_msg.c \
	uCOS-III/Source/os_mutex.c \
	uCOS-III/Source/os_pend_multi.c \
	uCOS-III/Source/os_prio.c \
	uCOS-III/Source/os_q.c \
	uCOS-III/Source/os_sem.c \
	uCOS-III/Source/os_stat.c \
	uCOS-III/Source/os_task.c \
	uCOS-III/Source/os_tick.c \
	uCOS-III/Source/os_time.c \
	uCOS-III/Source/os_tmr.c \
	uCOS-III/Source/os_var.c
	

S_SRCS = uC-CPU/ARM-Cortex-M3/GNU/cpu_a.s \
	uC-LIB/Ports/ARM-Cortex-M3/GNU/lib_mem_a.s \
	uCOS-III/Ports/ARM-Cortex-M3/Generic/GNU/os_cpu_a.s

OBJS = $(SRCS:.c=.o)
S_OBJS = $(S_SRCS:.s=.o)

MAKEDEFS = FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)

LIBNAME = $(OUTPATH)/libucosiii.a

all: $(LIBNAME) shell

$(LIBNAME): $(OBJS) $(S_OBJS)
	$(AR) -r $@ $(addprefix $(OUTPATH)/, $(notdir $(OBJS))) $(addprefix $(OUTPATH)/, $(notdir $(S_OBJS)))

shell:
	$(MAKE) -C uCOS-SHELL $(MAKEDEFS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $(addprefix $(OUTPATH)/, $(notdir $@)) $^

%.o : %.s
	$(AS) $(CPU) -o $(addprefix $(OUTPATH)/, $(notdir $@)) $^

clean:
	rm -f $(LIBNAME)
	rm -f $(addprefix $(OUTPATH)/,$(notdir $(OBJS)))
	rm -f $(addprefix $(OUTPATH)/,$(notdir $(S_OBJS)))
	$(MAKE) clean -C uCOS-SHELL $(MAKEDEFS)


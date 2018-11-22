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
SRCS = src/api/api_lib.c \
	src/api/api_msg.c \
	src/api/err.c \
	src/api/netbuf.c \
	src/api/netdb.c \
	src/api/netifapi.c \
	src/api/sockets.c \
	src/api/tcpip.c \
	src/core/def.c \
	src/core/dhcp.c \
	src/core/dns.c \
	src/core/init.c \
	src/core/mem.c \
	src/core/memp.c \
	src/core/netif.c \
	src/core/pbuf.c \
	src/core/raw.c \
	src/core/stats.c \
	src/core/sys.c \
	src/core/tcp.c \
	src/core/tcp_in.c \
	src/core/tcp_out.c \
	src/core/timers.c \
	src/core/udp.c \
	src/core/ipv4/autoip.c \
	src/core/ipv4/icmp.c \
	src/core/ipv4/igmp.c \
	src/core/ipv4/inet.c \
	src/core/ipv4/inet_chksum.c \
	src/core/ipv4/ip.c \
	src/core/ipv4/ip_addr.c \
	src/core/ipv4/ip_frag.c \
	src/core/snmp/asn1_dec.c \
	src/core/snmp/asn1_enc.c \
	src/core/snmp/mib2.c \
	src/core/snmp/mib_structs.c \
	src/core/snmp/msg_in.c \
	src/core/snmp/msg_out.c \
	src/netif/etharp.c


OBJS = $(SRCS:.c=.o)
LIBNAME = $(OUTPATH)/liblwip.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $(addprefix $(OUTPATH)/, $(notdir $@)) $^

$(LIBNAME): $(OBJS) $(S_OBJS)
	$(AR) -r $@ $(addprefix $(OUTPATH)/, $(notdir $(OBJS)))

clean:
	rm -f $(LIBNAME)
	rm -f $(addprefix $(OUTPATH)/,$(notdir $(OBJS)))


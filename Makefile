TARGET		:= PokemonControl
BUILD		:= build
SOURCES		:= source
INCLUDES	:= include

include $(DEVKITPRO)/libnx/switch_rules

CFLAGS		:= -g -Wall -O2 -ffunction-sections $(ARCH)
CXXFLAGS	:= $(CFLAGS)
ASFLAGS		:= $(ARCH)
LDFLAGS		:= -specs=switch.specs $(ARCH)

LIBS		:= -lnx -lm

include $(DEVKITPRO)/libnx/switch_target


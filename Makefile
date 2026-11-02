#---------------------------------------------------------------------------------
# CLEAR IMPLICIT RULES
#---------------------------------------------------------------------------------
.SUFFIXES:

#---------------------------------------------------------------------------------
# TARGET & DIRECTORIES
#---------------------------------------------------------------------------------
TARGET		:= PokemonControl
BUILD		:= build
SOURCES		:= source
INCLUDES	:= include

#---------------------------------------------------------------------------------
# DEVKITPRO ENVIRONMENT & FALLBACKS
#---------------------------------------------------------------------------------
export DEVKITPRO	?= /opt/devkitpro
export DEVKITA64	?= $(DEVKITPRO)/devkitA64

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:= $(CURDIR)/$(TARGET)
export TOPDIR	:= $(CURDIR)

export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export DEPSDIR	:= $(CURDIR)/$(BUILD)

# Automatically find all C, C++, and assembly files regardless of casing
CFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c $(dir)/*.C)))
CPPFILES	:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp $(dir)/*.CPP)))
SFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s $(dir)/*.S)))

export OFILES	:= $(CPPFILES:.cpp=.o) $(CPPFILES:.CPP=.o) $(CFILES:.c=.o) $(CFILES:.C=.o) $(SFILES:.s=.o) $(SFILES:.S=.o)
export INCLUDE	:= $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(CURDIR)/$(BUILD)

include $(DEVKITPRO)/libnx/switch_rules

.PHONY: clean all

all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nro

else

#---------------------------------------------------------------------------------
# COMPILER & LINKER FLAGS
#---------------------------------------------------------------------------------
CFLAGS		:= -g -Wall -O2 -ffunction-sections $(ARCH) $(INCLUDE)
CXXFLAGS	:= $(CFLAGS)
ASFLAGS		:= $(ARCH)
LDFLAGS		:= -specs=switch.specs $(ARCH)

LIBS		:= -lnx -lm

#---------------------------------------------------------------------------------
# MAIN TARGET RULE
#---------------------------------------------------------------------------------
$(OUTPUT).nro	: $(OUTPUT).elf
$(OUTPUT).elf	: $(OFILES)

-include $(DEPSDIR)/*.d

endif

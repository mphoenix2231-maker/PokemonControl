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

export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
		   $(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:= $(CURDIR)/$(BUILD)

CFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export OFILES	:= $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE	:= $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
		   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
		   -I$(CURDIR)/$(BUILD)

export LIBPATHS	:= $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

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
LDFLAGS		:= -specs=switch.specs $(ARCH) $(LIBPATHS)

LIBS		:= -lnx -lm

#---------------------------------------------------------------------------------
# MAIN TARGET RULE
#---------------------------------------------------------------------------------
$(OUTPUT).nro	: $(OUTPUT).elf
$(OUTPUT).elf	: $(OFILES)

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

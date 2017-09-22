cpu = x86

EMU_X86_CFLAGS = -DEMU_X86
AVR_8BIT_CFLAGS = -DAVR_8BIT

ifeq "$(cpu)" "avr"
CFLAGS = $(AVR_8BIT_CFLAGS)
else
ifeq "$(cpu)" "x86"
CFLAGS = $(EMU_X86_CFLAGS)
endif
endif
TARGET ?= a.out
SRC_DIRS ?=  ./srtos/inc  ./srtos/src  ./app/samples/emu_x86

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LDFLAGs=-m32

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS)   $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)

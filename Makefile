CC  := $(COMPILER_DIRS)arm-none-eabi-gcc
CXX := $(COMPILER_DIRS)arm-none-eabi-g++

TARGET_EXEC ?= app.elf
BUILD_DIR   ?= ./build
SRC_DIRS    := src
LDPATH      := $(MKDIR)MDR32F9Qx.ld

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.S)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := -mcpu=cortex-m3 \
			-mthumb \
			-O0 \
			-ggdb3 \
			-ffunction-sections \
			-fdata-sections \
			-Wall \
			-Wextra \
			-Wshadow \
			-Wredundant-decls \
			-Wno-missing-field-initializers \
			-Wconversion \
			-Wno-packed-bitfield-compat \
			-pipe \
			-Wl,--print-memory-usage \
			-Wcpp \
			$(INC_FLAGS) \
			--specs=nosys.specs \
			--specs=nano.specs \
			-Wl,--gc-sections \
			-ffreestanding \
			-fno-rtti \
			-fno-exceptions \
			-fno-threadsafe-statics \
			-DUSE_MDR1986VE9x \
			-D__STARTUP_CLEAR_BSS \
			-MMD \
			-MP 

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@echo "start linking"
	$(CXX) -T $(LDPATH) $(CPPFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.S.o: %.S
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS)-c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
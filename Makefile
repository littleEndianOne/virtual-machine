CC := gcc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Wall -Wextra
LDFLAGS :=
LDLIBS := -pthread

BUILD_DIR := build
TARGET := $(BUILD_DIR)/vm-tests

VM_SOURCES := \
	VM/vm_array_operations.c \
	VM/vm_cpu.c \
	VM/vm_event_buffer.c \
	VM/vm_opstack_operations.c \
	VM/vm_progmem_operations.c \
	VM/vm_string_operations.c \
	VM/vm_variable_operations.c

.PHONY: all test clean

all: $(TARGET)

$(TARGET): src/*.c $(VM_SOURCES) VM\ Utility/*.c Test\ Utility/*.c Tests/*.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) src/*.c $(VM_SOURCES) "VM Utility"/*.c "Test Utility"/*.c Tests/*.c $(LDLIBS) -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

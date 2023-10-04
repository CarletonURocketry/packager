CC = qcc
OUT = packager
TARGET = gcc_ntoaarch64le

ifeq ($(OS), Windows_NT)
ENV_CMD = powershell ~/qnx710/qnxsdp-env.bat
else
ENV_CMD = ~/qnx710/qnxsdp-env.sh
endif

### NECESSARY QNX LIBRARIES ###
INCLUDE_DIRS += ~/qnx710/target/qnx7/usr/include/
# Contains QNX specific libraries for hardware interaction
INCLUDE_DIRS += ~/qnx710/host/$(OS)/x86_64/usr/lib/gcc/aarch64-unknown-nto-qnx7.1.0/8.3.0/include

### COMPILER OPTIONS ###
CLFAGS += -V $(TARGET)
CFLAGS += $(patsubst %,-I%,$(INCLUDE_DIRS))

### WARNINGS ###
# (see https://gcc.gnu.org/onlinedocs/gcc-6.3.0/gcc/Warning-Options.html)
CFLAGS += -Wall -Wextra -Wshadow -Wundef -Wformat=2 -Wtrampolines -Wfloat-equal
CFLAGS += -Wbad-function-cast -Wstrict-prototypes -Wpacked
CFLAGS += -Wno-aggressive-loop-optimizations -Wmissing-prototypes -Winit-self
CFLAGS += -Wmissing-declarations -Wmissing-format-attribute -Wunreachable-code
CFLAGS += -Wshift-overflow=2 -Wduplicated-cond -Wpointer-arith -Wwrite-strings
CFLAGS += -Wnested-externs -Wcast-align -Wredundant-decls
CFLAGS += -Werror=implicit-function-declaration -Wlogical-not-parentheses
CFLAGS += -Wlogical-op -Wold-style-definition -Wcast-qual -Wdouble-promotion
CFLAGS += -Wunsuffixed-float-constants -Wmissing-include-dirs -Wnormalized
CLFAGS += -Wdisabled-optimization -Wsuggest-attribute=const

### RULES ###
env:
	@echo Initializing environment...
	$(ENV_CMD)

%.o: %.c env
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(OUT)
	rm *.o

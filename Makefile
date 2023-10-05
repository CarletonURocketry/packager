CC = qcc
TARGET = gcc_ntoaarch64le
CSTD = c11

### ARTIFACTS ###
OUT = packager
OBJDIR = obj

### SHELL SETTINGS ###
ifeq ($(OS), Windows_NT)
SHELL = powershell.exe
else
SHELL = bash
endif

### DETECT PROPER ENVIRONMENT ###
ENV_FILES = ~/qnx710/qnxsdp-env.bat, ~/qnx710/qnxsdp-env.sh, ./env.ps1
ifeq ($(QNX_CONFIGURATION),)
$(error Please make sure you run one of the following environment files: $(ENV_FILES) )
endif

### NECESSARY QNX LIBRARIES ###
# Contains QNX specific libraries for hardware interaction
INCLUDE_DIRS += $(QNX_HOST)/usr/lib/gcc/aarch64-unknown-nto-qnx7.1.0/8.3.0/include
INCLUDE_DIRS += $(QNX_TARGET)/usr/include

### COMPILER OPTIONS ###
CLFAGS += -V $(TARGET)
CFLAGS += $(patsubst %,-I%,$(INCLUDE_DIRS))
CFLAGS += -std=$(CSTD)

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
$(OBJDIR):
	@mkdir -p $@

%.o: %.c $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -r $(OBJDIR)
	rm $(OUT)

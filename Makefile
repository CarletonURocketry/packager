# As an aside, the 'make' command binary included with QNX is able to run the Makefile as though it is being run on a
# POSIX system. This even works on Windows, as QNX also includes binaries for common utilities (echo, mkdir, etc.) that
# run exactly like their Linux counterparts. As such, this Makefile is not specifically designed to be OS agnostic, but
# rather is by the simple fact that users have access to this QNX binary for 'make' and other utilities it needs.
CC = qcc
CSTD = c11
OUT = packager.exe

### SOURCE FILES ###
SRCDIRS = src
SRCFILES = $(foreach D,$(SRCDIRS),$(wildcard $(D)/*.c))
OBJFILES = $(patsubst %.c,%.o,$(SRCFILES))

### DETECT PROPER ENVIRONMENT ###
ifeq ($(OS), Windows_NT)
ENV_FILE = ~/qnx710/qnxsdp-env.bat
else
ENV_FILE = ~/qnx710/qnxsdp-env.sh
endif

ifeq ($(QNX_CONFIGURATION),)
$(error Please make sure you run '$(ENV_FILE)' in order to properly set up the QNX development environment.)
endif

### NECESSARY QNX LIBRARIES ###
INCLUDE_DIRS += src/include

### COMPILER OPTIONS ###
CFLAGS += $(patsubst %,-I%,$(INCLUDE_DIRS))
# Have to do annoying subst because there is a "- " artifact for some reason.
CFLAGS += $(subst - ,,$(MAKEFLAGS))
CFLAGS += -std=$(CSTD)
CFLAGS += $(DEPENDENCY_FLAGS)

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
CFLAGS += -Wdisabled-optimization -Wsuggest-attribute=const

### RULES ###
all: $(OBJFILES)
	$(CC) $^ -o $(OUT)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@rm $(OUT)
	@rm $(OBJFILES)

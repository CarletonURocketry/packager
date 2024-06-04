### WARNINGS ###
WARNINGS += -Wall -Wextra -Wshadow -Wundef -Wformat=2 -Wtrampolines -Wfloat-equal
WARNINGS += -Wbad-function-cast -Wstrict-prototypes -Wpacked
WARNINGS += -Wno-aggressive-loop-optimizations -Winit-self
WARNINGS += -Wmissing-format-attribute -Wunreachable-code
WARNINGS += -Wshift-overflow=2 -Wduplicated-cond -Wpointer-arith -Wwrite-strings
WARNINGS += -Wnested-externs -Wcast-align -Wredundant-decls
WARNINGS += -Werror=implicit-function-declaration -Wlogical-not-parentheses
WARNINGS += -Wlogical-op -Wold-style-definition -Wcast-qual -Wdouble-promotion
WARNINGS += -Wunsuffixed-float-constants -Wmissing-include-dirs -Wnormalized
WARNINGS += -Wdisabled-optimization -Wsuggest-attribute=const

STD = gnu11
CFLAGS = -std=$(STD) -D__DOXYGEN__=0

### INFORMATION FOR LINTING ###
PROJECT_ROOT = $(abspath .)
SRCDIRS += $(PROJECT_ROOT)/src
SRCFILES := $(filter-out $(SRCDIRS)/main.c,$(wildcard $(SRCDIRS)/*.c))

TESTDIR += $(PROJECT_ROOT)/tests
TESTFILES += $(wildcard $(TESTDIR)/*.c)
TESTBINS = $(patsubst %.c,%,$(TESTFILES))

test: WARNINGS = 

$(TESTBINS):
	@gcc $(CFLAGS) $(WARNINGS) $(SRCFILES) $@.c -o $@
	$@

test: $(TESTBINS)

clean:
	@rm $(TESTBINS)

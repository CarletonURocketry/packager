### WARNINGS ###
WARNINGS += -Wall -Wextra -Wshadow -Wundef -Wformat=2 -Wtrampolines -Wfloat-equal
WARNINGS += -Wbad-function-cast -Wstrict-prototypes -Wpacked
WARNINGS += -Wno-aggressive-loop-optimizations -Wmissing-prototypes -Winit-self
WARNINGS += -Wmissing-declarations -Wmissing-format-attribute -Wunreachable-code
WARNINGS += -Wshift-overflow=2 -Wduplicated-cond -Wpointer-arith -Wwrite-strings
WARNINGS += -Wnested-externs -Wcast-align -Wredundant-decls
WARNINGS += -Werror=implicit-function-declaration -Wlogical-not-parentheses
WARNINGS += -Wlogical-op -Wold-style-definition -Wcast-qual -Wdouble-promotion
WARNINGS += -Wunsuffixed-float-constants -Wmissing-include-dirs -Wnormalized
WARNINGS += -Wdisabled-optimization -Wsuggest-attribute=const

### INFORMATION FOR LINTING ###
PROJECT_ROOT = $(abspath .)
SRCDIRS += $(PROJECT_ROOT)/src
SRCFILES = $(wildcard $(SRCDIRS)/*.c)

INCLUDE_DIRS += $(PROJECT_ROOT)/src/include
INCLUDE = $(patsubst %,-I%,$(INCLUDE_DIRS))

LINT_OUTPUT=deleteme

# Compile using regular gcc and immediately remove output binary. Just to see warnings.
# __DOXYGEN__ is defined as 0 to avoid undef errors but still compile regularly without special treatment for doc
# generation
lint:
	gcc -D__DOXYGEN__=0 $(WARNINGS) $(INCLUDE) $(SRCFILES) -o $(LINT_OUTPUT)
	@rm $(LINT_OUTPUT)


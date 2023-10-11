ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

# use -i info
define PINFO
PINFO DESCRIPTION=CUinSpace Packager
endef

NAME=packager

# Project includes
EXTRA_INCVPATH+= $(PROJECT_ROOT)/src/include
# EXCLUDE_OBJS= exclude objects

### Source files
EXTRA_SRCVPATH+= $(PROJECT_ROOT)/src

$(info VAR='$(EXTRA_INCVPATH)')
$(info VAR='$(SRCVPATH)')
$(info VAR='$(EXTRA_SRCVPATH)')
$(info VAR='$(PROJECT)')
$(info VAR='$(PROJECT_ROOT)')

### COMPILER OPTIONS ###
CCFLAGS+= -std cc11 -02 -Wall

include $(MKFILES_ROOT)/qtargets.mk

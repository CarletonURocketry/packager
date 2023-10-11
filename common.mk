ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

# use -i info
define PINFO
PINFO DESCRIPTION=CUinSpace Packager
endef

NAME=packager

#include $(MKFILES_ROOT)/qtargets.mk

#### PROJECT SPECIFIC ####

# Project includes
EXTRA_INCVPATH+= $(PROJECT_ROOT)/src/include
# EXCLUDE_OBJS+= Exclude objects

### SOURCE FILES ###
EXTRA_SRCVPATH+= $(PROJECT_ROOT)/src

### COMPILER OPTIONS ###
CCFLAGS+= -std=c11 -O2 -Wall

include $(MKFILES_ROOT)/qtargets.mk

$(info VAR='$(SRCVPATH)')
$(info VAR='$(EXTRA_SRCVPATH)')
$(info VAR='$(SRCVPATH)')

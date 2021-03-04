CIRCLE_PORT_DIR := $(call select_from_ports,circle)
CIRCLE_DIR      := $(CIRCLE_PORT_DIR)/src/lib/circle

LIBS += libc

# header files
#INC_DIR += $(CIRCLE_DIR)/include
include $(call select_from_repositories,lib/import/import-circle.mk)

# source files
CIRCLE_FILES = bcm54213.cc \
			   macaddress.cc \
			   bcmpropertytags.cc \
			   bcmmailbox.cc \
			   assert.cc \
			   synchronize64.cc \
			   netdevice.cc

SRC_CC += $(addprefix $(CIRCLE_DIR)/lib/,$(CIRCLE_FILES))

# SRC_CC += $(notdir $(wildcard $(CIRCLE_DIR)/lib/*.cc))
# SRC_CC += $(notdir $(wildcard $(CIRCLE_DIR)/lib/net/*.cc))

vpath %.cc       $(CIRCLE_DIR)/lib
vpath %.cc       $(CIRCLE_DIR)/lib/net

CC_OPT      = -DRASPPI=4 -DAARCH=64

# usb library is needed for rpi3 -> not necessary right now
#SRC_CC += $(notdir $(wildcard $(CIRCLE_DIR)/lib/usb/*.cc))
#vpath %.cc       $(CIRCLE_DIR)/lib/usb

# assembly files
#SRC_S += $(notdir $(wildcard $(CIRCLE_DIR)/lib/*.S))
#vpath %.S        $(CIRCLE_DIR)/lib

#SHARED_LIB = yes

CC_CXX_WARN_STRICT = -Wextra -Weffc++ -Werror

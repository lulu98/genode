TARGET   = rpi4_timer_drv
REQUIRES = rpi4
GEN_DIR := $(dir $(call select_from_repositories,src/timer/main.cc))
INC_DIR += $(GEN_DIR)/epit
SRC_CC  += epit/time_source.cc epit/rpi4/timer.cc

include $(GEN_DIR)/target.inc

vpath %.cc $(GEN_DIR)

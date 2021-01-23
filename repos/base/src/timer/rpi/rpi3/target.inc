TARGET   = rpi3_timer_drv
REQUIRES = rpi3
GEN_DIR := $(dir $(call select_from_repositories,src/timer/main.cc))
INC_DIR += $(GEN_DIR)/rpi
SRC_CC  += rpi/time_source.cc rpi/rpi3/timer.cc

include $(GEN_DIR)/target.inc

vpath %.cc $(GEN_DIR)

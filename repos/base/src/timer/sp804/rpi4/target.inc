TARGET   = rpi4_timer_drv
REQUIRES = arm_v8
GEN_DIR := $(BASE_DIR)/src/timer
INC_DIR += $(GEN_DIR)/sp804
SRC_CC  += sp804/time_source.cc sp804/rpi4/timer.cc

include $(GEN_DIR)/target.inc

vpath %.cc $(GEN_DIR)

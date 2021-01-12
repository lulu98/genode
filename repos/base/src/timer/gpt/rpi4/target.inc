TARGET   = rpi4_timer_drv
REQUIRES = arm_v8
GEN_DIR := $(BASE_DIR)/src/timer
INC_DIR += $(GEN_DIR)/gpt
SRC_CC  += gpt/time_source.cc gpt/rpi4/timer.cc

include $(GEN_DIR)/target.inc

vpath %.cc $(GEN_DIR)

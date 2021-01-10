SPECS       += arm_v8 arm 32bit
REP_INC_DIR += include/spec/arm_v8
REP_INC_DIR += include/spec/arm_v7
REP_INC_DIR += include/spec/arm

CC_MARCH    ?= -march=armv8-a

include $(BASE_DIR)/mk/spec/32bit.mk

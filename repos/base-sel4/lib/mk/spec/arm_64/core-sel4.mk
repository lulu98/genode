GEN_SRC_CC = platform_services.cc

REP_SRC_CC = \
             spec/arm_64/boot_info.cc \
             spec/arm_64/irq.cc \
             spec/arm_64/platform.cc \
             spec/arm_64/platform_thread.cc \
             spec/arm_64/thread.cc \
             spec/arm_64/vm_space.cc

INC_DIR += $(REP_DIR)/src/core/spec/arm_64

include $(REP_DIR)/lib/mk/core-sel4.inc

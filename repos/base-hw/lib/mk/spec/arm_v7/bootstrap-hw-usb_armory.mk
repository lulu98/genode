INC_DIR += $(REP_DIR)/src/bootstrap/spec/usb_armory

SRC_S   += bootstrap/spec/arm/crt0.s

SRC_CC  += bootstrap/spec/arm/cortex_a8_mmu.cc
SRC_CC  += bootstrap/spec/arm/cpu.cc
SRC_CC  += bootstrap/spec/arm/imx_tzic.cc
SRC_CC  += bootstrap/spec/usb_armory/platform.cc
SRC_CC  += bootstrap/spec/arm/arm_v7_cpu.cc
SRC_CC  += hw/spec/32bit/memory_map.cc

include $(REP_DIR)/lib/mk/bootstrap-hw.inc

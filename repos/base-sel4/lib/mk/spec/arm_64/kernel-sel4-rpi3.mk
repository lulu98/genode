PLAT  := bcm2837
CPU   := cortex-a53
override BOARD := rpi3

-include $(REP_DIR)/lib/mk/spec/arm/kernel-sel4-arm_v8a.inc

PLAT  := bcm2711
CPU   := cortex-a53
override BOARD := rpi4

-include $(REP_DIR)/lib/mk/spec/arm/kernel-sel4-arm_v8a.inc

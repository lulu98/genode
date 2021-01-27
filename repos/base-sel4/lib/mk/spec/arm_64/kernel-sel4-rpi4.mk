PLAT  := bcm2711
CPU   := cortex-a72
override BOARD := rpi4

-include $(REP_DIR)/lib/mk/spec/arm_64/kernel-sel4-arm_v8a.inc

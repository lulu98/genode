PLAT := bcm2837
ARCH := arm

SEL4_ARCH := aarch32
PLAT_BOARD := /rpi3
SEL4_WORDBITS := 32

include $(REP_DIR)/lib/mk/syscall-sel4.inc

PLAT := bcm2711
ARCH := arm64

SEL4_ARCH := aarch64
PLAT_BOARD := /rpi4
SEL4_WORDBITS := 64

include $(REP_DIR)/lib/mk/syscall-sel4.inc

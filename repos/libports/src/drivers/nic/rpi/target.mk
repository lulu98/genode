REQUIRES = arm_v8a
TARGET   = rpi_nic_drv
LIBS    += base circle
SRC_CC  += main.cc

vpath main.cc $(PRG_DIR)

INC_DIR += $(PRG_DIR)

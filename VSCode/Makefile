# Project Name
TARGET = GuitarMultiEffect

# Sources
CPP_SOURCES = src/main.cpp

ifeq ($(DEBUG), 1)
OPT = -O0
else
OPT = -Os
endif

#https://forum.electro-smith.com/t/debugging-app-in-sram-daisy/3213
#https://github.com/bkshepherd/DaisySeedProjects/blob/main/Software/GuitarPedal/Makefile
# APP_TYPE = BOOT_SRAM

LDFLAGS += -u _printf_float

# Library Locations
LIBDAISY_DIR ?= include/framework/libDaisy

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
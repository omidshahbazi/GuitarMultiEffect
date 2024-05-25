# Project Name
TARGET = GuitarMultiEffect

# Sources
CPP_SOURCES = src/main.cpp

ifeq ($(DEBUG), 1)
OPT = -Os
else
OPT = -O2
endif

LDFLAGS += -u _printf_float

# Library Locations
LIBDAISY_DIR ?= include/framework/libDaisy

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core

include $(SYSTEM_FILES_DIR)/Makefile
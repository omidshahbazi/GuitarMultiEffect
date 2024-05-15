# Project Name
TARGET = GuitarMultiEffect

# Sources
CPP_SOURCES = src/main.cpp

# Library Locations
LIBDAISY_DIR ?= include/framework/libDaisy

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float

ifeq ($(DEBUG), 1)
OPT = -O0
endif
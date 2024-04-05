TARGET ?= binder_test

CURRENT_PATH := $(shell pwd)
KERNELDIR := ../../../kernel/
MODULEDIR := ../../../modules/binder/
OUT_DIR := $(CURRENT_PATH)/../../../out

COMPILER 		?= arm-poky-linux-gnueabi-
CXX 			:= $(COMPILER)g++
LD 				:= $(COMPILER)ld
OBJCOPY 		:= $(COMPILE)objcopy
OBJDUMP 		:= $(COMPILE)objdump

CXXFLAGS := -march=armv7ve -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=$(POKYSYSROOT) -Wall -O3 -std=c++11
LDFLAGS  := --sysroot=$(POKYSYSROOT) --hash-style=gnu --as-needed

LIBPATH			:=

INCDIRS 		:= . $(MODULEDIR)
SRCDIRS			:= .

INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

CPPFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CPPFILES))

CPPOBJS			:= $(patsubst %, %, $(CFILENDIR:.cpp=.o))
OBJS			:= $(CPPOBJS)

VPATH			:= $(SRCDIRS)

.PHONY: clean

$(TARGET) : $(CPPFILES)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^
	cp $(TARGET) $(OUT_DIR)/test/

clean:
	rm -rf $(TARGET) $(CPPOBJS)


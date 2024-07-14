TARGET ?= binder_client

COMPILER 		?= arm-linux-gnueabihf-
CXX 			:= $(COMPILER)g++
LD 				:= $(COMPILER)ld
OBJCOPY 		:= $(COMPILER)objcopy
OBJDUMP 		:= $(COMPILER)objdump

CXXFLAGS := -march=armv7ve -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=$(POKYSYSROOT) -Wall -O0 -std=c++11
LDFLAGS  := --sysroot=$(POKYSYSROOT) --hash-style=gnu --as-needed

LIBPATH			:=

INCDIRS 		:= . \
					../../../../include/
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

clean:
	rm -rf $(TARGET) $(CPPOBJS)


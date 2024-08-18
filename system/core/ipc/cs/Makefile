TARGET ?= libwraith_cs.so
OUT_DIR ?= $(WRAITH_PATH)/out/system/lib

COMPILER 		?= arm-linux-gnueabihf-
CXX 			:= $(COMPILER)g++
LD 				:= $(COMPILER)ld
OBJCOPY 		:= $(COMPILER)objcopy
OBJDUMP 		:= $(COMPILER)objdump

CXXFLAGS := -march=armv7ve -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=$(POKYSYSROOT) -Wall -g -O2 -std=c++11 -shared -fPIC
LDFLAGS  := --sysroot=$(POKYSYSROOT) --hash-style=gnu --as-needed

LIBPATH			:=

INCDIRS 		:= . \
					$(WRAITH_PATH)/include
SRCDIRS			:= .

INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))
CPPFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))
OBJS			:= $(patsubst %, %, $(CFILENDIR:.cpp=.o))

.PHONY: clean

$(TARGET) : $(CPPFILES)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^
	cp $(TARGET) $(OUT_DIR)/

clean:
	rm -rf $(TARGET) $(OBJS)


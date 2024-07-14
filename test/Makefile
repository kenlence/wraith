MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_PATH := $(dir $(MAKEFILE_PATH))

SUB_DIRS := modules

.PHONY: all clean

all:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir -j;\
	done

clean:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir clean;\
	done
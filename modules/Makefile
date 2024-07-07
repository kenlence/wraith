MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_PATH := $(dir $(MAKEFILE_PATH))

SUB_DIRS := binder

.PHONY: all clean
all:
	for dir in $(SUB_DIRS); do\
		make -C $$dir -j MODULES_DIR=$(CURRENT_PATH);\
	done

clean:
	for dir in $(SUB_DIRS); do\
		make -C $$dir clean;\
	done
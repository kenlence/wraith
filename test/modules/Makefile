CURRENT_PATH := $(shell pwd)
SUB_DIRS := binder_test
OUT_DIR := ../../out/

.PHONY: all clean
all:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir;\
	done

clean:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir clean;\
	done
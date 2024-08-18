CURRENT_PATH := $(shell pwd)
SUB_DIRS := ipc

.PHONY: all clean
all:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir -j;\
	done

clean:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir clean;\
	done

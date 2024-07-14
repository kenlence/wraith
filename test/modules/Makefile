
MODULES_DIR := $(TEST_DIR)/modules
SUB_DIRS := binder

.PHONY: all clean

all:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir -j;\
	done

clean:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir clean;\
	done
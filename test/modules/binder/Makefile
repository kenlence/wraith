BINDER_DIR := $(MODULES_DIR)/binder

SUB_DIRS := service \
			client

.PHONY: all clean

all:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir -j;\
	done

clean:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir clean;\
	done
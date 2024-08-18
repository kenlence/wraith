CURRENT_PATH := $(shell pwd)
SUB_DIRS := core
OUT_DIR := ../out

.PHONY: all clean
all:
	mkdir -p $(OUT_DIR)/system/
	mkdir -p $(OUT_DIR)/system/lib/
	mkdir -p $(OUT_DIR)/system/bin/

	@for dir in $(SUB_DIRS); do\
		make -C $$dir -j -I include;\
	done

	cp -rf rootdir/ $(OUT_DIR)/system/

clean:
	@for dir in $(SUB_DIRS); do\
		make -C $$dir clean;\
	done

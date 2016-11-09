.PHONY: all clean run debug profile

LD = $(CC)

OUT_DIR = build
OBJ_DIR = $(OUT_DIR)/obj

CFLAGS  += -std=c99 -g -Wall -pedantic
LDFLAGS +=
LDLIBS  +=

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

all: common commands shell

clean: clean-common clean-commands clean-shell

run: run-shell

debug: debug-shell

profile: profile-shell

restore:
	/bin/cp -f disks/original/* disks/

include common/*.mk
include cmd/*.mk
include shell/*.mk

.PHONY: shell clean-shell run-shell debug-shell profile-shell

DISKS_DIR = disks/
SHELL_EXE = shell
SHELL_OUT = $(OUT_DIR)/$(SHELL_EXE)
SHELL_SRC = $(wildcard shell/*.c)
SHELL_OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SHELL_SRC)))

$(OBJ_DIR)/shell/%.o: shell/%.c shell/%.h
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

shell: CFLAGS  += $(COMMON_CFLAGS)
shell: LDFLAGS += $(COMMON_LDFLAGS)
shell: LDLIBS  += $(COMMON_LDLIBS) -l readline
shell: $(SHELL_OUT)

clean-shell:
	rm -f $(SHELL_OBJ) $(SHELL_OUT)

run-shell:
	cd build; ./$(SHELL_EXE) ../$(DISKS_DIR)

debug-shell:
	cd build; gdb --args ./$(SHELL_EXE) ../$(DISKS_DIR)

profile-shell:
	cd build; valgrind ./$(SHELL_EXE) ../$(DISKS_DIR)

$(SHELL_OUT): commands $(COMMON_OUT) $(SHELL_OBJ)
	$(LD) $(LDFLAGS) $(COMMON_LDFLAGS) $(SHELL_OBJ) -o $@ $(LDLIBS) $(COMMON_LDLIBS)

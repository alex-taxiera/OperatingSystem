.PHONY: commands

CMD_DIR = cmd

# $1 = Command Name
define CommandTemplate

$(1)_OUT = $$(OUT_DIR)/$(1)
$(1)_SRC = $$(wildcard $$(CMD_DIR)/$(1)/*.c)
$(1)_OBJ = $$(addprefix $$(OBJ_DIR)/, $$(patsubst %.c, %.o, $$($(1)_SRC)))

$$(shell mkdir -p $(dir $$(OUT_DIR)) >/dev/null)

$(1): $$($(1)_OUT)

clean-$(1):
	rm -f $$($(1)_OBJ) $$($(1)_OUT)

$$($(1)_OUT): CFLAGS  += $$(COMMON_CFLAGS)
$$($(1)_OUT): LDFLAGS += $$(COMMON_LDFLAGS)
$$($(1)_OUT): LDLIBS  += $$(COMMON_LDLIBS)
$$($(1)_OUT): $$(COMMON_OUT) $$($(1)_OBJ)
	$$(LD) $$(LDFLAGS) $$($(1)_OBJ) -o $$@ $$(LDLIBS)

endef

COMMANDS = pbs pfe pwd ls cd

$(foreach cmd,$(COMMANDS),$(eval $(call CommandTemplate,$(cmd))))

commands: $(COMMANDS)

clean-commands: $(addprefix clean-,$(COMMANDS))

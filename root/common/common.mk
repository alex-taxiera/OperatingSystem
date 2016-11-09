.PHONY: common clean-common

COMMON_OUT = $(OUT_DIR)/libshell.a
COMMON_SRC = $(wildcard common/*.c)
COMMON_OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(COMMON_SRC)))

COMMON_CFLAGS = -I common/
COMMON_LDFLAGS = -L build/
COMMON_LDLIBS = -l shell

common: $(COMMON_OUT)

clean-common:
	rm -f $(COMMON_OBJ) $(COMMON_OUT)

$(COMMON_OUT): $(COMMON_OBJ)
	$(AR) rvs $(COMMON_OUT) $(COMMON_OBJ)

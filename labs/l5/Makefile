CC = clang
CFLAGS = -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla
SRC = list.c sort.c ui.c
CMD_SRC = cmd_main.c
OUT_CMD = ex2_cmd
OUT_CMD_MC = ex2_cmd_memory_check
MENU_SRC = menu_main.c
OUT_MENU = ex2_menu
OUT_MENU_MC = ex2_menu_memory_check
MC_FLAGS = -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=integer
.PHONY: clean

all: $(OUT_CMD) $(OUT_CMD_MC) $(OUT_MENU) $(OUT_MENU_MC)
	@echo all ex2 successfully constructed

$(OUT_CMD): $(SRC) $(CMD_SRC)
	$(CC) $(CFLAGS) -o $(OUT_CMD) $(SRC) $(CMD_SRC)

$(OUT_CMD_MC) : $(SRC) $(CMD_SRC)
	$(CC) $(CFLAGS) $(MC_FLAGS) -o $(OUT_CMD_MC) $(SRC) $(CMD_SRC)

$(OUT_MENU): $(SRC) $(MENU_SRC)
	$(CC) $(CFLAGS) -o $(OUT_MENU) $(SRC) $(MENU_SRC)

$(OUT_MENU_MC) : $(SRC) $(MENU_SRC)
	$(CC) $(CFLAGS) $(MC_FLAGS) -o $(OUT_MENU_MC) $(SRC) $(MENU_SRC)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.a *~ $(OUT_CMD) $(OUT_CMD_MC) $(OUT_MENU) $(OUT_MENU_MC)
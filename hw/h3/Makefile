CC = clang
CFLAGS = -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion
SRC = ex3.c
OUT = ex3
OUT_MC = ex3_memory_check
MC = ex3_memory_check
MC_FLAGS = -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=integer
.PHONY: clean

all: $(OUT) $(OUT_MC)
	@echo ex3 successfully constructed

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

$(OUT_MC) : $(SRC)
	$(CC) $(CFLAGS) $(MC_FLAGS) -o $(OUT_MC) $(SRC)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.a *~ $(OUT) $(OUT_MC)



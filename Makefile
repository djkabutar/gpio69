OUT_DIR = bin

SRC_PATH = lib
SOURCES = $(wildcard $(SRC_PATH)/*.c)
OBJS = $(patsubst $(SRC_PATH)/%.c, $(SRC_PATH)/%.o, $(SOURCES))

EXP_PATH = examples
EXP_SOURCES = $(wildcard $(EXP_PATH)/*.c)
EXP_BINS = $(patsubst $(EXP_PATH)/%.c, $(EXP_PATH)/%, $(EXP_SOURCES))

CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -L$(OUT_DIR) -lgpio69

.PHONY: all install clean

all: bin lib examples

bin:
	if [ ! -d "$(OUT_DIR)" ]; then mkdir $(OUT_DIR); fi

lib: $(OBJS)
	$(CC) -shared -fPIC $^ -o $(OUT_DIR)/libgpio69.so

examples: $(EXP_BINS)

$(SRC_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(EXP_PATH)/%: $(EXP_PATH)/%.c lib
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

install:
	cp $(OUT_DIR)/libgpio69.so /usr/lib
	cp include/gpio69.h /usr/include

uninstall:
	-rm /usr/lib/libgpio69.so include/gpio69.h

clean:
	-rm $(OUT_DIR)/libgpio69.so lib/*.o $(EXP_BINS)

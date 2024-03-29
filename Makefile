CXXFLAGS += -std=c++17 -Wall -Wextra
LDLIBS += -lSDL2 -lSDL2_image

objects = main.o piece.o pos.o kind.o color.o board.o \
	valid_next_positions.o can_take_place_of.o gui.o \
	assets.o load_texture.o is_checked.o is_check_mated.o \
	valid_next_boards.o choice.o best_next_board.o \
	score.o current_millis.o

assets = $(wildcard ./assets/*.png)

all: lushin

lushin: $(objects)
	$(CXX) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

assets.o: $(assets) assets.hh
	ld -r -b binary -o $@ $(assets)

clean:
	rm -f lushin $(objects)

.PHONY: all clean

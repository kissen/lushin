CXXFLAGS += -std=c++17 -Wall -Wextra -g
LDFLAGS += -lSDL2 -lSDL2_image

objects = main.o piece.o pos.o kind.o color.o board.o \
	valid_next_positions.o can_take_place_of.o gui.o \
	assets.o load_texture.o

assets = $(wildcard *.png)

all: lushin

lushin: $(objects)
	$(CXX) $(LDFLAGS) -o $@ $(objects)

assets.o: $(assets) assets.hh
	ld -r -b binary -o $@ $(assets)

clean:
	rm -f lushin $(objects)

.PHONY: all clean

CXXFLAGS += -std=c++17 -Wall -Wextra
LDFLAGS += -lSDL2 -lSDL2_image

objects = main.o piece.o pos.o kind.o color.o board.o \
	valid_next_positions.o can_take_place_of.o gui.o

all: lushin

lushin: $(objects)
	$(CXX) $(LDFLAGS) -o $@ $(objects)

clean:
	rm -f lushin $(objects)

.PHONY: all clean

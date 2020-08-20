CXXFLAGS += -std=c++17 -Wall -Wextra
LDFLAGS += -lSDL2 -lSDL2_image

objects = main.o

all: lushin

lushin: $(objects)
	$(CXX) $(LDFLAGS) -o $@ $(objects)

clean:
	rm -f lushin $(objects)

.PHONY: all clean

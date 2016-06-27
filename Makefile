CXX=g++-6
LD=g++-6
CXXFLAGS= -std=c++11 -Wall -pedantic -Wextra -g -Wno-long-long -O3
PROGRAM=go
LIBS=-lncurses -lpthread -lrt

all: $(PROGRAM) doc

run: compile
	./$(PROGRAM)

compile: $(PROGRAM)

$(PROGRAM): src/game.o src/main.o src/menu.o src/player.o src/table.o src/draw.o src/functions.o src/connect.o src/msgbox.o
	$(LD) -o $@ $^ $(LIBS)
	mkdir saves	

doc: src/connect.hpp src/constants.hpp src/draw.hpp src/functions.hpp src/game.hpp src/menu.hpp src/msgbox.hpp src/player.hpp src/structures.hpp src/table.hpp
	doxygen Doxyfile
run:

clean:
	rm -f $(PROGRAM)
	rm -f src/*.o
	rm -dfr saves
	rm -dfr doc

%o: %cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


src/connect.o: src/connect.cpp src/connect.hpp src/game.hpp src/constants.hpp src/structures.hpp \
 src/table.hpp src/functions.hpp src/player.hpp
src/draw.o: src/draw.cpp src/draw.hpp
src/functions.o: src/functions.cpp src/functions.hpp src/constants.hpp
src/game.o: src/game.cpp src/game.hpp src/constants.hpp src/structures.hpp src/table.hpp \
 src/functions.hpp src/player.hpp src/connect.hpp src/msgbox.hpp
src/main.o: src/main.cpp src/constants.hpp src/structures.hpp src/game.hpp src/table.hpp \
 src/functions.hpp src/player.hpp src/connect.hpp src/draw.hpp src/menu.hpp src/msgbox.hpp
src/menu.o: src/menu.cpp src/menu.hpp src/constants.hpp src/game.hpp src/structures.hpp src/table.hpp \
 src/functions.hpp src/player.hpp src/connect.hpp
src/msgbox.o: src/msgbox.cpp src/msgbox.hpp src/constants.hpp
src/player.o: src/player.cpp src/player.hpp src/table.hpp src/constants.hpp src/structures.hpp \
 src/functions.hpp src/game.hpp src/connect.hpp
src/table.o: src/table.cpp src/functions.hpp src/constants.hpp src/table.hpp src/structures.hpp \
 src/game.hpp src/player.hpp src/connect.hpp

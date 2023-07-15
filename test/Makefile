TESTGAME_SOURCES = testgame.cpp ../src/game.cpp ../src/tile.cpp ../src/client.cpp ../src/ievent.cpp ../src/goptions.cpp
TESTGAME_INCLUDES = ../include/*.h

.PHONY: testgame
testgame: $(TESTGAME_SOURCES) $(TESTGAME_INCLUDES)
	g++ -std=c++17 $(TESTGAME_SOURCES) -I ../include -I . -o $@ && ./testgame

.PHONY: dtestgame
dtestgame: $(TESTGAME_SOURCES) $(TESTGAME_INCLUDES)
	g++ -std=c++17 $(TESTGAME_SOURCES) -I ../include -I . -g -o $@ && gdb ./dtestgame

TESTIO_SOURCES = testio.cpp
TESTIO_INCLUDES = ../include/*.h

.PHONY: testio
testio: $(TESTIO_SOURCES) $(TESTIO_INCLUDES)
	g++ -std=c++17 $(TESTIO_SOURCES) -I ../include -o $@ && ./testio
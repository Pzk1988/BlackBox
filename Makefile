CC=g++
CFLAGS=-g -Wall -fprofile-arcs -ftest-coverage -pg
STRIP=strip
LFLAGS=-lgtest -lgcov --coverage -pg -lrt -pthread

OBJS=Main.o Parser.o IParser.o global.o Logger.o IStorage.o FileStorage.o File.o SystemCommands.o TimeCounter.o UdpReceiver.o Buffer.o
TESTS=UnitTests/tests.o UnitTests/ParserTest.o

all: clean build

build: $(OBJS) $(TESTS)
	$(CC) $(LFLAGS) $^ -o BlackBox

$(OBJS): %.o: %.cpp %.h
	$(CC) -c $(CFLAGS) $< -o $@
	
$(TEST): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
		
clean:
	rm -rf *.o *.gcda *.gcno UnitTests/*.o UnitTests/*.gcda UnitTests/*.gcno BlackBox
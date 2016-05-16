ARCH=x86

ifeq ($(ARCH),x86)
	PREFIXPATH=
	CC=$(PREFIXPATH)g++
	LD = 
	STRIP=strip
	CFLAGS= -Wall -g -fprofile-arcs -ftest-coverage #-pg
	LDFLAGS= -lrt -lpthread -L/usr/local/lib -lgtest -lgmock -lgcov --coverage  #-pg
endif
ifeq ($(ARCH),arm_iopac)
	PREFIXPATH=/usr/local/arm-linux
	CC=$(PREFIXPATH)/bin/arm-linux-g++
	LD = $(PREFIXPATH)/bin/arm-linux-ld
	STRIP=$(PREFIXPATH)/bin/arm-linux-strip
	CFLAGS=-DIOPAC -DIOPACSERIAL -I/usr/local/arm-linux/include -I/usr/local/arm-linux/include/RTU
	LDFLAGS=-L$(PREFIXPATH)/lib -lpthread -lmxml -lrt -L$(PREFIXPATH)/lib/RTU -lmoxa_rtu -lrtu_common -ltag -Wl,-rpath,/lib/RTU/ -Wl,--allow-shlib-undefined
endif
ifeq ($(ARCH),arm_uc7112)
	PREFIXPATH=/usr/local/arm-linux-e4200
	#PREFIXPATH=/usr/local/arm-linux-7112
	CC=$(PREFIXPATH)/bin/arm-linux-g++
	STRIP=$(PREFIXPATH)/bin/arm-linux-strip
	CFLAGS=-DE4200 -DUC7112SERIAL -I$(PREFIXPATH)/include -Wall
	LDFLAGS=-L$(PREFIXPATH)/lib -lrt -lpthread -lmxio -static
endif
ifeq ($(ARCH),arm_uc8100)
	PREFIXPATH=/usr/local/arm-linux-gnueabihf-4.7-20130415
	CC=$(PREFIXPATH)/bin/arm-linux-gnueabihf-g++
	STRIP=$(PREFIXPATH)/bin/arm-linux-gnueabihf-strip
	CFLAGS=-DMODBUS -DUC8100SERIAL -I$(PREFIXPATH)/include -Wall
	LDFLAGS=-L$(PREFIXPATH)/lib -lrt -lpthread -static	
endif

OBJS=Main.o Parser.o IParser.o global.o Logger.o IStorage.o FileStorage.o File.o SystemCommands.o TimeCounter.o UdpReceiver.o Buffer.o FileHandler.o
TESTS=UnitTests/tests.o UnitTests/SystemCommandsMock.o

all: clean build

build: $(OBJS) $(TESTS)
	$(CC) $^ -o blackbox $(LDFLAGS)

$(OBJS): %.o: %.cpp %.h
	$(CC) -c $(CFLAGS) $< -o $@
	
$(TEST): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
		
clean:
	rm -rf *.o *.gcda *.gcno UnitTests/*.o UnitTests/*.gcda UnitTests/*.gcno Blackbox
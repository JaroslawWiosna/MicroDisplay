CXX = g++

CXXFLAGS = -std=c++11
CXXFLAGS += -c
CXXFLAGS += -Wall

CFLAGS = $(CXXFLAGS) $(DBGFLAG)

BFLAGS = -lboost_system -lboost_thread

LFLAGS = $(BFLAGS)

TARGET = serial

all: serial

serial: main.o serial.o
	$(CXX) main.o serial.o -o $(TARGET)  $(LFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

serial.o: serial.cpp serial.h
	$(CXX) $(CXXFLAGS) serial.cpp -o serial.o $(LFLAGS)

clean:
	rm -rf *.o
	rm -rf $(TARGET)

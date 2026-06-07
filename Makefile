CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

TARGET = montecarlo

SRC = main.cpp \
      engine/simulator.cpp \
      engine/parallel_simulator.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

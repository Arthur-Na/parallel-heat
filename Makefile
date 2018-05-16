CXX=g++
CXXFLAGS= -Wall -Werror -Wextra -std=c++17 -pedantic

OBJS=src/main.o src/heat_simulator.o
TARGET=heat_diffusion

.PHONY: clean

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

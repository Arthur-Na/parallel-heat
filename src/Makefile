CXX=g++
CXXFLAGS= -Wall -Werror -Wextra -std=c++17 -pedantic

OBJS=main.o heat_simulator.o
TARGET=heat_diffusion

.PHONY: clean

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

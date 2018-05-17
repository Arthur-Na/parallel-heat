CXX=g++
CXXFLAGS= -Wall -Werror -Wextra -std=c++17 -pedantic
CPPFLAGS=-fopenmp

OBJS=src/main.o src/heat_simulator.o
TARGET=heat_diffusion

.PHONY: clean

$(TARGET): $(OBJS)
	${LINK.cc} $^ -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

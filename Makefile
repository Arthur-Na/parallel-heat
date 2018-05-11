all:
	g++ -std=c++17 src/*.cc -o heat-diffusion

clean:
	rm src/*.o heat-diffusion

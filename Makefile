CXX = g++

all: plot.pdf

%.x: %.cpp
	$(CXX)  $< -o $@

graficas:
	gnuplot plot.gp

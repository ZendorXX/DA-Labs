CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic

main: main.o
	$(CXX) $(CXXFLAGS) main.o -o main.out

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

benchmark: benchmark.o
	$(CXX) $(CXXFLAGS) benchmark.o -o benchmark.out

benchmark.o: benchmark.cpp
	$(CXX) $(CXXFLAGS) -c benchmark.cpp

generator: generator.o
	$(CXX) $(CXXFLAGS) generator.o -o generator.out

generator.o: generator.cpp
	$(CXX) $(CXXFLAGS) -c generator.cpp

clean:
	rm -f *.o *.out
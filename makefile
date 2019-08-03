CXX = g++
CXXFLAGS = -Wall -pedantic -std=c++11 -g3 -iquote ../../Libr 
OBJS = generate_chaos.o
HEADERS = bitmap.hpp color.hpp chaos.hpp command_flags.hpp point.hpp

GC: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

generate_chaos.o: generate_chaos.cpp 
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm GC generate_chaos.o

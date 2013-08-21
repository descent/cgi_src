CXX=g++
CXXFLAGS=-g

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

cgi1: cgi1.o
	$(CXX) -g -o $@ $<

parse_content: parse_content.o
	$(CXX) -g -o $@ $<


clean:
	rm -rf *.o cgi1

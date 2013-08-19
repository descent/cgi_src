CXX=g++
cgi1: cgi1.o
	$(CXX) -g -o $@ $<
cgi1.o: cgi1.cpp
	$(CXX) -g -c $<
clean:
	rm -rf *.o cgi1

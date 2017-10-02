CXX=g++
CXXFLAGS=-std=c++1z -O2 -Wall
INC=-IC:/dev/boost_1_65_1


all: dbcparser_test


dbcparser_test: dbcparser.o ../src/dbcfile.h
	$(CXX) $(CXXFLAGS) dbcparser.o dbcparser_test.cpp -o dbcparser_test
	@echo "Build finished"


dbcparser.o: ../src/dbcparser.cpp ../src/dbcparser.h
	$(CXX) -c $(CXXFLAGS) $(INC) ../src/dbcparser.cpp

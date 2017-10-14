CXX=g++
CXXFLAGS=-std=c++17 -O3 -Wall
LIBS=-lstdc++fs
INC=-IC:/dev/boost_1_65_1


all: dbcparser_test signalutil_test


dbcparser_test: dbcparser.o ../src/file/dbcfile.h
	$(CXX) $(CXXFLAGS) dbcparser.o dbcparser_test.cpp $(LIBS) -o dbcparser_test
	@echo "Build finished"

signalutil_test: signalutil.o
	$(CXX) $(CXXFLAGS) signalutil.o signalutil_test.cpp -o signalutil_test
	@echo "Build finished"

dbcparser.o: ../src/file/dbcparser.cpp ../src/file/dbcparser.h
	$(CXX) -c $(CXXFLAGS) $(INC) ../src/file/dbcparser.cpp

signalutil.o: ../src/tincan/signalutil.cpp ../src/tincan/signalutil.h
	$(CXX) -c $(CXXFLAGS) ../src/tincan/signalutil.cpp

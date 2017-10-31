CXX=g++
CXXFLAGS=-std=c++17 -O3 -Wall
LIBS=-lstdc++fs
INC=-IC:/dev/boost_1_65_1


all: dbcparser_test signalutil_test


dbcparser_test: dbcmetaparser.o dbcparser.o ../src/file/dbcfile.h
	$(CXX) $(CXXFLAGS) dbcmetaparser.o dbcparser.o dbcparser_test.cpp $(LIBS) -o dbcparser_test
	@echo "Build finished"

signalutil_test: signalutil.o
	$(CXX) $(CXXFLAGS) signalutil.o signalutil_test.cpp -o signalutil_test
	@echo "Build finished"

dbcparser.o: ../src/file/dbcmetaparser.h ../src/file/dbcparser.cpp ../src/file/dbcparser.h
	$(CXX) -c $(CXXFLAGS) $(INC) ../src/file/dbcparser.cpp

dbcmetaparser.o: ../src/file/dbcmetaparser.cpp ../src/file/dbcmetaparser.h
	$(CXX) -c $(CXXFLAGS) $(INC) ../src/file/dbcmetaparser.cpp

signalutil.o: ../src/tincan/signalutil.cpp ../src/tincan/signalutil.h
	$(CXX) -c $(CXXFLAGS) ../src/tincan/signalutil.cpp

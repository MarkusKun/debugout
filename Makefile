DEBUGTEST_BIN := douttest.bin


all : $(DEBUGTEST_BIN)

run :
	./$(DEBUGTEST_BIN)

douttest.bin : debugout.o douttest.o
	$(CXX) -o $@ $^


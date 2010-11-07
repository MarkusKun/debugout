BIN_DEBUGTEST := douttest.bin

all : all_bins

PATH_TO_DEBUGOUT = ./

# this collects OBJ_DEBUGOUT
include $(PATH_TO_DEBUGOUT)Makefile.inc 

OBJ_DEBUGTEST = ./douttest.o



CLEANFILES = $(OBJ_DEBUGOUT) \
						 $(OBJ_DEBUGTEST) \
						 $(BIN_DEBUGTEST)

all_bins : $(BIN_DEBUGTEST)

run :
	./$(BIN_DEBUGTEST)

clean :
	rm -rf $(CLEANFILES)

$(BIN_DEBUGTEST) : $(OBJ_DEBUGOUT) $(OBJ_DEBUGTEST)
	$(CXX) -o $@ $^


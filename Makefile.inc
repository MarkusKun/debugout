# include this file to use debugoutput

# PATH_TO_DEBUGOUT has to be defined
# include using
# include $(PATH_TO_CMDLINEPARSER)Makefile.inc
#
# afterwards, you should be able to use OBJ_DEBUGOUT
# use OBJ_DEBUGNOUT for an object having all functions empty
# (this disabling debug)

SRC_DEBUGOUT  = $(PATH_TO_DEBUGOUT)debugout.cc
OBJ_DEBUGOUT  = debugout.o
OBJ_DEBUGNOUT = debugnout.o

$(OBJ_DEBUGOUT): $(SRC_DEBUGOUT)
	$(CXX) -c -o $@ $^

$(OBJ_DEBUGNOUT): $(SRC_DEBUGOUT)
	$(CXX) -c -o $@ -D NO_OUTPUT $^


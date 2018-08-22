CC = g++
CFLAGS = -Wall -g -std=c++11 -I/usr/include/python2.7
PROG = main

SRCS = main.cpp ProbCount.cpp NaiveHashTable.cpp TrafficMeasureBase.cpp VirtualBmp.cpp VirtualFM.cpp CountMin.cpp BloomFilter.cpp


ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lpython2.7
endif

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)

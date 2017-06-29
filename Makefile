LIBS = -ljpeg -lm 

IFLAGS = -I. -D JPEG
# -I$(MYINCLUDE)
#-D DEBUG 
CFLAGS = -O6 -Wall
#CFLAGS = -g -Wall -DNDEBUG
CC = g++
O_LIST = $(MYLIB)image.o $(MYLIB)image2.o $(MYLIB)cmdLine.o $(MYLIB)match.o $(MYLIB)energy_ish.o $(MYLIB)graph.o 

all : $(MYBIN)flow

clean : 
	rm -f $(MYBIN)flow $(MYLIB)flow.o $(O_LIST)

$(MYLIB)%.o: ./%.cpp ./%.h
	$(CC) $(CFLAGS) -c -o $@ $(IFLAGS) $< 

$(MYBIN)flow : ./flow.cpp $(O_LIST)
	$(CC) $(CFLAGS) -o $@ $(IFLAGS) flow.cpp $(O_LIST) $(LIBS)


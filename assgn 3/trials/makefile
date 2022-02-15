.PHONY : all1 all2 all3 clean

CC=g++

INCLUDE=./
EXE1=cs20btech11005-RMS
EXE2=cs20btech11005-EDF
EXE3=input_gen

SRCS1=Assgn3-RMScs20btech11005.cpp basic.h
SRCS2=Assgn3-EDFcs20btech11005.cpp basic.h
SRCS3=input_gen.cpp basic.h


TEXT=RMS-Log.txt RMS-stats.txt EDF-Log.txt EDF-stats.txt

all1     : 

	$(CC) $(SRCS1) -o $(EXE1) 

run1		: $(EXE1)
	./$(EXE1)

all2     : 

	$(CC) $(SRCS2) -o $(EXE2) 

run2		: $(EXE2)
	./$(EXE2)


all3     : 

	$(CC) $(SRCS3) -o $(EXE3) 

run3		: $(EXE3)
	./$(EXE3)


clean   :
	rm -f *.o $(EXE1) $(EXE2) $(EXE3) $(TEXT)

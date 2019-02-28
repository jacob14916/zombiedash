CC = g++
LIBS = -lglut -lGL -lGLU
STD = -std=c++17
FLAGS = 

# to simulate g32, uncomment the following

#FLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fsanitize=bounds -Wall -Wextra -Wno-sign-compare -Werror=return-type

# On cs32.seas.ucla.edu or lnxsrv06 or lnxsrv07 or lnxsrv09, uncomment the following two lines
# CC = g32
# STD = 
# PATH = 

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean

PRODUCT = ZombieDash

all: $(PRODUCT)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(STD) $(FLAGS) $< -o $@

$(PRODUCT): $(OBJECTS) 
	$(CC) $(FLAGS) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f *.o
	rm -f $(PRODUCT)

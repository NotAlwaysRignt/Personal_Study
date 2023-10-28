FLAGS = -g -Wall -D_GNU_SOURCE
TARGET = main.out
CC = g++
OBJS = main.o Std.o hook.o
all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(FLAGS) -o $(TARGET) $^  -ldl

%.o: %.cpp
	$(CC) $(FLAGS) -c -o $@ $< -ldl

clean:
	rm -f ./*.o ./*.out

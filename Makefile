CC = mpicc

TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c

.PHONY: clean

clean:
	$(RM) $(TARGET)


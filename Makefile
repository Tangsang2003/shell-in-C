CC = gcc # Define the compiler
CFLAGS = -Wall -Wextra -g # Wall shows all warnings, g adds debugging info

TARGET = shell

SRC = src/main.c
# The 'all' rule is what runs when we just type 'make'
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# A 'clean' rule to delete the executable and start fresh
clean:
	rm -f $(TARGET)
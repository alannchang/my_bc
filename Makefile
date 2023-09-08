CFLAGS = -Wall -Wextra -Werror
TARGET = my_bc
SRC = my_bc.c helpers.c
all : $(TARGET)

$(TARGET) : $(SRC)
	gcc $(CFLAGS) -o $(TARGET) $(SRC) 

clean:
	rm -f $(TARGET)

fclean: clean
	rm -f $(TARGET)

re: fclean all

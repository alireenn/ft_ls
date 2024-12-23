# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC_DIR = src
OBJ_DIR = obj
FT_PRINTF_DIR = ft_printf
FT_PRINTF_LIB = $(FT_PRINTF_DIR)/libftprintf.a
INCLUDES = -I include -I $(FT_PRINTF_DIR)/include

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET = ft_ls

# Rules
all: $(TARGET)

$(TARGET): $(OBJ_FILES) $(FT_PRINTF_LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(FT_PRINTF_LIB) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(FT_PRINTF_LIB):
	$(MAKE) -C $(FT_PRINTF_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(FT_PRINTF_DIR) clean

fclean: clean
	rm -f $(TARGET)
	$(MAKE) -C $(FT_PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
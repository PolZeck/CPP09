# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pol <pol@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/12 11:21:22 by pol               #+#    #+#              #
#    Updated: 2026/02/13 09:09:00 by pol              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := btc
TEST_FILE   := input.txt

CXX         := c++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++98 -Iinc

SRC_DIR     := src
OBJ_DIR     := obj

SRC_FILES   := main.cpp BitcoinExchange.cpp 
SRC         := $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ         := $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))
DEP         := $(OBJ:.o=.d)

# Default rule (calls creation of test file)
all: $(NAME) $(TEST_FILE)

# Linking
$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Create the test file with subject content [cite: 151, 153-162]
$(TEST_FILE):
	@echo "Creating $(TEST_FILE)..."
	@echo "date | value" > $(TEST_FILE)
	@echo "2011-01-03 | 3" >> $(TEST_FILE)
	@echo "2011-01-03 | 2" >> $(TEST_FILE)
	@echo "2011-01-03 | 1" >> $(TEST_FILE)
	@echo "2011-01-03 | 1.2" >> $(TEST_FILE)
	@echo "2011-01-09 | 1" >> $(TEST_FILE)
	@echo "2012-01-11 | -1" >> $(TEST_FILE)
	@echo "2001-42-42" >> $(TEST_FILE)
	@echo "2012-01-11 | 1" >> $(TEST_FILE)
	@echo "2012-01-11 | 2147483648" >> $(TEST_FILE)

# Compilation rule with dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Create folders if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Include dependencies (header tracking)
-include $(DEP)

# Clean object and dep files
clean:
	rm -rf $(OBJ_DIR)

# Clean all (including test file)
fclean: clean
	rm -f $(NAME)
	rm -f $(TEST_FILE)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pol <pol@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/12 11:21:22 by pol               #+#    #+#              #
#    Updated: 2026/02/12 11:30:15 by pol              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := btc

CXX         := c++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++98 -Iinc

SRC_DIR     := src
OBJ_DIR     := obj

SRC_FILES   := main.cpp BitcoinExchange.cpp 
SRC         := $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ         := $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))
DEP         := $(OBJ:.o=.d)

# Default rule
all: $(NAME)

# Linking
$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

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

# Clean all
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
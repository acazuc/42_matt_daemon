# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acazuc <acazuc@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/25 06:50:12 by acazuc            #+#    #+#              #
#    Updated: 2016/10/08 12:30:37 by acazuc           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Matt_daemon

CC = g++ -std=c++11

CFLAGS = -Wall -Wextra -Werror -Ofast

INCLUDES_PATH = -I src/

SRCS_PATH = src/

SRCS_NAME = Main.cpp \
			Tintin_reporter.cpp \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.cpp=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

LIBRARY =

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo " - Making $(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $^ $(LIBRARY)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@echo " - Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES_PATH)

odir:
	@mkdir -p $(OBJS_PATH)

clean:
	@echo " - Cleaning objs"
	@rm -f $(OBJS)

fclean: clean
	@echo " - Cleaning executable"
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re odir

NAME = webserv

SRCS = ./srcs/server/Server.cpp \
	   ./srcs/socket/SimpleSocket.cpp \
	   ./srcs/parsing/HttpRequest/ParsingRequest.cpp \
		./srcs/main.cpp

INCLUDES = ./srcs/server/Server.hpp \
		   ./srcs/socket/SimpleSocket.hpp \
		   ./srcs/parsing/HttpRequest/ParsingRequest.hpp

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

CC = c++

all: $(NAME)

$(NAME): $(SRCS) $(INCLUDES)
	@$(CC) $(FLAGS) $(SRCS) -o $(NAME)

clean:
	@rm -rf $(NAME)

fclean:
	@rm -rf $(NAME)

re: fclean all

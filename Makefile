NAME	= webserv

all:
	c++ -Wall -Wextra -Werror -o $(NAME) main.cpp Server.cpp

fclean:
	rm -f $(NAME)
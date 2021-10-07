NAME	=	webserv

SRCS	=	main.cpp \
			Server.cpp \
			Connections.cpp \
			Tools.cpp \
			Request.cpp \
			Client.cpp

OBJS	=	$(SRCS:.cpp=.o)

CC		=	clang++

FLAGS	=	-Wall -Werror -Wextra -std=c++98

%.o:		%.cpp
			$(CC) $(FLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(FLAGS) -o $(NAME) $(OBJS)

clean:
			rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

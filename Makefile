NAME	=	webserv

SRCS	=	srcs/main.cpp \
			srcs/tools.cpp \
			srcs/parsing.cpp \
			class/Server/Server.cpp \
			class/Connections/Connections.cpp \
			class/Request/Request.cpp \
			class/Client/Client.cpp \
			class/Config/Config.cpp \
			class/Config/Location.cpp \
			class/Response/Response.cpp \

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

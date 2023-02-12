CPPFLAGS = -std=c++98 -Wall -Wextra -Werror  -g -fsanitize=address

NAME = webserv
CC = c++
SRCS = ./source/main.cpp ./source/config_file/ConfigFile.cpp ./source/config_file/CheckValidity.cpp \
		./source/Server/Server.cpp ./source/Server/ServerClass.cpp

INC = ./include/WebServer.hpp ./include/ConfigFile.hpp ./include/classes.hpp

OBJS = $(SRCS:.cpp=.o)


.SILENT :

all : $(NAME)
	
$(NAME): $(OBJS) $(INC)
	@$(CC) $(CPPFLAGS) $(SRCS) -o $(NAME)

%.o : %.cpp  $(INC) 
	$(CC)  $(CPPFLAGS) -o $@ -c $< 

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME) 
	@rm -rf webserv.dSYM
	
re: fclean $(NAME)
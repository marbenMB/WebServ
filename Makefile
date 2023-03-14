CPPFLAGS = -std=c++98 -Wall -Wextra -Werror  -g -fsanitize=address

NAME = webserv
CC = c++

SERVSRC = ./source/Server/Server.cpp ./source/Server/ServerClass.cpp ./source/Server/utils.cpp \
			./source/Server/Check.cpp ./source/Server/unitTests.cpp

SRCS = ./source/main.cpp ./source/config_file/ConfigFile.cpp ./source/config_file/CheckValidity.cpp \
		./source/config_file/ft_error.cpp ./source/config_file/save_data.cpp ./source/config_file/tool.cpp \
		$(SERVSRC)
		

INC = ./include/WebServer.hpp ./include/ConfigFile.hpp ./include/classes.hpp ./include/serverSide.hpp ./include/header.hpp ./include/unitTests.hpp

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

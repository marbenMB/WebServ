CC = c++
NAME = webServ
REQUEST_PATH = ./source/request/
METHODS_PATH = ./source/methods/
CONFIGFILE_PATH = ./source/config_file/
INCLUDES_PATH = ./include/
FLAGS =  -Wall -Wextra -Werror -std=c++98  -fsanitize=address


config_SRCS = $(CONFIGFILE_PATH)ConfigFile.cpp $(CONFIGFILE_PATH)CheckValidity.cpp


method_SRCS =  $(METHODS_PATH)get.cpp  $(METHODS_PATH)method.cpp $(METHODS_PATH)delete.cpp  $(METHODS_PATH)post.cpp  $(METHODS_PATH)error.cpp
request_SRCS =  $(REQUEST_PATH)request.cpp $(REQUEST_PATH)split.cpp $(REQUEST_PATH)trimFront.cpp $(REQUEST_PATH)parse.cpp $(REQUEST_PATH)request_getters.cpp
SRCS =  main.cpp 

method_OBJS	= $(method_SRCS:.cpp=.o)
request_OBJS = $(request_SRCS:.cpp=.o)
config_OBJS = $(config_SRCS:.cpp=.o)
OBJS	= $(SRCS:.cpp=.o)
DEPS =  $(INCLUDES_PATH)request.hpp $(INCLUDES_PATH)method.hpp $(INCLUDES_PATH)WebServer.hpp $(INCLUDES_PATH)ConfigFile.hpp $(INCLUDES_PATH)classes.hpp



%.o:%.cpp $(DEPS)
	${CC} ${FLAGS}  -o $@ -c $<

all: $(NAME)

$(NAME): $(OBJS) $(config_OBJS) $(request_OBJS) $(method_OBJS)
	${CC}  ${FLAGS} $(config_SRCS) $(SRCS)  $(request_SRCS) $(method_SRCS) -o $(NAME) -g
	@make clean -C ./

push:fclean
	@git status
	@read -p "Files To Add:" files; git add "$$files"
	@read -p "Message:" message; git commit -m "$$message"
	@read -p "Branch:" branch; git push origin $$branch

clean:
	@rm -f $(OBJS)  $(request_OBJS) $(method_OBJS) $(config_OBJS)
	@echo "\x1b[36m   +> Clean \033[0m\033[38;5;42m [Done] \033[0m";
	
fclean: clean
	@rm -f $(NAME)
	@echo "\x1b[36m   +> fClean \033[0m\033[38;5;42m [Done] \033[0m";

re: fclean all


.PHONY : all  push clean fclean re

# 

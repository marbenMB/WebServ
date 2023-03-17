CC = c++
NAME = webServ
REQUEST_PATH = ./source/request/
METHODS_PATH = ./source/methods/
CONFIGFILE_PATH = ./source/config_file/
INCLUDES_PATH = ./include/
FLAGS =  -Wall -Wextra -Werror -std=c++98  -fsanitize=address




SERVSRC = ./source/Server/Server.cpp ./source/Server/ServerClass.cpp ./source/Server/utils.cpp ./source/Server/Check.cpp ./source/Server/unitTests.cpp
config_SRCS = $(CONFIGFILE_PATH)ConfigFile.cpp $(CONFIGFILE_PATH)CheckValidity.cpp $(CONFIGFILE_PATH)ft_error.cpp $(CONFIGFILE_PATH)save_data.cpp $(CONFIGFILE_PATH)tool.cpp


method_SRCS =  $(METHODS_PATH)get.cpp  $(METHODS_PATH)method.cpp $(METHODS_PATH)delete.cpp  $(METHODS_PATH)post.cpp  $(METHODS_PATH)error.cpp
request_SRCS =  $(REQUEST_PATH)request.cpp $(REQUEST_PATH)split.cpp $(REQUEST_PATH)trimFront.cpp $(REQUEST_PATH)parse.cpp $(REQUEST_PATH)request_getters.cpp $(REQUEST_PATH)redirection.cpp $(REQUEST_PATH)execute_request.cpp $(REQUEST_PATH)Retrieving_requested_resource.cpp $(REQUEST_PATH)create_response.cpp $(REQUEST_PATH)GETstatusOfexecution.cpp
SRCS =  ./source/main.cpp  ./source/Assets.cpp $(REQUEST_PATH)request_errors.cpp $(REQUEST_PATH)is__subdir.cpp $(REQUEST_PATH)is_cgi.cpp $(REQUEST_PATH)retrievingStatusCodeFile.cpp $(SERVSRC)

method_OBJS	= $(method_SRCS:.cpp=.o)
request_OBJS = $(request_SRCS:.cpp=.o)
config_OBJS = $(config_SRCS:.cpp=.o)
OBJS	= $(SRCS:.cpp=.o)
DEPS =  $(INCLUDES_PATH)request.hpp $(INCLUDES_PATH)method.hpp $(INCLUDES_PATH)WebServer.hpp $(INCLUDES_PATH)ConfigFile.hpp $(INCLUDES_PATH)classes.hpp $(INCLUDES_PATH)serverSide.hpp $(INCLUDES_PATH)header.hpp $(INCLUDES_PATH)unitTests.hpp $(INCLUDES_PATH)Assets.hpp

FILES_OBJ = $(OBJS) $(config_OBJS) $(request_OBJS) $(method_OBJS)
FILES_SRC = $(config_SRCS) $(SRCS)  $(request_SRCS) $(method_SRCS)
%.o:%.cpp $(DEPS)
	${CC} ${FLAGS}  -o $@ -c $<

all: $(NAME)

$(NAME): $(FILES_OBJ) 
	${CC}  ${FLAGS}  $(FILES_SRC) -o $(NAME) -g
	# @make clean -C ./

push:fclean
	@git status
	@read -p "Files To Add:" files; git add "$$files"
	@read -p "Message:" message; git commit -m "$$message"
	@read -p "Branch:" branch; git push origin $$branch

clean:
	@rm -f $(OBJS)  $(request_OBJS) $(method_OBJS) $(config_OBJS)
	@echo "\x1b[36m   +> Clean \033[0m\033[38;5;42m [Done] \033[0m";
	
fclean: clean
	@rm -f $(NAME) var/upload/Default/* var/upload/*
	@echo "\x1b[36m   +> fClean \033[0m\033[38;5;42m [Done] \033[0m";

re: fclean all


.PHONY : all  push clean fclean re

# 

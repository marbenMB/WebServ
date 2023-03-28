CC = c++
NAME = webServ
REQUEST_PATH = ./source/request/
METHODS_PATH = ./source/methods/
CONFIGFILE_PATH = ./source/config_file/
SERVER_PATH = ./source/Server/
INCLUDES_PATH = ./include/
FLAGS =  -Wall -Wextra -Werror -std=c++98  -fsanitize=address

DEBUG = false

SERVSRC = $(SERVER_PATH)Server.cpp $(SERVER_PATH)ServerClass.cpp $(SERVER_PATH)utils.cpp $(SERVER_PATH)Check.cpp $(SERVER_PATH)unitTests.cpp $(SERVER_PATH)acceptProcess.cpp $(SERVER_PATH)socketClass.cpp
config_SRCS = $(CONFIGFILE_PATH)ConfigFile.cpp $(CONFIGFILE_PATH)CheckValidity.cpp $(CONFIGFILE_PATH)ft_error.cpp $(CONFIGFILE_PATH)save_data.cpp $(CONFIGFILE_PATH)tool.cpp


method_SRCS =  $(METHODS_PATH)get.cpp  $(METHODS_PATH)method.cpp $(METHODS_PATH)delete.cpp  $(METHODS_PATH)post.cpp  $(METHODS_PATH)error.cpp
request_SRCS =  $(REQUEST_PATH)request.cpp $(REQUEST_PATH)request_utils.cpp  $(REQUEST_PATH)request_getters.cpp $(REQUEST_PATH)Retrieving_requested_resource.cpp 
SRCS =  ./source/main.cpp  ./source/Assets.cpp  ./source/_Exception.cpp  $(REQUEST_PATH)is_cgi.cpp $(SERVSRC)

method_OBJS	= $(method_SRCS:.cpp=.o)
request_OBJS = $(request_SRCS:.cpp=.o)
config_OBJS = $(config_SRCS:.cpp=.o)
OBJS	= $(SRCS:.cpp=.o)
DEPS =  $(INCLUDES_PATH)request.hpp $(INCLUDES_PATH)method.hpp $(INCLUDES_PATH)WebServer.hpp $(INCLUDES_PATH)ConfigFile.hpp $(INCLUDES_PATH)classes.hpp $(INCLUDES_PATH)serverSide.hpp $(INCLUDES_PATH)unitTests.hpp $(INCLUDES_PATH)Assets.hpp

FILES_OBJ = $(OBJS) $(config_OBJS) $(request_OBJS) $(method_OBJS)

ifeq ($(DEBUG),true)
    FILES_OBJ = $(config_SRCS) $(SRCS)  $(request_SRCS) $(method_SRCS)
endif

%.o:%.cpp $(DEPS)
	${CC} ${FLAGS}  -o $@ -c $<

all: Env $(NAME)

$(NAME): $(FILES_OBJ)
	${CC}  ${FLAGS}  $(FILES_OBJ) -o $(NAME)
	

clean:
	@rm -f $(OBJS)  $(request_OBJS) $(method_OBJS) $(config_OBJS)
	@echo "\x1b[36m   +> Clean \033[0m\033[38;5;42m [Done] \033[0m";
	
fclean: clean
	@rm -rf $(NAME) public/upload webServ.dSYM public/session_list.html source/cgi_files/cgi_pages/file_cgi.html
	@echo "\x1b[36m   +> fClean \033[0m\033[38;5;42m [Done] \033[0m";

re: fclean all

Env : 
	@ mkdir -p public/upload/
	@ mkdir -p source/cgi_files/cgi_pages
	@ zsh conf/scriptEnv.sh

.PHONY : all  push clean fclean re Env
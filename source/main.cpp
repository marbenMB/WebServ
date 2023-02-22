#include "../include/WebServer.hpp"


int main (int ac, char **av)
{
    Data g_Data;
    if (ac == 2)
    {
        g_Data.configuration.parcing_file(av[1], g_Data);
        
    }
    else
        g_Data.error = "error in number of argument";
    if (!g_Data.error.empty())
    {
        std::cout << RED << g_Data.error << END_CLR << std::endl;
        // system("leaks webserv");
        return (1);
    }
    // system("leaks webserv");
    return (0);
}

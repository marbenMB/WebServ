#include <ctime>
#include <string>
#include <iostream>
#include <unistd.h>

std::string getCurrentTime() {
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
  std::string str(buffer);

  return str;
}

double timeDifference(const std::string& time) {
  struct tm tm1 = {0}, tm2 = {0};

  const std::string time_now = getCurrentTime();

  strptime(time_now.c_str(), "%Y-%m-%d %H:%M:%S", &tm1);
  strptime(time.c_str(), "%Y-%m-%d %H:%M:%S", &tm2);

  time_t t1 = mktime(&tm1);
  time_t t2 = mktime(&tm2);

  return difftime(t1, t2);
}



int main (){
    std::string time1 = getCurrentTime();
    sleep(5);
    // std::string time2 = getCurrentTime();
    // std::cout << time1 << " ---- " << time2 << std::endl;
    std::cout << timeDifference(time1) << std::endl;
}
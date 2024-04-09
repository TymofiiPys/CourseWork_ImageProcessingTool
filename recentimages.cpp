#include "recentimages.h"

#include <exception>
#include <forward_list>
#include <fstream>
#include <iterator>

std::forward_list<std::string> getRecentImages() {
    std::ifstream begin(kRecentListFile);
    if (begin.is_open()) {
        auto ret = std::forward_list<std::string>(std::istream_iterator<std::string>(begin),
                                                  std::istream_iterator<std::string>());
        begin.close();
        return ret;
    }
    std::forward_list<std::string> oops;
    oops.push_front("oopsie");
    begin.close();
    return oops;
}

void savePathToFile(std::string &filePath) {
    std::ofstream recent(kRecentListFile, std::ios_base::out | std::ios_base::app);
    recent << filePath << std::endl;
    recent.close();
}

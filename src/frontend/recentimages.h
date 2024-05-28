#ifndef RECENTIMAGES_H
#define RECENTIMAGES_H

#include <forward_list>
#include <string>

const std::string kRecentListFile = "recent.txt";

std::forward_list<std::string> getRecentImages();

void savePathToFile(std::string &filePath);

#endif // RECENTIMAGES_H

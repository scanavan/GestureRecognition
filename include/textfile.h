#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <sstream>
#include <fstream>
#include <vector>
#include<string>
#include"filenames.h"
class txt {
public:
	txt(char * path,std::string output);
private:
	FileNames file;
	std::vector<std::string> percents;
	std::vector<std::string> filenames;

};
#endif
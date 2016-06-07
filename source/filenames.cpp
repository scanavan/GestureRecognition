#include "filenames.h"
#include <iostream>

void FileNames::readDir(const char * path) {

	tinydir_open(&dir, path);

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (!strcmp(file.extension, "bin")) {
			bin.push_back((std::string)path + "/" + (std::string)file.name);
		}
		else if (!strcmp(file.extension, "csv")) {
			leap.push_back((std::string)path + "/" + (std::string)file.name);
		}
		else if (!strcmp(file.extension, "png")) {
			if (strstr(file.name, "depth")) depth.push_back((std::string)path + "/" + (std::string)file.name);
			else if (strstr(file.name, "rgb")) rgb.push_back((std::string)path + "/" + (std::string)file.name);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

}

void FileNames::print() {

	std::cout << "Depth files: " << std::endl;
	for (int i = 0; i < depth.size(); i++) {
		std::cout << "\t" << depth[i] << std::endl;
	}

	std::cout << "RGB files: " << std::endl;
	for (int i = 0; i < rgb.size(); i++) {
		std::cout << "\t" << rgb[i] << std::endl;
	}

	std::cout << "Leap files: " << std::endl;
	for (int i = 0; i < leap.size(); i++) {
		std::cout << "\t" << leap[i] << std::endl;
	}

	std::cout << "Bin files: " << std::endl;
	for (int i = 0; i < bin.size(); i++) {
		std::cout << "\t" << bin[i] << std::endl;
	}

}
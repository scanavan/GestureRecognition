#include "filenames.h"
#include <iostream>

FileNames::FileNames(const char * path)
{
	readDir(path);
}
void FileNames::readDir(const char * path) {
	tinydir_dir tempDir;
	tinydir_open(&tempDir, path);
	int counter = 1;
	while (tempDir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&tempDir, &file);

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
		// goes into the subdirectories recursively
		// the counter deals with an issue within the process of going into the subdirectories with hiddedn folders
		else if (file.is_dir && counter > 2) {
			std::string newPath = (std::string)path + "/" + (std::string)file.name;
			const char *cstr = newPath.c_str();
			readDir(cstr);
		}
		counter++;
		tinydir_next(&tempDir);
	}

	tinydir_close(&tempDir);

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
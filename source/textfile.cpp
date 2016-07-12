#include "textfile.h"
#include <iostream>
#include <iomanip>
txt::txt(char* path,std::string output)
{
	file.readDir(path);
	for (int i = 0; i < file.txt.size(); i++)
	{
		std::ifstream ifs;
		std::string line;
		ifs.open(file.txt[i], std::ifstream::in);
		if (ifs.is_open())
		{
			while (!ifs.eof())
			{
				std::getline(ifs, line);
				if (line.find("Stratified cross-validation") != std::string::npos) {
					break;
				}
			}
			std::getline(ifs, line);
			std::getline(ifs, line);
			std::istringstream ss(line);
			std::string token;
			int counter = 0;
			std::string filename;
			int start;
			int end;
			

			while (std::getline(ss, token, ' ')) {
				if (token!="") 
				{
					if (counter == 4)
					{
						start = file.txt[i].find_last_of('/');
						end = file.txt[i].find_last_of('.');
						filename = file.txt[i].substr(start + 1, end - start-1);
						percents.push_back(token);
						filenames.push_back(filename);
						
					}
					counter++;
				}
				
			}
			
		}
		ifs.close();

	}
	std::ofstream outfile;
	outfile.open(output);

	for (int i = 0; i < percents.size(); i++)
	{
		//outfile << std::setw(20);
		//outfile << std::left << filenames[i];
		//outfile << std::right << percents[i] << "\n";
		outfile << filenames[i] << ',' << percents[i] << "\n";
	}
	outfile.close();
}
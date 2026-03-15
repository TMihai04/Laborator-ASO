#include <iostream>
#include <fstream>
#include <string>

int main() {
	std::string s_InputFilePath;

	std::cout << "Input file path: ";
	getline(std::cin, s_InputFilePath);

	std::ifstream if_InputFile(s_InputFilePath);
	if (!if_InputFile.good()) {
		std::cout << "File does not exist" << std::endl;
		return -1;
	}

	std::string s_Line;
	while (getline(if_InputFile, s_Line)) {
		std::cout << s_Line << std::endl;
	}

	return 0;
}
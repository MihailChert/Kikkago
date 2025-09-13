#include <algorithm>
#include <fstream>
#include <sstream>
#include <string_view>
#include "interpretate-line.hpp"

int getCmdOption(int argc, char* argv[], const std::string_view option);
bool cmdOptionExists(int argc, char* argv[], const std::string_view option);

std::ifstream findProgramArg(int argc, char* argv[]);
void initConstParameters();
std::vector<std::string> parseProgram(std::ifstream& file);
void executeProgram(std::vector<std::string>& program);


int main(int argc, char* argv[]){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		std::cout << "Kikkago - Quine is so easy in this language!" << std::endl;
	}
	if(cmdOptionExists(argc, argv, "-v") || cmdOptionExists(argc, argv, "--version")){
		std::cout << "Version 1.1" << std::endl;
	}

	initConstParameters();
	
	std::ifstream file = findProgramArg(argc, argv);
	vars::sarrs["program"] = parseProgram(file);
	file.close();
	executeProgram(vars::sarrs.at("program"));
	return 0;
}


int getCmdOption(int argc, char* argv[], const std::string_view option){
	std::string_view cmd_option;
	for(int i = 0; i < argc; i++){
		cmd_option = argv[i];
		if(option == cmd_option){
			return i;
		}
	}
	return 0;
}

bool cmdOptionExists(int argc, char* argv[], const std::string_view option){
	return getCmdOption(argc, argv, option) != 0;
}

std::ifstream findProgramArg(int argc, char* argv[]){
	std::ifstream file;
	for(size_t i = 1; i < argc; i++){
		file.open(argv[i]);
		if(!file.fail()){
			break;
		}
	}
	return file;
}

void initConstParameters(){
	vars::vars.at("cycles") = 0;
	flags::cycles = vars::vars.at("cycles");
	vars::vars.at("lineNumber") = 0;
	vars::dvars.at("pi") = 3.14159265358979323846;
	vars::dvars.at("euler") = 2.71828182845904523536;
	vars::sarrs.at("program") = std::vector<std::string>{};
}

std::vector<std::string> parseProgram(std::ifstream& file){
	std::vector<std::string> program;
	std::string line, name;
	bool skip_line, has_end=false;
	size_t line_number = 0;
	while(std::getline(file, line)){
		moderniseProgramLine(line, line_number++, skip_line);
		if(line.find("hajumaru")){
			vars::vars.at("lineNumber") = line_number-1;
		}
		has_end |= (line.find("owari") != -1);
		program.push_back(line);
	}
	if(!has_end){
		throw std::logic_error("The \'owari\' operator was never written down! Your programm will never end.");
	}
	return program;
}

void executeProgram(std::vector<std::string>& program){
	ParseStatus status;
	do{
		try{
			status = interpretline(
				vars::sarrs.at("program").at(vars::vars.at("lineNumber"))
			);
		}catch(const std::invalid_argument& e){
			status = ParseStatus::PARSE_ERROR;
		};
		if(++vars::vars.at("lineNumber") == vars::sarrs.at("program").size()){
			vars::vars.at("lineNumber") = 0;
			vars::vars.at("cycles") = ++flags::cycles;

		}
		if(status != ParseStatus::OK && status != ParseStatus::END){
			parseError(status);
			if(hasContinue()){
				status = ParseStatus::OK;
			}
		}
	}while(status == ParseStatus::OK);
}
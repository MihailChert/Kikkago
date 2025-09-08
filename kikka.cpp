#include <algorithm>
#include <fstream>
#include <sstream>
#include <interpretate-flags.hpp>
#include <interpretate-args.hpp>
#include <interpretate-line.hpp>

char* getCmdOption(char** begin, char** end, const std::string& option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);

std::ifstream findProgramArg(int argc, char* argv[]);
void initConstParameters();
std::vector<std::string> parseProgram(const std::ifstream file);
void executeProgram(std::vector<std::string>& program);


int main(int argc, char* argv[]){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		std::cout << "Kikkago - Quine is so easy in this language!" << std::endl;
	}
	if(cmdOptionExists("-v" || cmdOptionExists("--version"))){
		std::cout << "Version 1.1" << std::endl;
	}

	init_const_parameters();
	
	std::ifstream file = findProgramArg(argc, argv);
	vars::sarrs["program"] = parseProgram(file);
	file.close();
	executeProgram(vars::sarrs["program"]);
	return 0;
}


char* getCmdOption(char** begin, char** end, const std::string& option){
	char** itr = std::find(begin, end, option);
	if(itr != end && ++itr != end){
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option){
	return std::find(begin, end, option) != end;
}

std::ifstream findProgramArg(int argc, char* argv[]){
	std::ifstream file;
	for(size_t i = 1; i < argc; i++){
		file.open(file);
		if(!file.fail()){
			break;
		}
	}
	return file;
}

void initConstParameters(){
	vars::vars["cycles"] = 0;
	flags::cycles = vars::vars["cycles"];
	vars::vars["lineNumber"] = 0;
	vars::dvars["pi"] = 3.14159265358979323846;
	vars::dvars["euler"] = 2.71828182845904523536;
	vars::sarrs["program"] = std::vector();
}

std::vector<std::string> parseProgram(const std::ifstream& file){
	std::vector<std::string> program;
	std::string line, name;
	bool skip_line, has_end=false;
	size_t line_number = 0;
	while(std::getline(file, line)){
		moderniseProgramLine(line, line_number++, skip_line);
		if(line.find("hajumaru")){
			vars::vars["lineNumber"] = line_number-1;
		}
		has_end |= (line.find("owari") != line.end());
		program.append(line);
	}
	if(!has_end){
		throw std::logic_error("The \'owari\' operator was never written down! Your programm will never end.");
	}
	return program;
}

void executeProgram(std::vector<std::string>& program){
	short status;
	do{
		try{
			status = interpretline(vars::sarrs["program"][vars::vars["lineNumber"]]);
		}case(const std::invalid_argument& e){
			status = ParseStatus::PARSE_ERROR;
		};
		if(++vars::vars["lineNumber"] == vars::sarrs["program"].size()){
			vars::vars["lineNumber"] = 0;
			vars::vars["cycles"] = ++flags::cycles;

		}
		if(status != ParseStatus::OK && status != ParseStatus::END){
			parseError(status);
			if(hasContinue()){
				status = ParseStatus::OK;
			}
		}
	}while(status == ParseProgram::OK);
}
#include "interpretate-line.hpp"

const std::vector<std::string> same_keyword{
	"do",
	"break",
	"int",
	"float",
	"double",
	"char",
	"bool",
	"goto"
};

std::string may_keyword_change(const std::string& option){
	if(option == "->") return std::string("shr");
	if(option == "<-") return std::string("shl");
	if(option == "<") return std::string("lt");
	if(option == ">") return std::string("gt");
	if(option == "<=") return std::string("le");
	if(option == ">=") return std::string("ge");
	if(option == "==") return std::string("eq");
	if(option == "!=") return std::string("ne");
	if(option == "" || option == ";") return std::string("empty");
	if(std::find(same_keyword.cbegin(), same_keyword.cend(), option) != same_keyword.end()){
		return option + "_";
	}
	return option;
}

void set_addr(int& a1, int& a2){
	if(a1 != -1){
		flags::addr = a1;
	}else{
		a1 = flags::addr;
	}
	if(a2 == -1){
		a2 = flags::addr;
	}
}

int get_from_tape(int a1){
	return (a1 == -1) ? flags::tape[flags::addr] : flags::tape[a1];
}

void set_to_tape(int a1, int value){
	if(a1 == -1){
		flags::tape[flags::addr] = value;
	}else{
		flags::tape[a1] = value;
	}
}

short manageLowLevel(std::istringstream& iss, const InterpretatorKeyword operation){
	int a1, a2;
	iss >> a1 >> a2;
	switch(operation){
	case InterpretatorKeyword::ugoku:
		set_addr(a1, a2);
		return ParseStatus::OK;
	case InterpretatorKeyword::henkamono:
		set_addr(a1, a2);
		interpretator_math::henkamono(a2, flags::f1, flags::f2, flags::f3, flags::cyc, flags::conf1, flags::conf2, flags::prob, a1);
		return ParseStatus::OK;
	case InterpretatorKeyword::bunkiten:
		if(get_from_tape(a1) == get_from_tape(a2)){
			vars::vars["lineNumber"]++;
		}
		return ParseStatus::OK;

	case InterpretatorKeyword::conf:
		flags::conf1 = get_from_tape(a1);
		flags::conf2 = get_from_tape(a2);
		return ParseStatus::OK;
	case InterpretatorKeyword::kaku:
		flags::printtape(get_from_tape(a1), get_from_tape(a2));
		return ParseStatus::OK;
	case InterpretatorKeyword::cycle:
		flags::cyc = get_from_tape(a1);
		return ParseStatus::OK;
	case InterpretatorKeyword::conf1:
		flags::conf1 = get_from_tape(a1);
		return ParseStatus::OK;
	case InterpretatorKeyword::conf2:
		flags::conf2 = get_from_tape(a1);
		return ParseStatus::OK;
	case InterpretatorKeyword::f1:
		flags::f1 = get_from_tape(a1);
		return ParseStatus::OK;
	case InterpretatorKeyword::f2:
		flags::f2 = get_from_tape(a1);
		return ParseStatus::OK;
	case InterpretatorKeyword::f3:
		flags::f3 = get_from_tape(a1);
		return ParseStatus::OK;
	case InterpretatorKeyword::zero:
		set_to_tape(a1, 0);
		return ParseStatus::OK;
	case InterpretatorKeyword::hitotsu:
		set_to_tape(a1, 1);
		return ParseStatus::OK;
	case InterpretatorKeyword::addr:
		flags::addr = interpretator_math::abs_mod(a1, 0, 257);
		return ParseStatus::OK;
	case InterpretatorKeyword::addrwokaku:
		std::cerr << flags::addr << std::endl;
		return ParseStatus::OK;
	case InterpretatorKeyword::mojiwokaku:
		std::cout << char(flags::addr);
		return ParseStatus::OK;
	case InterpretatorKeyword::shr: //"->"
		flags::addr = interpretator_math::abs_mod(++flags::addr, 0, 257);
		return ParseStatus::OK;
	case InterpretatorKeyword::shl: //"<-"
		flags::addr = interpretator_math::abs_mod(--flags::addr, 0, 257);
		return ParseStatus::OK;
	case InterpretatorKeyword::prob:
		flags::prob = std::abs((a1 == -1? flags::addr:a1)) % 101;
		return ParseStatus::OK;
	case InterpretatorKeyword::kyouki:
		vars::vars["lineNumber"] = std::rand() % vars::vars["lineNumber"];
		return ParseStatus::OK;
	case InterpretatorKeyword::inaddr:
		std::cin >> flags::inaddr;
		flags::inaddr = interpretator_math::abs_mod(flags::inaddr, 0, 257);
		flags::addr = flags::inaddr;
		return ParseStatus::OK;
	case InterpretatorKeyword::loop:
		flags::addr = interpretator_math::abs_mod(flags::cycles, 0, 257);
		return ParseStatus::OK;
	}
	return ParseStatus::UD_OPERATION;
}

short manageBlock(std::istringstream& iss, const InterpretatorKeyword& operation){
	std::string block;
	iss >> block;
	switch(operation){
	case InterpretatorKeyword::do_:
		vars::inblocks[block] = true;
		vars::dos[block] = vars::vars["lineNumber"];
		vars::vars["lineNumber"] = vars::blocks[block];
		return ParseStatus::OK;
	case InterpretatorKeyword::break_:
		if(vars::inblocks[block] == true){
			vars::vars["lineNumber"] = vars::dos[block];
			vars::inblocks[block] = false;
		}
		return ParseStatus::OK;
	case InterpretatorKeyword::block:
		flags::watchblock = true;
		return ParseStatus::OK;
	}
	return ParseStatus::UD_OPERATION;
}

bool strtob(const std::string& str){
	std::vector<const char*>false_arrs{{"False", "false", "0", "0.0"}};
	if(std::find(false_arrs.cbegin(), false_arrs.cend(), str) != false_arrs.end()){
		return false;
	}
	return true;
}

short cast_bool(const std::string& varname, bool notdeclarated, bool& casted){
	if(notdeclarated){
		casted = strtob(varname);
		return ParseStatus::OK;
	}
	if(!notdeclarated && vars::isint(varname)){
		casted = vars::vars[varname];
	}else if(!notdeclarated && vars::isfloat(varname)){
		casted = vars::fvars[varname];
	}else if(!notdeclarated && vars::isdouble(varname)){
		casted = vars::dvars[varname];
	}else{
		return ParseStatus::PARSE_ERROR;
	}
	return ParseStatus::OK;
}

short cast_char(const std::string& varname, bool notdeclarated, char& casted){
	if(notdeclarated){
		casted = varname.c_str()[0];
		return ParseStatus::OK;
	}
	if(vars::ischar(varname)){
		casted = vars::cvars[varname];
		return ParseStatus::OK;
	}
	return ParseStatus::PARSE_ERROR;
}

short cast_string(const std::string& varname, bool notdeclarated, std::string& casted){
	if(notdeclarated){
		casted = varname;
		return ParseStatus::OK;
	}
	if(vars::isint(varname)){
		casted = std::to_string(vars::vars[varname]);
	}else if(vars::isfloat(varname)){
		casted = std::to_string(vars::fvars[varname]);
	}else if(vars::isdouble(varname)){
		casted = std::to_string(vars::dvars[varname]);
	}else if(vars::isstring(varname)){
		casted = vars::strings[varname];
	}else{
		return ParseStatus::PARSE_ERROR;
	}
	return ParseStatus::OK;
}

template<typename T>
short create_array(
	std::map<std::string, std::vector<T> >& arrmap,
	T(*parse)(const std::string&, size_t*, int),
	const std::string& varname,
	const std::string& varname2
){
	try{
		arrmap[varname] = std::vector<T>(parse(varname2, nullptr, 10));
		return ParseStatus::OK;
	}catch(std::invalid_argument const &e){
		if(vars::isnotdeclared(varname2)){
			return ParseStatus::UD_OPERATION;
		}
		if(vars::isint(varname2)){
			arrmap[varname] = std::vector<T>(static_cast<T>(vars::vars[varname2]));
		}else if(vars::isfloat(varname2)){
			arrmap[varname] = std::vector<T>(static_cast<T>(vars::fvars[varname2]));
		}else if(vars::isdouble(varname2)){
			arrmap[varname] = std::vector<T>(static_cast<T>(vars::dvars[varname2]));
		}
		return ParseStatus::OK;
	}
	return ParseStatus::PARSE_ERROR;
}
template<typename T>
short create_array(
	std::map<std::string, std::vector<T> >& arrmap,
	T(*parse)(const std::string&, size_t*),
	const std::string& varname,
	const std::string& varname2
){
	try{
		arrmap[varname] = std::vector<T>(parse(varname2, nullptr));
		return ParseStatus::OK;
	}catch(std::invalid_argument const &e){
		if(vars::isnotdeclared(varname2)){
			return ParseStatus::UD_OPERATION;
		}
		if(vars::isint(varname2)){
			arrmap[varname] = std::vector<T>(static_cast<T>(vars::vars[varname2]));
		}else if(vars::isfloat(varname2)){
			arrmap[varname] = std::vector<T>(static_cast<T>(vars::fvars[varname2]));
		}else if(vars::isdouble(varname2)){
			arrmap[varname] = std::vector<T>(static_cast<T>(vars::dvars[varname2]));
		}
		return ParseStatus::OK;
	}
	return ParseStatus::PARSE_ERROR;
}

template<typename T>
short create_array(
	std::map<std::string, std::vector<T> >& arrmap,
	const std::string& varname,
	std::string& varname2,
	short(*cast)(const std::string&, bool, T&)
){
	T casted;
	short respons = cast(varname2, vars::isnotdeclared(varname2), casted);
	arrmap[varname] = std::vector<T>({casted});
	return respons;
}

short manageCreateHightLevelArray(std::istringstream& iss, const std::string& varname, const std::string& type_str){
	std::string varname2;
	InterpretatorKeyword type = InterpretatorKeyword::_from_string(may_keyword_change(type_str).c_str());
	iss >> varname2;
	switch(type){
	case InterpretatorKeyword::int_:
		return create_array(vars::iarrs, std::stoi, varname, varname2);
	case InterpretatorKeyword::float_:
		return create_array(vars::farrs, std::stof, varname, varname2);
	case InterpretatorKeyword::double_:
		return create_array(vars::darrs, std::stod, varname, varname2);
	case InterpretatorKeyword::bool_:
		return create_array<bool>(vars::barrs, varname, varname2, cast_bool);
	case InterpretatorKeyword::char_:
		return create_array<char>(vars::carrs, varname, varname2, cast_char);
	case InterpretatorKeyword::string:
		return create_array<std::string>(vars::sarrs, varname, varname2, cast_string);
	}
	return ParseStatus::PARSE_ERROR;
}

short manageCreateHightLevelVar(std::istringstream& iss, const InterpretatorKeyword& operation){
	std::string varname, val;
	std::string svar;
	iss >> varname >> val;
	if(!vars::isnotdeclared(varname)){
		return ParseStatus::ALREADY_EXISTS;
	}
	switch(operation){
	case InterpretatorKeyword::int_:
		try{
			vars::vars[varname] = std::stoi(val);
			return ParseStatus::OK;
		}catch(std::invalid_argument const &e){
			return ParseStatus::PARSE_ERROR;
		}
	case InterpretatorKeyword::float_:
		try{
			vars::fvars[varname] = std::stof(val);
			return ParseStatus::OK;
		}catch(std::invalid_argument const& e){
			return ParseStatus::PARSE_ERROR;
		}
	case InterpretatorKeyword::double_:
		try{
			vars::dvars[varname] = std::stod(val);
			return ParseStatus::OK;
		}catch(std::invalid_argument const& e){
			return ParseStatus::PARSE_ERROR;
		}
	case InterpretatorKeyword::char_:
		vars::cvars[varname] = val.c_str()[0];
		return ParseStatus::OK;
	case InterpretatorKeyword::string:
		vars::strings[varname] = val;
		return ParseStatus::OK;
	case InterpretatorKeyword::bool_:{
		bool ansver = strtob(val);
		vars::bools[varname] = ansver;
		return ParseStatus::OK;
	}
	case InterpretatorKeyword::array:
		return manageCreateHightLevelArray(iss, varname, val);
	}
	return ParseStatus::UD_OPERATION;
}

void printArrs(const std::string& varname){
	std::cout << "[ ";
	if (vars::isiarr(varname)) {
		for (const auto& str : vars::iarrs[varname]) {
			std::cout << str << " ";
		}
	} else if (vars::isfarr(varname)) {
		for (const auto& str : vars::farrs[varname]) {
			std::cout << str << " ";
		}
	} else if (vars::isdarr(varname)) {
		for (const auto& str : vars::darrs[varname]) {
			std::cout << str << " ";
		}
	} else if (vars::isbarr(varname)) {
		for (const auto& str : vars::barrs[varname]) {
			std::cout << str << " ";
		}
	} else if (vars::iscarr(varname)) {
		for (const auto& str : vars::carrs[varname]) {
			std::cout << str << " ";
		}
	} else if (vars::issarr(varname)) {
		for (const auto& str : vars::sarrs[varname]) {
			std::cout << str << " ";
		}
	}
	std::cout << "]" << std::endl;
}

void printVar(const std::string& varname){
	if (vars::isint(varname)) {
		std::cout << vars::vars[varname] << std::endl;
	} else if (vars::isfloat(varname)) {
		std::cout << vars::fvars[varname] << std::endl;
	} else if (vars::isdouble(varname)) {
		std::cout << vars::dvars[varname] << std::endl;
	} else if (vars::ischar(varname)) {
		std::cout << vars::cvars[varname] << std::endl;
	} else if (vars::isbool(varname)) {
		std::cout << vars::bools[varname] << std::endl;
	} else if (vars::isstring(varname)) {
		std::cout << vars::strings[varname] << std::endl;
	}
}

template<typename T>
T tryParse(const std::string& varname, bool include_arrs=true){
	if(vars::isarray(varname) && !include_arrs){
		throw std::invalid_argument("Cant interpretate argument like array.");
	}
	return T{};
}

template<>
int tryParse(const std::string& varname, bool include_arrs){
	if(vars::isarray(varname) && !include_arrs){
		throw std::invalid_argument("Cant interpretate argument like array.");
	}
	try{
		return std::stoi(varname);
	}catch(const std::invalid_argument& e){
		if(vars::isnotdeclared(varname)){
			throw e;
		}
	}
	if(vars::isint(varname)){
		return vars::vars[varname];
	}else if(vars::isfloat(varname)){
		return static_cast<int>(vars::fvars[varname]);
	}else if(vars::isdouble(varname)){
		return static_cast<int>(vars::dvars[varname]);
	}
	throw std::invalid_argument("Cant interpretate argument.");
}

template<>
float tryParse(const std::string& varname, bool include_arrs){
	if(vars::isarray(varname) && !include_arrs){
		throw std::invalid_argument("Cant interpretate argument like array.");
	}
	try{
		return std::stoi(varname);
	}catch(const std::invalid_argument& e){
		if(vars::isnotdeclared(varname)){
			throw e;
		}
	}
	if(vars::isint(varname)){
		return static_cast<float>(vars::vars[varname]);
	}else if(vars::isfloat(varname)){
		return static_cast<float>(vars::fvars[varname]);
	}else if(vars::isdouble(varname)){
		return static_cast<float>(vars::dvars[varname]);
	}
	throw std::invalid_argument("Cant interpretate argument.");
}

template<>
double tryParse(const std::string& varname, bool include_arrs){
	if(vars::isarray(varname) && !include_arrs){
		throw std::invalid_argument("Cant interpretate argument like array.");
	}
	try{
		return std::stoi(varname);
	}catch(const std::invalid_argument& e){
		if(vars::isnotdeclared(varname)){
			throw e;
		}
	}
	if(vars::isint(varname)){
		return static_cast<double>(vars::vars[varname]);
	}else if(vars::isfloat(varname)){
		return static_cast<double>(vars::fvars[varname]);
	}else if(vars::isdouble(varname)){
		return static_cast<double>(vars::dvars[varname]);
	}
	throw std::invalid_argument("Cant interpretate argument.");
}

template<>
std::string tryParse(const std::string& varname, bool include_arrs){
	if(vars::isarray(varname) && !include_arrs){
		throw std::invalid_argument("Cant interpretate argument like array.");
	}
	if(vars::isnotdeclared(varname)){
		return varname;
	}
	if(vars::isstring(varname)){
		return vars::strings[varname];
	}
	if(vars::isint(varname)){
		return std::to_string(vars::vars[varname]);
	}
	if(vars::isfloat(varname)){
		return std::to_string(vars::fvars[varname]);
	}
	if(vars::isdouble(varname)){
		return std::to_string(vars::dvars[varname]);
	}
	throw std::invalid_argument("Cant interpretate argument.");
}

short sumArrs(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isiarr(varname)){
		try{
			interpretator_math::sumArray<int>(vars::iarrs[varname], tryParse<int>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isfarr(varname)){
		try{
			interpretator_math::sumArray<float>(vars::farrs[varname], tryParse<float>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isdarr(varname)){
		try{
			interpretator_math::sumArray<double>(vars::darrs[varname], tryParse<double>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}
	return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
}

short sumVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		vars::vars[varname] += tryParse<int>(varname2, false);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] += tryParse<float>(varname2, false);
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] += tryParse<double>(varname2, false);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short subArrs(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isiarr(varname)){
		try{
			interpretator_math::subArray<int>(vars::iarrs[varname], tryParse<int>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isfarr(varname)){
		try{
			interpretator_math::subArray<float>(vars::farrs[varname], tryParse<float>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isdarr(varname)){
		try{
			interpretator_math::subArray<double>(vars::darrs[varname], tryParse<double>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}
	return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
}

short subVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		vars::vars[varname] -= tryParse<int>(varname2, false);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] -= tryParse<float>(varname2, false);
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] -= tryParse<double>(varname2, false);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}


short mulArrs(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isiarr(varname)){
		try{
			interpretator_math::mulArray<int>(vars::iarrs[varname], tryParse<int>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isfarr(varname)){
		try{
			interpretator_math::mulArray<float>(vars::farrs[varname], tryParse<float>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isdarr(varname)){
		try{
			interpretator_math::mulArray<double>(vars::darrs[varname], tryParse<double>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}
	return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
}

short mulVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		vars::vars[varname] *= tryParse<int>(varname2, false);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] *= tryParse<float>(varname2, false);
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] *= tryParse<double>(varname2, false);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}


short divArrs(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isiarr(varname)){
		try{
			interpretator_math::divArray<int>(vars::iarrs[varname], tryParse<int>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isfarr(varname)){
		try{
			interpretator_math::divArray<float>(vars::farrs[varname], tryParse<float>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}else if(vars::isdarr(varname)){
		try{
			interpretator_math::divArray<double>(vars::darrs[varname], tryParse<double>(varname2, false));
		}catch(const std::invalid_argument& e){
			return ParseStatus::PARSE_ERROR;
		}
		return ParseStatus::OK;
	}
	return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
}

short divVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		vars::vars[varname] /= tryParse<int>(varname2, false);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] /= tryParse<float>(varname2, false);
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] /= tryParse<double>(varname2, false);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short powVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		vars::vars[varname] = static_cast<int>(std::pow(vars::vars[varname], tryParse<int>(varname2, false)));
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = static_cast<float>(std::pow(vars::fvars[varname], tryParse<float>(varname2, false)));
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] = static_cast<double>(std::pow(vars::dvars[varname], tryParse<double>(varname2, false)));
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short sqrtVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isdouble(varname)){
		if(vars::dvars[varname] < 0){
			return ParseStatus::MATH_ERROR;
		}
		vars::dvars[varname] = std::sqrt(vars::dvars[varname]);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short cnkVars(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isarray(varname) || vars::isarray(varname2)){
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	if(vars::isint(varname)){
		vars::vars[varname] = interpretator_math::cnk(vars::vars[varname], tryParse<int>(varname2));
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = interpretator_math::cnk(vars::fvars[varname], tryParse<int>(varname2));
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] = interpretator_math::cnk(vars::dvars[varname], tryParse<int>(varname2));
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short sinVars(const std::string& varname){
	if(vars::isarray(varname)){
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	if(vars::isint(varname)){
		vars::vars[varname] = interpretator_math::sinn(vars::vars[varname]);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = interpretator_math::sinn(vars::fvars[varname]);
	}else if(vars::isdouble){
		vars::dvars[varname] = interpretator_math::sinn(vars::dvars[varname]);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short cosVars(const std::string& varname){
	if(vars::isarray(varname)){
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	if(vars::isint(varname)){
		vars::vars[varname] = interpretator_math::cosn(vars::vars[varname]);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = interpretator_math::cosn(vars::fvars[varname]);
	}else if(vars::isdouble){
		vars::dvars[varname] = interpretator_math::cosn(vars::dvars[varname]);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short expVars(const std::string& varname){
	if(vars::isarray(varname)){
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	if(vars::isint(varname)){
		vars::vars[varname] = interpretator_math::expn(vars::vars[varname]);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = interpretator_math::expn(vars::fvars[varname]);
	}else if(vars::isdouble){
		vars::dvars[varname] = interpretator_math::expn(vars::dvars[varname]);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short lnVars(const std::string& varname){
	if(vars::isarray(varname)){
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	if(vars::isint(varname)){
		vars::vars[varname] = interpretator_math::lnn(vars::vars[varname]);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = interpretator_math::lnn(vars::fvars[varname]);
	}else if(vars::isdouble){
		vars::dvars[varname] = interpretator_math::lnn(vars::dvars[varname]);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short assignVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		vars::vars[varname] = tryParse<int>(varname2);
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = tryParse<float>(varname2);
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] = tryParse<double>(varname2);
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short modVar(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		vars::vars[varname] = interpretator_math::mod(vars::vars[varname], tryParse<int>(varname2));
	}else if(vars::isfloat(varname)){
		vars::fvars[varname] = interpretator_math::mod(vars::fvars[varname], tryParse<int>(varname2));
	}else if(vars::isdouble(varname)){
		vars::dvars[varname] = interpretator_math::mod(vars::dvars[varname], tryParse<int>(varname2));
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short modArrs(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	int varvalue2 = tryParse<int>(varname2);
	if(vars::isiarr(varname)){
		interpretator_math::modArray(vars::iarrs[varname], varvalue2);
	}else if(vars::isfarr(varname)){
		interpretator_math::modArray(vars::farrs[varname], varvalue2);
	}else if(vars::isdarr(varname)){
		interpretator_math::modArray(vars::darrs[varname], varvalue2);
	}
	return ParseStatus::OK;
}

short setArrayVal(std::istringstream& iss, const std::string& varname){
	std::string index_str, value_str;
	iss >> index_str >> value_str;
	int index = tryParse<int>(index_str);
	if(vars::isiarr(varname)){
		vars::iarrs[varname].at(index) = tryParse<int>(value_str, false);
	}else if(vars::isfarr(varname)){
		vars::farrs[varname].at(index) = tryParse<float>(value_str, false);
	}else if(vars::isdarr(varname)){
		vars::darrs[varname].at(index) = tryParse<double>(value_str, false);
	}else if(vars::iscarr(varname)){
		vars::carrs[varname].at(index) = tryParse<char>(value_str, false);
	}else if(vars::issarr(varname)){
		vars::sarrs[varname].at(index) = tryParse<std::string>(value_str, false);
	}else{
		return ParseStatus::PARSE_ERROR;
	}
	return ParseStatus::OK;
}

short getArrayVal(std::istringstream& iss, const std::string& varname){
	std::string index_str, varname_str;
	iss >> index_str >> varname_str;
	int index = tryParse<int>(index_str);
	if(vars::isiarr(varname)){
		if(vars::isint(varname_str)){
			vars::vars[varname_str] = vars::iarrs[varname][index];
			return ParseStatus::OK;
		}
		if(vars::isfloat(varname_str)){
			vars::fvars[varname_str] = vars::iarrs[varname][index];
			return ParseStatus::OK;
		}
		if(vars::isdouble(varname_str)){
			vars::dvars[varname_str] = vars::iarrs[varname][index];
			return ParseStatus::OK;
		}
	}else if(vars::isfarr(varname)){
		if(vars::isint(varname_str)){
			vars::vars[varname_str] = vars::farrs[varname][index];
			return ParseStatus::OK;
		}
		if(vars::isfloat(varname_str)){
			vars::fvars[varname_str] = vars::farrs[varname][index];
			return ParseStatus::OK;
		}
		if(vars::isdouble(varname_str)){
			vars::dvars[varname_str] = vars::farrs[varname][index];
			return ParseStatus::OK;
		}
	}else if(vars::isdarr(varname)){
		if(vars::isint(varname_str)){
			vars::vars[varname_str] = vars::darrs[varname][index];
			return ParseStatus::OK;
		}
		if(vars::isfloat(varname_str)){
			vars::fvars[varname_str] = vars::darrs[varname][index];
			return ParseStatus::OK;
		}
		if(vars::isdouble(varname_str)){
			vars::dvars[varname_str] = vars::darrs[varname][index];
			return ParseStatus::OK;
		}
	}else if(vars::iscarr(varname) && vars::ischar(varname_str)){
		vars::cvars[varname_str] = vars::carrs[varname][index];
		return ParseStatus::OK;
	}else if(vars::isbarr(varname) && vars::isbool(varname_str)){
		vars::bools[varname_str] = vars::barrs[varname][index];
		return ParseStatus::OK;
	}else if(vars::isstring(varname_str) && vars::issarr(varname)){
		vars::strings[varname_str] = vars::sarrs[varname][index];
		return ParseStatus::OK;
	}
	return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
}

short popArray(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isnotdeclared(varname2)){
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	if (vars::isiarr(varname)) { // int array
		if (vars::isint(varname2)) {
			vars::vars[varname2] = vars::iarrs[varname].back();
		} else if (vars::isfloat(varname2)) {
			vars::fvars[varname2] = float(vars::iarrs[varname].back());
		} else if (vars::isdouble(varname2)) {
			vars::dvars[varname2] = double(vars::iarrs[varname].back());
		} else return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		vars::iarrs[varname].pop_back();
		return ParseStatus::OK;
	} else if (vars::isfarr(varname)) { // float array
		if (vars::isint(varname2)) {
			vars::vars[varname2] = int(vars::farrs[varname].back());
		} else if (vars::isfloat(varname2)) {
			vars::fvars[varname2] = vars::farrs[varname].back();
		} else if (vars::isdouble(varname2)) {
			vars::dvars[varname2] = double(vars::farrs[varname].back());
		} else return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		vars::farrs[varname].pop_back();
		return ParseStatus::OK;
	} else if (vars::isdarr(varname)) { // double array
		if (vars::isint(varname2)) {
			vars::vars[varname2] = int(vars::darrs[varname].back());
		} else if (vars::isfloat(varname2)) {
			vars::fvars[varname2] = float(vars::darrs[varname].back());
		} else if (vars::isdouble(varname2)) {
			vars::dvars[varname2] = vars::darrs[varname].back();
		} else return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		vars::darrs[varname].pop_back();
		return ParseStatus::OK;
	} else if (vars::iscarr(varname)) { // char array
		if (vars::ischar(varname2)) {
			vars::cvars[varname2] = vars::carrs[varname].back();
		} else return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		vars::carrs[varname].pop_back();
		return ParseStatus::OK;
	} else if (vars::isbarr(varname2)) { // bool array
		if (vars::isbool(varname2)) {
			vars::bools[varname2] = vars::barrs[varname].back();
		} else return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		vars::barrs[varname].pop_back();
		return ParseStatus::OK;
	} else if (vars::issarr(varname)) { // string array
		if (vars::isstring(varname2)) {
			vars::strings[varname2] = vars::sarrs[varname].back();
		} else return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		vars::sarrs[varname].pop_back();
		return ParseStatus::OK;
	} else return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;

}

short appendArray(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isiarr(varname)){
		vars::iarrs[varname].push_back(tryParse<int>(varname2, false));
	}else if(vars::isfarr(varname)){
		vars::farrs[varname].push_back(tryParse<float>(varname2, false));
	}else if(vars::isdarr(varname)){
		vars::darrs[varname].push_back(tryParse<double>(varname2, false));
	}else if(vars::iscarr(varname)){
		vars::carrs[varname].push_back(static_cast<char>(tryParse<int>(varname2, false)));
	}else if(vars::issarr(varname)){
		vars::sarrs[varname].push_back(tryParse<std::string>(varname2, false));
	}else if(vars::isbarr(varname)){
		vars::barrs[varname].push_back(static_cast<bool>(tryParse<int>(varname2, false)));
	}else{
		return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::OK;
}

short lenghtArray(std::istringstream& iss, const std::string& varname){
	std::string varname2;
	iss >> varname2;
	if(vars::isint(varname)){
		int& res = vars::vars[varname];
		if(vars::isiarr(varname2)){
			res = vars::iarrs[varname2].size();
		}else if(vars::isfarr(varname2)){
			res = vars::farrs[varname2].size();
		}else if(vars::isdarr(varname2)){
			res = vars::darrs[varname2].size();
		}else if(vars::isbarr(varname2)){
			res = vars::barrs[varname2].size();
		}else if(vars::iscarr(varname2)){
			res = vars::carrs[varname2].size();
		}else if(vars::issarr(varname2)){
			res = vars::sarrs[varname2].size();
		}else{
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		}
		return ParseStatus::OK;
	}
	return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
}

short manageHightLevelFunc(std::istringstream& iss, InterpretatorKeyword operation){
	std::string varname1, varname2;
	short status;
	iss >> varname1;
	if(vars::isnotdeclared(varname1)){
		return ParseStatus::UD_OPERATION;
	}
	switch(operation){
		case InterpretatorKeyword::print:
			if(vars::isarray(varname1)){
				printArrs(varname1);
			}else{
				printVar(varname1);
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::sum:
			if(vars::isarray(varname1)){
				status = sumArrs(iss, varname1);
			}else{
				status = sumVar(iss, varname1);
			}
			return status;
		case InterpretatorKeyword::sub:
			if(vars::isarray(varname1)){
				status = subArrs(iss, varname1);
			}else{
				status = subVar(iss, varname1);
			}
			return status;
		case InterpretatorKeyword::mult:
			if(vars::isarray(varname1)){
				status = mulArrs(iss, varname1);
			}else{
				status = subVar(iss, varname1);
			}
			return status;
		case InterpretatorKeyword::div:
			if(vars::isarray(varname1)){
				status = divArrs(iss, varname1);
			}else{
				status = divVar(iss, varname1);
			}
			return status;
		case InterpretatorKeyword::pow:
			if(vars::isarray(varname1)){
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return powVar(iss, varname1);
		case InterpretatorKeyword::sqrt:
			if(vars::isarray(varname1)){
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return sqrtVar(iss, varname1);
		case InterpretatorKeyword::lt:
			iss >> varname2;
			if(!(tryParse<double>(varname1, false) < tryParse<double>(varname2, false))){
				vars::vars["lineNumber"]++;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::gt:
			iss >> varname2;
			if(!(tryParse<double>(varname1, false) > tryParse<double>(varname2, false))){
				vars::vars["lineNumber"]++;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::le:
			iss >> varname2;
			if(!(tryParse<double>(varname1, false) <= tryParse<double>(varname2, false))){
				vars::vars["lineNumber"]++;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::ge:
			iss >> varname2;
			if(!(tryParse<double>(varname1, false) >= tryParse<double>(varname2, false))){
				vars::vars["lineNumber"]++;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::eq:
			iss >> varname2;
			if(!(tryParse<double>(varname1, false) == tryParse<double>(varname2, false))){
				vars::vars["lineNumber"]++;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::ne:
			iss >> varname2;
			if(!(tryParse<std::string>(varname1, false) != tryParse<std::string>(varname2, false))){
				vars::vars["lineNumber"]++;
			}
			if(!(tryParse<double>(varname1, false) != tryParse<double>(varname2, false))){
				vars::vars["lineNumber"]++;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::factor:
			if(vars::isint(varname1)){
				vars::vars[varname1] = interpretator_math::factor(tryParse<int>(varname1));
			}else if(vars::isfloat(varname1)){
				vars::fvars[varname1] = interpretator_math::factor(tryParse<int>(varname1));
			}else if(vars::isdouble(varname1)){
				vars::dvars[varname1] = interpretator_math::factor(tryParse<int>(varname1));
			}else{
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::cnk:
			return cnkVars(iss, varname1);
		case InterpretatorKeyword::sin:
			return sinVars(varname1);
		case InterpretatorKeyword::cos:
			return cosVars(varname1);
		case InterpretatorKeyword::exp:
			return expVars(varname1);
		case InterpretatorKeyword::ln:
			return lnVars(varname1);
		case InterpretatorKeyword::equal:
			return assignVar(iss, varname1);
		case InterpretatorKeyword::mod:
			if(vars::isarray(varname1)){
				status = modArrs(iss, varname1);
			}else{
				status = modVar(iss, varname1);
			}
		case InterpretatorKeyword::set:
			if(!vars::isarray(varname1)){
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return setArrayVal(iss, varname1);
		case InterpretatorKeyword::get:
			if(!vars::isarray(varname1)){
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return getArrayVal(iss, varname1);
		case InterpretatorKeyword::pop:
			return popArray(iss, varname1);
		case InterpretatorKeyword::append:
			return appendArray(iss, varname1);
		case InterpretatorKeyword::lenght:
			return lenghtArray(iss, varname1);
		case InterpretatorKeyword::mean:
			iss >> varname2;
			if(vars::isdouble(varname1) && vars::isdarr(varname2)){
				vars::dvars[varname1] = interpretator_math::calculateAverage(varname2);
				return ParseStatus::OK;
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		case InterpretatorKeyword::cumsum:
			iss >> varname2;
			if(vars::isdouble(varname1) && vars::isdarr(varname2)){
				vars::dvars[varname1] = interpretator_math::cumsum(varname2);
				return ParseStatus::OK;
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		case InterpretatorKeyword::dispersion:
			iss >> varname2;
			if(vars::isdouble(varname1) && vars::isdarr(varname2)){
				vars::dvars[varname1] = interpretator_math::calcdisp(varname2);
				return ParseStatus::OK;
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		case InterpretatorKeyword::ceil:
			iss >> varname2;
			if(vars::isint(varname1)){
				if(vars::isfloat(varname2)){
					vars::vars[varname1] = ceil(vars::fvars[varname2]);
				}else if(vars::isdouble(varname2)){
					vars::vars[varname1] = ceil(vars::dvars[varname2]);
				}else{
					return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
				}
			}else{
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::trunc:
			iss >> varname2;
			if(vars::isint(varname1)){
				if(vars::isfloat(varname2)){
					vars::vars[varname1] = trunc(vars::fvars[varname2]);
				}else if(vars::isdouble(varname2)){
					vars::vars[varname1] = trunc(vars::dvars[varname2]);
				}else{
					return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
				}
			}else{
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::floor:
			iss >> varname2;
			if(vars::isint(varname1)){
				if(vars::isfloat(varname2)){
					vars::vars[varname1] = floor(vars::fvars[varname2]);
				}else if(vars::isdouble(varname2)){
					vars::vars[varname1] = floor(vars::dvars[varname2]);
				}else{
					return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
				}
			}else{
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return ParseStatus::OK;
		case InterpretatorKeyword::round:
			iss >> varname2;
			if(vars::isint(varname1)){
				if(vars::isfloat(varname2)){
					vars::vars[varname1] = (vars::fvars[varname2] - floor(vars::fvars[varname2]) < 0.5) ? floor(vars::fvars[varname2]) : ceil(vars::fvars[varname2]);
				}else if(vars::isdouble(varname2)){
					vars::vars[varname1] = (vars::dvars[varname2] - floor(vars::dvars[varname2]) < 0.5) ? floor(vars::dvars[varname2]) : ceil(vars::dvars[varname2]);
				}else{
					return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
				}
			}else{
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			}
			return ParseStatus::OK;
	}

	return ParseStatus::UD_OPERATION;
}

short manageRandomLevelFunc(std::istringstream& iss, InterpretatorKeyword operation){
	std::string varname, varname2, varname3;
	double varvalue2, varvalue3;
	short status;
	iss >> varname >> varname2;
	if(vars::isnotdeclared(varname)){
		return ParseStatus::UD_OPERATION;
	}
	switch(operation){
		case InterpretatorKeyword::uniform:
			iss >> varname3;
			varvalue2 = tryParse<double>(varname2, false);
			varvalue3 = tryParse<double>(varname3, false);
			if(varvalue2 >= varvalue3){
				return ParseStatus::MATH_ERROR;
			}
			if(vars::isdouble(varname)){
				vars::dvars[varname] = interpretator_math::RandomUniform(varvalue2, varvalue3);
			return ParseStatus::OK;
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;

		case InterpretatorKeyword::rand:
			return ParseStatus::OK;

		case InterpretatorKeyword::normal:
			iss >> varname3;
			varvalue3 = tryParse<double>(varname3, false);
			if(varvalue3 <= 0){
				return ParseStatus::MATH_ERROR;
			}
			if(vars::isdouble(varname)){
				vars::dvars[varname] = interpretator_math::RandomNormal(tryParse<double>(varname2, false), varvalue3);
				return ParseStatus::OK;
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		
		case InterpretatorKeyword::bernoulli:
			iss >> varname3;
			varvalue2 = tryParse<double>(varname2, false);
			if(varvalue2 < 0 || 1 < varvalue2){
				return ParseStatus::MATH_ERROR;
			}
			if(vars::isdouble(varname)){
				vars::dvars[varname] = interpretator_math::RandomBernoulli(varvalue2);
				return ParseStatus::OK;
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;

		case InterpretatorKeyword::poisson:
			varvalue2 = tryParse<double>(varname2, false);
			if(varvalue2 <= 0){
				return ParseStatus::MATH_ERROR;
			}
			if(vars::isint(varname)){
				vars::vars[varname] = interpretator_math::RandomPoisson(varvalue2);
				return ParseStatus::OK;
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
		case InterpretatorKeyword::histogram:
			iss >> varname3;
			if(vars::isiarr(varname)){
				if(vars::isdarr(varname2)){
					vars::iarrs[varname] = interpretator_math::createHistogram(varname2, tryParse<int>(varname3));
					return ParseStatus::OK;
				}
			}
			return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
	}
	return ParseStatus::UD_OPERATION;
}

void manageTimedelta(std::istringstream& iss, InterpretatorKeyword operation){
	switch(operation){
		case InterpretatorKeyword::jikannoowari:
			if(flags::jikanwomiru){
				flags::jikanwomiru = false;
				std::cerr << "Microseconds: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - flags::start).count() << std::endl;
			}
			break;
		case InterpretatorKeyword::jikannohajimaru:
			flags::jikanwomiru = true;
			flags::start = std::chrono::high_resolution_clock::now();
			break;
	}
}

short manageReadFileFunc(std::istringstream& iss){
	std::string varname1, varname2;
	iss >> varname1 >> varname2;
	std::string varvalue1;
	std::string varvalue2 = tryParse<std::string>(varname2);
	std::ifstream file(varvalue2);
	if(file.fail() && !vars::issarr(varname1)){
		file.close();
		return ParseStatus::CANT_OPEN_FILE;
	}
	while(getline(file, varvalue1)){
		vars::sarrs[varname1].push_back(varvalue1);
	}
	file.close();
	return ParseStatus::OK;
}

short binaryArrayToInt(std::istringstream& iss){
	std::string varname1, varname2, varname3;
	iss >> varname1 >> varname2 >> varname3;
	int start = tryParse<int>(varname2);
	start = interpretator_math::abs_mod(start, 0, flags::tape.size());
	int end = tryParse<int>(varname3);
	end = interpretator_math::abs_mod(end, 0, flags::tape.size());
	
	vars::vars[varname1] = flags::binaryArrayToInt(start, end);
	return ParseStatus::OK;
}

short manageWriteFileFunc(std::istringstream& iss){
	std::string varname1, varname2;
	iss >> varname1 >> varname2;
	std::string varvalue1 = tryParse<std::string>(varname1);
	std::ofstream file(varvalue1);
	if(file.fail() && !vars::issarr(varname2)){
		file.close();
		return ParseStatus::CANT_OPEN_FILE;
	}
	for(auto line = vars::sarrs[varname2].cbegin(); line != vars::sarrs[varname2].cend(); line++){
		file << *line << '\n';
	}
	file.close();
	return ParseStatus::OK;
}

short interpretline(std::string& progline){
	std::istringstream iss{progline};
	std::string operation_str, label, block;
	short respons;

	iss >> operation_str;
	
	operation_str = may_keyword_change(operation_str);
	InterpretatorKeyword operation = InterpretatorKeyword::_from_string(operation_str.c_str());
	if(!flags::watchblock){
		switch(operation){
			case InterpretatorKeyword::hajimaru:
			case InterpretatorKeyword::label:
				if(vars::isnotdeclared)
			
			case InterpretatorKeyword::do_:
			case InterpretatorKeyword::break_:
			case InterpretatorKeyword::block:
				respons = manageBlock(iss, operation);
				if(ParseStatus::OK != respons) return respons;
				break;
			
			case InterpretatorKeyword::ugoku:
			case InterpretatorKeyword::henkamono: 
			case InterpretatorKeyword::bunkiten:
			case InterpretatorKeyword::conf:
			case InterpretatorKeyword::conf1:
			case InterpretatorKeyword::conf2:
			case InterpretatorKeyword::kaku:
			case InterpretatorKeyword::cycle:
			case InterpretatorKeyword::zero:
			case InterpretatorKeyword::hitotsu:
			case InterpretatorKeyword::f1:
			case InterpretatorKeyword::f2:
			case InterpretatorKeyword::f3:
			case InterpretatorKeyword::addr:
			case InterpretatorKeyword::addrwokaku:
			case InterpretatorKeyword::mojiwokaku:
			case InterpretatorKeyword::shl:
			case InterpretatorKeyword::shr:
			case InterpretatorKeyword::prob:
			case InterpretatorKeyword::inaddr:
			case InterpretatorKeyword::loop:
			case InterpretatorKeyword::kyouki:
				respons = manageLowLevel(iss, operation);
				if(respons != ParseStatus::OK) return respons;
				break;
			case InterpretatorKeyword::owari:
				return ParseStatus::END;
			case InterpretatorKeyword::empty:
				vars::vars["lineNumber"]++;
				break;
			case InterpretatorKeyword::int_:
			case InterpretatorKeyword::float_:
			case InterpretatorKeyword::double_:
			case InterpretatorKeyword::char_:
			case InterpretatorKeyword::bool_:
			case InterpretatorKeyword::string:
			case InterpretatorKeyword::array:{
				respons = manageCreateHightLevelVar(iss, operation);
				if(respons != ParseStatus::OK) return respons;
				break;
			}
			case InterpretatorKeyword::print:
			case InterpretatorKeyword::sum:
			case InterpretatorKeyword::sub:
			case InterpretatorKeyword::mult:
			case InterpretatorKeyword::div:
			case InterpretatorKeyword::pow:
			case InterpretatorKeyword::sqrt:
			case InterpretatorKeyword::lt:
			case InterpretatorKeyword::gt:
			case InterpretatorKeyword::le:
			case InterpretatorKeyword::ge:
			case InterpretatorKeyword::eq:
			case InterpretatorKeyword::ne:
			case InterpretatorKeyword::factor:
			case InterpretatorKeyword::cnk:
			case InterpretatorKeyword::sin:
			case InterpretatorKeyword::cos:
			case InterpretatorKeyword::exp:
			case InterpretatorKeyword::ln:
			case InterpretatorKeyword::equal:
			case InterpretatorKeyword::mod:
			case InterpretatorKeyword::set:
			case InterpretatorKeyword::pop:
			case InterpretatorKeyword::append:
			case InterpretatorKeyword::lenght:
			case InterpretatorKeyword::mean:
				respons = manageHightLevelFunc(iss, operation);
				if(respons != ParseStatus::OK) return respons;
				break;
			case InterpretatorKeyword::uniform:
			case InterpretatorKeyword::rand:
			case InterpretatorKeyword::bernoulli:
			case InterpretatorKeyword::poisson:
			case InterpretatorKeyword::histogram:
				respons = manageRandomLevelFunc(iss, operation);
				if(respons != ParseStatus::OK) return respons;
				break;
			case InterpretatorKeyword::run:
				break;

			case InterpretatorKeyword::read:
				respons = manageReadFileFunc(iss);
				if(respons != ParseStatus::OK) return respons;
				break;
			case InterpretatorKeyword::write:
				respons = manageWriteFileFunc(iss);
				if(respons != ParseStatus::OK) return respons;
				break;

			case InterpretatorKeyword::jikannohajimaru:
			case InterpretatorKeyword::jikannoowari:
				manageTimedelta(iss, operation);
				break;
			case InterpretatorKeyword::goto_:
				iss >> label;
				if(vars::islabel(label)){
					vars::vars["lineNumber"] = vars::labels[label];
					return ParseStatus::OK;
				}else{
					int gtl = tryParse<int>(label);
					if(gtl == -1){
						vars::vars["lineNumber"] = flags::addr - 1;
						return ParseStatus::OK;
					}else{
						vars::vars["lineNumber"] = gtl - 1;
						return ParseStatus::OK;
					}
				}
				return ParseStatus::UNHANDLED_OPERATION_WITH_TYPES;
			case InterpretatorKeyword::tapeint:
				return binaryArrayToInt(iss);
			default:
				return ParseStatus::UD_OPERATION;

		}
	}
	return ParseStatus::END;
}

void moderniseProgramLine(std::string& program_line, int line_number, bool& skip_line){
	if(program_line.empty()){
		return;
	}
	std::string::iterator start_commit = program_line.begin() + program_line.find("\\*");
	if(start_commit != program_line.end()){
		skip_line = true;
	}
	std::string::iterator end_commit = program_line.begin() + program_line.find("*\\");
	if(end_commit != program_line.end()){
		start_commit = start_commit == program_line.end() ? program_line.begin() : start_commit;
		program_line = std::string(
			program_line.begin(),
			start_commit
		)
		+ std::string(
			end_commit,
			program_line.end()
		);
		skip_line = false;
	}
	if(skip_line){
		return;
	}
	std::istringstream sprog_line(program_line);
	std::string operation;
	sprog_line >> operation;
	if(operation == "label"){
		std::string name;
		sprog_line >> name;
		vars::labels[name] = line_number;
		program_line = std::string();
		return;
	}
	if(operation == "block"){
		std::string name;
		sprog_line >> name;
		vars::blocks[name] = line_number;
		program_line = std::string();
		return;
	}
	if(operation == "prec"){
		int val;
		sprog_line >> val;
		std::cout << std::fixed << std::setprecision(val);
		program_line = std::string();
		return;
	}
}

void parseError(ParseStatus status){
	if(status == ParseStatus::OK || ParseStatus::END){
		return;
	}
	switch(status){
		case ParseStatus::UD_OPERATION:
			std::cerr << "Unknown operation in line!" << std::endl;
			break;
		case ParseStatus::UNHANDLED_OPERATION_WITH_TYPES:
			std::cerr << "Unhandled operation with types." << std::endl;
			break;
		case ParseStatus::MATH_ERROR:
			std::cerr << "Division by zero in line." << std::endl;
			break;
		case ParseStatus::PARSE_ERROR:
			std::cerr << "Cant parse command or value on line." << std::endl;
			break;
		case ParseStatus::ALREADY_EXISTS:
			std::cerr << "The variable already exists on line." << std::endl;
			break;
		case ParseStatus::CANT_OPEN_FILE:
			std::cerr << "Cant open file." << std::endl;
			std::cerr << "File is not exists or dont have permissions." << std::endl;
			break;
	}
	std::cout << std::endl;
	std::cout << "The line on which the error occurred:\n" << std::endl;
	std::cout << "Line " << vars::vars["lineNumber"] << ':' << vars::sarrs["program"][vars::vars["lineNumber"]] << std::endl;
}

bool hasContinue(){
	std::string choise;
	do{
		std::cout << "Ignore the error (the line will be skipped, even bigger errors are possible!)? (y/n):";
		std::cin >> choise;
		std::transform(choise.begin(), choise.end(), choise.begin(), (int(*)(int))std::tolower);
		if(choise[0] == 'y'){
			std::cout << "The program will continue!\n" << std::endl;
			try{
				vars::sarrs["program"].at(++vars::vars["lineNumber"]) = std::string();
			}catch(std::out_of_range const &e){
				vars::vars["lineNumber"] = 0;
				vars::vars["cycles"] = ++flags::cycles;
			}
			return true;
		}
		if(choise[0] == 'n'){
			std::cout << "The program stops!" << std::endl;
			return false;
		}
		std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
	}while(true);
}
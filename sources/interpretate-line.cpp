#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <execution>
#include <utility>
#include "interpretator-flags.hpp"
#include "interpretator-args.hpp"
#include "interpretator-math.hpp"
#include "interpretate-line.hpp"

const std::vector<std::string> same_keyword{
	"do",
	"break",
	"int",
	"float",
	"double",
	"char",
	"bool"
};

std::string may_keyword_change(const std::string& option){
	if(option == "->") return std::string("shr");
	if(option == "<-") return std::string("shl");
	if(option == "" || option == ";") return std::string("empty");
	if(std::execution::sequenced_policy(), std::find(same_keyword.cbegin(), same_keyword.cend(), option) != same_keyword.end()){
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
		flags::addr = interpretator_math::abs(a1, 0, 257);
		return ParseStatus::OK;
	case InterpretatorKeyword::addrwokaku:
		std::cerr << flags::addr << std::endl;
		return ParseStatus::OK;
	case InterpretatorKeyword::mojiwokaku:
		std::cout << char(flags::addr);
		return ParseStatus::OK;
	case InterpretatorKeyword::shr: //"->"
		flags::addr = interpretator_math::abs(++flags::addr, 0, 257);
		return ParseStatus::OK;
	case InterpretatorKeyword::shl: //"<-"
		flags::addr = interpretator_math::abs(--flags::addr, 0, 257);
		return ParseStatus::OK;
	case InterpretatorKeyword::prob:
		flags::prob = std::abs((a1 == -1? flags::addr:a1)) % 101;
		return ParseStatus::OK;
	case InterpretatorKeyword::kyouki:
		vars::vars["lineNumber"] = std::rand() % vars::vars["lineNumber"];
		return ParseStatus::OK;
	case InterpretatorKeyword::inaddr:
		std::cin >> flags::inaddr;
		flags::inaddr = interpretator_math::abs(flags::inaddr, 0, 257);
		flags::addr = flags::inaddr;
		return ParseStatus::OK;
	case InterpretatorKeyword::loop:
		flags::addr = interpretator_math::abs(flags::cycles, 0, 257);
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

short interpretline(std::string progline){
	std::istringstream iss{progline};
	std::string operation_str, label, block;
	short respons;

	iss >> operation_str;
	
	operation_str = may_keyword_change(operation_str);
	InterpretatorKeyword operation = InterpretatorKeyword::_from_string(operation_str.c_str());
	if(!flags::watchblock){
		switch(operation){
			case InterpretatorKeyword::hajimaru:
			case InterpretatorKeyword::label: break;
			
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
			default:
				return ParseStatus::UD_OPERATION;

		}
	}
	return ParseStatus::END;
}
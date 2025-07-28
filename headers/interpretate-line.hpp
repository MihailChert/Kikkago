#pragma once


#include <string>
#include "better_enum.hpp"

//int& lineNumber = vars["lineNumber"];  // Номер текущей обрабатываемой строки программы

// Массив с программой (построчно)
//std::vector<std::string>& program = sarrs["program"];
BETTER_ENUM(InterpretatorKeyword, int, hajimaru, label, do_, to, break_, block, ugoku, henkamono, bunkiten, conf, conf1, conf2, kaku, cycle, zero, hitotsu, f1, f2, f3, addr, addrwokaku, mojiwokaku, shl, shr, empty, prob, inaddr, loop, kyouki, owari, int_, float_, double_, char_, bool_, string, array, print, sum, sub, mult, div, pow);

enum ParseStatus{OK, END, UD_OPERATION, UNHANDLED_OPERATION_WITH_TYPES, ALREADY_EXISTS, PARSE_ERROR};
short interpretline(std::string progline);
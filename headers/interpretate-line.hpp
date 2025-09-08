#pragma once


#include <string>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <utility>
#include "interpretator-flags.hpp"
#include "interpretator-args.hpp"
#include "interpretator-math.hpp"
#include "better_enum.hpp"

//int& lineNumber = vars["lineNumber"];  // Номер текущей обрабатываемой строки программы

// Массив с программой (построчно)
//std::vector<std::string>& program = sarrs["program"];
BETTER_ENUM(InterpretatorKeyword, int, hajimaru, label, do_, to, break_, block, ugoku, henkamono, bunkiten, conf, conf1, conf2, kaku, cycle, zero, hitotsu, f1, f2, f3, addr, addrwokaku, mojiwokaku, shl, shr, empty, prob, inaddr, loop, kyouki, owari, int_, float_, double_, char_, bool_, string, array, print, sum, sub, mult, div, pow, sqrt, lt, gt, le, ge, eq, ne, factor, cnk, sin, cos, exp, ln, equal, run, mod, set, get, uniform, rand, normal, bernoulli, poisson, pop, append, write, read, lenght, jikannohajimaru, jikannoowari, mean, cumsum, dispersion, ceil, floor, round, trunc, histogram, goto_, tapeint, nsm);

enum ParseStatus{OK, END, UD_OPERATION, UNHANDLED_OPERATION_WITH_TYPES, ALREADY_EXISTS, PARSE_ERROR, MATH_ERROR, CANT_OPEN_FILE};
short interpretline(std::string& progline);
void parseError(ParseStatus status);
void moderniseProgramLine(std::string& program_lilne);
bool hasContinue();
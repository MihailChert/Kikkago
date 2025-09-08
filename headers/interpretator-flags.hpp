#pragma once


#include <array>
#include <chrono>
#include <stdexcept>
#include <iostream>

namespace flags{

extern std::array<int, 257> tape;

extern int f1;
extern int f2;
extern int f3;
extern int cyc;
extern int conf1;
extern int conf2;
extern int prob;
extern int output;
extern int addr;
extern int inaddr;
extern int cycles; // Сколько прошло циклов программы
extern bool watchblock;
extern bool jikanwomiru;
extern std::chrono::time_point<std::chrono::high_resolution_clock> start;

// Функция для преобразования ленты в число
int binaryArrayToInt(std::size_t start, std::size_t end);
void printtape(short a1, short a2);
}
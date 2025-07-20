#pragma once
#include "interpretator-flags.hpp"

namespace flags{

	std::array<int, 257> tape = {};

	int f1 = 0;
	int f2 = 0;
	int f3 = 0;
	int cyc = 0;
	int conf1 = 0;
	int conf2 = 0;
	int prob = 100;
	int output = 0;
	int addr = 0;
	int inaddr = 0;
	int cycles = 0; // Сколько прошло циклов программы
	bool watchblock = false;
	bool jikanwomiru = false;
	std::chrono::time_point start;

	// Функция для преобразования ленты в число
	int binaryArrayToInt(std::size_t start, std::size_t end) {
	    // Проверка на корректность диапазона
	    if (start >= end || end > tape.size()) {
	        throw std::out_of_range("Invalid range");
	    }

	    int result = 0;
	    
	    // Преобразование двоичной записи в целое число
	    for (std::size_t i = start; i < end; ++i) {
	        // Сдвигаем результат влево на 1 (умножаем на 2)
	        result <<= 1;
	        // Добавляем текущий бит (0 или 1)
	        result |= tape[i];
	    }

	    return result;
	}
	void printtape(int a1, int a2){
		for (int i = a1; i <= a2; i++) {

			std::cerr << tape[i]; << " ";
		}
	    std::cerr << std::endl;
}
}
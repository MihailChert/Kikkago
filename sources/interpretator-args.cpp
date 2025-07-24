#include "interpretator-args.hpp"

namespace args{
	// Метки, номера строк, блоки
	std::map<std::string, int> labels; // Хранилище меток
	std::map<std::string, int> blocks; // Хранилище блоков
	std::map<std::string, int> dos; // Хранилище строк исполнения
	std::map<std::string, bool> inblocks; // Хранилище переменных "находимся ли мы в блоке"
	// Типы данных одного значения
	std::map<std::string, int> vars; // Хранилище переменных типа int 
	std::map<std::string, double> dvars; // Хранилище переменных типа double
	std::map<std::string, char> cvars; // Хранилище переменных типа char
	std::map<std::string, float> fvars; // Хранилище переменных типа float
	std::map<std::string, int> alephs; // Хранилище переменных типа aleph
	std::map<std::string, bool> bools; // Хранилище переменных типа bool
	std::map<std::string, std::string> strings; // Хранилище переменных типа strings
	// Массивы целых, не целых и строк
	std::map<std::string, std::vector<int>> iarrs; // Хранилище массивов целых чисел
	std::map<std::string, std::vector<float>> farrs; // Хранилище массивов float чисел
	std::map<std::string, std::vector<double>> darrs; // Хранилище массивов double чисел
	std::map<std::string, std::vector<bool>> barrs; // Хранилище массивов bool
	std::map<std::string, std::vector<char>> carrs; // Хранилище массивов char
	std::map<std::string, std::vector<std::string>> sarrs; // Хранилище массивов string

	bool isint(const std::string& key) {return vars.find(key) != vars.end();}
	bool isfloat(const std::string& key) {return fvars.find(key) != fvars.end();}
	bool isdouble(const std::string& key) {return dvars.find(key) != dvars.end();}
	bool isbool(const std::string& key) {return bools.find(key) != bools.end();}
	bool ischar(const std::string& key) {return cvars.find(key) != cvars.end();}
	bool isstring(const std::string& key) {return strings.find(key) != strings.end();}

	bool isiarr(const std::string& key) {return iarrs.find(key) != iarrs.end();}
	bool isfarr(const std::string& key) {return farrs.find(key) != farrs.end();}
	bool isdarr(const std::string& key) {return darrs.find(key) != darrs.end();}
	bool isbarr(const std::string& key) {return barrs.find(key) != barrs.end();}
	bool iscarr(const std::string& key) {return carrs.find(key) != carrs.end();}
	bool issarr(const std::string& key) {return sarrs.find(key) != sarrs.end();}
	bool islabel(const std::string& key) {return labels.find(key) != labels.end();}
	bool isblock(const std::string& key) {return blocks.find(key) != blocks.end();}

	bool isnotdeclared(const std::string& key) {
		if ((!isint(key)) && 
		(!isfloat(key)) && 
		(!isdouble(key)) && 
		(!isbool(key)) && 
		(!ischar(key)) && 
		(!isstring(key)) && 
		(!isiarr(key)) && 
		(!isfarr(key)) && 
		(!isdarr(key)) && 
		(!isbarr(key)) && 
		(!iscarr(key)) && 
		(!issarr(key)) &&
		(!islabel(key)) &&
		(!isblock(key))) {
			return true;
		} else {return false;}
	}

	bool isarray(const std::string& key) {
		if ((!isiarr(key)) || 
		(!isfarr(key)) || 
		(!isdarr(key)) || 
		(!isbarr(key)) || 
		(!iscarr(key)) || 
		(!issarr(key))) {
			return true;
		} else {return false;}
	}

}
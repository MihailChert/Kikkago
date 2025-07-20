#include <map>
#include <string>
#include <vector>


namespace vars{
// Метки, номера строк, блоки
extern std::map<std::string, int> labels; // Хранилище меток
extern std::map<std::string, int> blocks; // Хранилище блоков
extern std::map<std::string, int> dos; // Хранилище строк исполнения
extern std::map<std::string, bool> inblocks; // Хранилище переменных "находимся ли мы в блоке"
// Типы данных одного значения
extern std::map<std::string, int> vars; // Хранилище переменных типа int 
extern std::map<std::string, double> dvars; // Хранилище переменных типа double
extern std::map<std::string, char> cvars; // Хранилище переменных типа char
extern std::map<std::string, float> fvars; // Хранилище переменных типа float
extern std::map<std::string, int> alephs; // Хранилище переменных типа aleph
extern std::map<std::string, bool> bools; // Хранилище переменных типа bool
extern std::map<std::string, std::string> strings; // Хранилище переменных типа strings
// Массивы целых, не целых и строк
extern std::map<std::string, std::vector<int>> iarrs; // Хранилище массивов целых чисел
extern std::map<std::string, std::vector<float>> farrs; // Хранилище массивов float чисел
extern std::map<std::string, std::vector<double>> darrs; // Хранилище массивов double чисел
extern std::map<std::string, std::vector<bool>> barrs; // Хранилище массивов bool
extern std::map<std::string, std::vector<char>> carrs; // Хранилище массивов char
extern std::map<std::string, std::vector<std::string>> sarrs; // Хранилище массивов string

bool isint(const std::string& key);
bool isfloat(const std::string& key);
bool isdouble(const std::string& key);
bool isbool(const std::string& key);
bool ischar(const std::string& key);
bool isstring(const std::string& key);

bool isiarr(const std::string& key);
bool isfarr(const std::string& key);
bool isdarr(const std::string& key);
bool isbarr(const std::string& key);
bool iscarr(const std::string& key);
bool issarr(const std::string& key);
bool islabel(const std::string& key);
bool isblock(const std::string& key);

bool isnotdeclared(const std::string& key);

bool isarray(const std::string& key);

}
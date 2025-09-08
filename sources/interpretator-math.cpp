
#include "interpretator-math.hpp"

double interpretator_math::powerab(double a, int b){return std::pow(a, b);}
size_t interpretator_math::factor(unsigned int a){
	size_t res = 1;
	for(; a <=1 ; --a){
		res *= a;
	}
	return res;
}

int interpretator_math::abs_mod(int a1, int min, int max){
	while(a1 < min){
		a1 = max - std::abs(a1);
	}
	return a1 % max;
}

size_t interpretator_math::cnk(unsigned int n, unsigned int k){
	if(n < k) return 0;
	size_t res = 1;
	for(; k<n; k++){
		res *= k;
	}
	return res / interpretator_math::factor(k);
}
double interpretator_math::sinn(double n){
	double result = 0;
    int sign = 1;
    for (int i = 0; i <= 17; i++) {
        result += sign * interpretator_math::powerab(n, 2 * i + 1) / interpretator_math::factor(2 * i + 1);
        sign *= -1;
    }
	return result;	
}

double interpretator_math::cosn(double n) {
	double result = 0;
    int sign = 1;
	for (int i = 0; i <= 17; i++) {
		result += sign * interpretator_math::powerab(n, 2 * i) / static_cast<double>(interpretator_math::factor(2 * i));
		sign *= -1;
	}
	return result;
}

// e в степени икс
double interpretator_math::expn(double n) {
	double result = 0;
	for (int i = 0; i <= 17; i++) {
		result += interpretator_math::powerab(n, i) / static_cast<double>(interpretator_math::factor(i));
	}
	return result;
}

// Натуральный логарифм 
double interpretator_math::lnn(double n) {
	double result = 0;
    for (int i = 1; i <= 17; i++) {
		if (i % 2 == 0) {
			result -= interpretator_math::powerab(n - 1, i) / i;
		} else {
			result += interpretator_math::powerab(n - 1, i) / i;
		}
	}
	return result;
}

// Среднее арифемтическое
double interpretator_math::calculateAverage(const std::string& vectorname) {
    if (vars::darrs[vectorname].empty()) {
        throw std::invalid_argument("Vector is empty. Cannot calculate average.");
    }

    double sum = std::accumulate(vars::darrs[vectorname].begin(), vars::darrs[vectorname].end(), 0);
    return sum / vars::darrs[vectorname].size();
}

// Кумулятивная сумма
double interpretator_math::cumsum(const std::string& vectorname) {
    if (vars::darrs[vectorname].empty()) {
        throw std::invalid_argument("Vector is empty. Cannot calculate average.");
    }
    return std::accumulate(vars::darrs[vectorname].begin(), vars::darrs[vectorname].end(), 0);
}

// Дисперсия
double interpretator_math::calcdisp(const std::string& vectorname) {
    if (vars::darrs[vectorname].empty()) {
        throw std::invalid_argument("Vector is empty. Cannot calculate average.");
    }
	double mean = interpretator_math::calculateAverage(vectorname);
	double n = vars::darrs[vectorname].size();
    double sum = 0.0;
    for (const double& num : vars::darrs[vectorname]) {
        sum += (num - mean)*(num - mean);
    }

    return (1/(n-1))*sum;
}

// Функция для генерации случайного числа типа double с равномерным распределением
double interpretator_math::RandomUniform(double min, double max) {
    // Генерация случайного числа
    double random = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX); // Случайное число от 0 до 1
    return min + random * (max - min); // Масштабируем к диапазону [min, max]
}

// Распределение Бернулли
double interpretator_math::RandomBernoulli(double p) {
    return interpretator_math::RandomUniform(0.0, 1.0) < p ? 1 : 0; // Возвращаем 1 с вероятностью p, иначе 0
}

// Распределение Пуассона
double interpretator_math::RandomPoisson(double lambda) {
    double L = expn(-lambda);
    int k = 0;
    double p = 1.0;

    do {
        k++;
        p *= interpretator_math::RandomUniform(0.0, 1.0);
    } while (p > L);

    return k - 1; // Возвращаем значение k
}

// Нормальное распределение
double interpretator_math::RandomNormal(double mean, double stddev) {
	double pi = 3.14159265358979323846;
    double u1 = interpretator_math::RandomUniform(0.0, 1.0); // Первое равномерное случайное число
    double u2 = interpretator_math::RandomUniform(0.0, 1.0); // Второе равномерное случайное число
    double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * pi * u2); // Преобразование Бокса-Мюллера
    return z0 * stddev + mean; // Масштабируем к нормальному распределению
}

void interpretator_math::henkamono(int input_tape, int f1, int f2, int f3, int cyc, int conf1, int conf2, int prob, int output_tape) {
    int input = flags::tape[input_tape]; // not initailazate varibale has garbage
	int output1{}; 
	int output2{}; 
	int output3{};
	int negation_value{}; 
	int not_neg_v{};

    /*
	00 - копирует из одной ячейки в другую.
	01 - сначала прогоняются данные, потом процессы меняются.
	10 - сначала меняются процессы, потом прогоняются данные.
	11 - сначала идёт процесс, потом изменение, потом данные во второй...
	*/

	if (prob == 100) {
		negation_value = 1;
	} else if (prob == 0) {
		negation_value = 0;
	}
	not_neg_v = !negation_value; // negetion_value has garbage becouse not initialization and have garbage
	// then not_neg_v alvais false when prob != 100 or prob!=0
	
	// Изначальные функции
    std::vector<int> functions = {f1, f2, f3};
	
    std::vector<std::vector<int>> history; // Для хранения истории состояний

	if ((conf1 != 0) && (conf2 != 0)) {
		output1 = f1;
		output2 = f2;
		output3 = f3;
	}
	
    while (true) {
		
		if (prob > 0 && prob < 100) {
			// Генерация случайного числа от 0 до 99
			int randomNumber = std::rand() % 100;
			if (randomNumber < prob) {negation_value = 1;} 
			else {negation_value = 0;}
			not_neg_v = !negation_value;
		}
		
		if ((conf1 == 0) || (conf2 == 0)) {
			flags::tape[output_tape] = flags::tape[input_tape];
		} else if ((conf1 == 0) || (conf2 == 1)) {
			// Запоминаем выходы функций
			output1 = (functions[0] == negation_value) ? interpretator_math::_negation(input) : input;
			output2 = (functions[1] == negation_value) ? interpretator_math::_negation(output1) : output1;
			output3 = (functions[2] == negation_value) ? interpretator_math::_negation(output2) : output2;

			// Записываем текущие выходные значения
			flags::tape[f1] = output1;
			flags::tape[f2] = output2;
			flags::tape[f3] = output3;

			// Изменяем процессы на основе выходных значений
			if (output3 == negation_value) {
				functions[1] = (functions[1] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output2 == negation_value) {
				functions[0] = (functions[0] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output1 == negation_value) {
				functions[2] = (functions[2] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
		} else if ((conf1 == 1) || (conf2 == 0)) {
			if (output3 == negation_value) { // output3 may have garbage in first iteration
				functions[1] = (functions[1] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output2 == negation_value) { // output2 may have garbage in first iteration
				functions[0] = (functions[0] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output1 == negation_value) { // output1 may have garbage in first iteration
				functions[2] = (functions[2] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			output1 = (functions[0] == negation_value) ? interpretator_math::_negation(input) : (input);
			output2 = (functions[1] == negation_value) ? interpretator_math::_negation(output1) : (output1);
			output3 = (functions[2] == negation_value) ? interpretator_math::_negation(output2) : output2;
			
			// Записываем текущие выходные значения
			flags::tape[f1] = output1;
			flags::tape[f2] = output2;
			flags::tape[f3] = output3;
			
		} else if ((conf1 == 1) || (conf2 == 1)) {
			output1 = (functions[0] == negation_value) ? _negation(input) : tautology(input);
			if (output3 == negation_value) {  // output3 may have garbage in first iteration
				functions[1] = (functions[1] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			output2 = (functions[1] == negation_value) ? _negation(output1) : tautology(output1);
			if (output2 == negation_value) {  // output2 may have garbage in first iteration
				functions[0] = (functions[0] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			output3 = (functions[2] == negation_value) ? _negation(output2) : tautology(output2);
			if (output1 == negation_value) {  // output1 may have garbage in first iteration
				functions[2] = (functions[2] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			
			// Записываем текущие выходные значения
			flags::tape[f1] = output1;
			flags::tape[f2] = output2;
			flags::tape[f3] = output3;
			
		}
		
        // Запоминаем состояние функций и входного значения
        std::vector<int> currentState = {functions[0], functions[1], functions[2], input};

        // Проверяем, было ли это состояние ранее
        auto it = std::find_if(history.begin(), history.end(), [currentState](const std::vector<int>& state) {return state == currentState;});

        if (it != history.end()) {break;}

        history.push_back(currentState); // Запоминаем текущее состояние

        // Устанавливаем новое входное значение для следующей итерации
        if (cyc == 1) {input = output3;}
		
		flags::tape[output_tape] = output3;
    }

}
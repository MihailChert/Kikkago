#pragma once


#include <vector>

namespace interpretator_math{
template<typename T>
T _negation(T input){return !input;}
template<typename T>
T tautology(T input){return input;}

template<typename To, typename Multiplier>
void sumArray(std::vector<To>& vect, Multiplier multiplier){
	for(To& element : vect){
		element += static_cast<To>(multiplier);
	}
}

template<typename To, typename Multiplier>
void subArray(std::vector<To>& vect, Multiplier multiplier){
	for(To& element : vect){
		element -= static_cast<To>(multiplier);
	}
}

template<typename To, typename Multiplier>
void mulArray(std::vector<To>& vect, Multiplier multiplier){
	for(To& element : vect){
		element *= static_cast<To>(multiplier);
	}
}

template<typename To, typename Multiplier>
void divArray(std::vector<To>& vect, Multiplier multiplier){
	for(To& element : vect){
		element /= static_cast<To>(multiplier);
	}
}
int abs(int a1, int min, int max);
double powerab(double a, int b);
size_t factor(unsigned int a);
size_t cnk(unsigned int n, unsigned int k);
double sinn(double n);
double cosn(double n);
double expn(double n);
double lnn(double n);

double calculateAverage(const std::string& vectorname);
double cumsum(const std::string& vectorname);
double calcdisp(const std::string& vectorname);

std::vector<int> createHistogram(const std::string& vectorname, int numBins);
void henkamono(int input_tape, int f1, int f2, int f3, int cyc, int conf1, int conf2, int prob, int output_tape);

}

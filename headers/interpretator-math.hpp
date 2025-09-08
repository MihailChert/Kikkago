#pragma once


#include <vector>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include "interpretator-flags.hpp"
#include "interpretator-args.hpp"

namespace interpretator_math{
template<typename T>
T _negation(T input){return !input;}
template<typename T>
T tautology(T input){return input;}

template<typename To, typename Multiplier>
void sumArray(std::vector<To>& vect, Multiplier multiplier){
	for(auto el = vect.begin(); el != vect.end(); el++){
		*el = *el + static_cast<To>(multiplier);
	}
}

template<typename To, typename Multiplier>
void subArray(std::vector<To>& vect, Multiplier multiplier){
	for(auto el = vect.begin(); el != vect.end(); el++){
		*el = *el - static_cast<To>(multiplier);
	}
}

template<typename To, typename Multiplier>
void mulArray(std::vector<To>& vect, Multiplier multiplier){
	for(auto el = vect.begin(); el != vect.end(); el++){
		*el = *el * static_cast<To>(multiplier);
	}
}

template<typename To, typename Multiplier>
void divArray(std::vector<To>& vect, Multiplier multiplier){
	if(multiplier == 0){
		throw std::invalid_argument("Division by zero is not allowed.");
	}
	for(auto el = vect.begin(); el != vect.end(); el++){
		*el = *el / static_cast<To>(multiplier);
	}
}
template<typename TO, typename Multiplier>
void modArray(std::vector<TO>& vect, Multiplier multiplier){
	if(multiplier == 0){
		throw std::invalid_argument("ModDivision by zero is not allowed.");
	}
	if(multiplier == 1){
		return;
	}
	for(auto el = vect.begin(); el != vect.end(); el++){
		*el = std::fmod(*el, multiplier);
	}
}

template<typename Nomerator, typename Denomerator>
Nomerator mod(Nomerator nomerator, Denomerator denomerator){
	if(denomerator == 0){
		throw std::invalid_argument("Mod Division by zero is not allowed.");
	}
	if(denomerator > 0){
		return std::fmod(nomerator, denomerator);
	}else{
		return std::fmod(nomerator, denomerator) + denomerator;
	}
}

int abs_mod(int a1, int min, int max);
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

double RandomUniform(double min, double max);
double RandomBernoulli(double p);
double RandomPoisson(double lambda);
double RandomNormal(double mean, double stddev);
}

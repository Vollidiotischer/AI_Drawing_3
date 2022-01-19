#pragma once

#include "Variables.h"

#include <iostream>
#include <vector> 
#include <cmath>
#include <random>
#include <time.h>

struct AI {
	double generation_score = 0; 

	std::vector<std::vector<double>> nodes;
	std::vector<std::vector<double>> weights;
	std::vector<std::vector<double>> biases;

	std::normal_distribution<double> dis;
	std::mt19937 gen;

	AI(std::vector<int> nodeSize) {
		// Initialize random generators
		dis = std::normal_distribution<double>(0.0, 1.0);
		gen = std::mt19937(rand() % 1000); 

		// Initialize biases
		for (int i = 0; i < nodeSize.size(); i++) {
			nodes.push_back(std::vector<double>(nodeSize[i]));
			biases.push_back(std::vector<double>(nodeSize[i]));

			for (int i2 = 0; i2 < biases[i].size(); i2++) {
				biases[i][i2] = dis(gen);
			}

		}

		// Initialize Weights
		for (int i = 0; i < nodeSize.size() - 1; i++) {
			weights.push_back(std::vector<double>(nodeSize[i] * nodeSize[i + 1]));

			for (int i2 = 0; i2 < weights[i].size(); i2++) {
				weights[i][i2] = dis(gen) / sqrt((double)nodes[i].size()); // IDFK WHAT IS GOING ON HERE
			}

		}
	}

	std::vector<double> input(std::vector<double> inp) {

		if (inp.size() != nodes[0].size()) {
			std::cout << "Invalid Input" << std::endl; 
			return { 0 };
		}

		double temp = 0;

		for (int i = 0; i < nodes[0].size(); i++) {
			nodes[0][i] = inp[i];
		}

		for (int i = 1; i < nodes.size(); i++) {

			for (int i2 = 0; i2 < nodes[i].size(); i2++) {

				for (int i3 = 0; i3 < nodes[i - 1].size(); i3++) {

					temp += nodes[i - 1][i3] * weights[i - 1][i3 + i2 * nodes[i - 1].size()];

				}

				temp += biases[i][i2];

				nodes[i][i2] = activation(temp);
				temp = 0;

			}

		}

		return nodes[nodes.size() - 1];

	}

	void train(AI& parent_ai) {

		// Generate adapted weights
		for (int i = 0; i < weights.size(); i++) {
			for (int i2 = 0; i2 < weights[i].size(); i2++) {
				this->weights[i][i2] = parent_ai.weights[i][i2] + dis(gen) * AI_RNG_FACTOR;
			}
		}

		// Generate adapted biases 
		for (int i = 0; i < biases.size(); i++) {
			for (int i2 = 0; i2 < biases[i].size(); i2++) {
				this->biases[i][i2] = parent_ai.biases[i][i2] + dis(gen) * AI_RNG_FACTOR;
			}
		}

	}

	float activation(float x) {
		return 1 / (1 + pow(2.7182818284, -x));
	}

};
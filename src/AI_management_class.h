#pragma once


#include "Variables.h"
#include "AI.h"
#include <fstream>
/*

Die verteilung muss in dem AI_manager initialisiert werden, da sonst alle AI's den gleichen seed in ihren random-generator bekommen

*/

#include <string>
#include <vector>
#include <array>


// Convert AI Pattern to string
#define XSTR(x) STR(x)
#define STR(x) #x


void parseInp2Space(std::string, double&, double&, double&);
bool read_ai_blocks(AI&);
void save_ai_blocks(AI&);
std::array<bool, 100> str_to_bool_arr(std::string&);


struct AI_manager {
	std::vector<AI> ais; 

	AI* utility_ai; 
	double max_ai_score = 0; 


	AI_manager() {
		utility_ai = new AI({ AI_LAYOUT }); 
		
	}

	void evaluate_ai_instances(std::vector<std::array<bool, 100>>& data, std::vector<short>& solutions) {
		this->max_ai_score = 0; 

		double max_score = -99999999; 
		int max_score_pos = 0; 

		for (int data_index = 0; data_index < data.size(); data_index++) {

			std::vector<double> ai_input(data[data_index].begin(), data[data_index].end());

			for (int ai_index = 0; ai_index < ais.size(); ai_index++) {

				// let the ai calculate the results
				std::vector<double> results = ais[ai_index].input(ai_input);

				// get the position of the correct number
				int pos_of_correct = solutions[data_index];

				// give the ai an score based on performance:
				// score = correct_guess - sum(incorrect_guesses)
				
				for (int score_i = 0; score_i < results.size(); score_i++) {
					if (score_i != pos_of_correct) {
						ais[ai_index].generation_score -= results[score_i];
					}
					else {
						ais[ai_index].generation_score += results[score_i];

					}
				}

			}

		}

		for (int i = 0; i < ais.size(); i++) {
			if (ais[i].generation_score > max_score) {
				max_score = ais[i].generation_score; 
				max_score_pos = i; 
			}
		}


		// save best ai in utility_ai -> so it can be used later
		max_ai_score = ais[max_score_pos].generation_score; 
		this->utility_ai = &ais[max_score_pos];
		 
	}

	void adjust_ai_weights_and_biases(AI& parent_ai) {
		
		// save parent in 0th slot, so that our generations dont get worse
		// train ais
		// reset generation score
		parent_ai.generation_score = 0; 
		ais[0] = parent_ai; 
		for (int i = 1; i < ais.size() - AI_RNG; i++) {
			ais[i].train(parent_ai); 
			ais[i].generation_score = 0; 
		}

		for (int i = ais.size() - 1; i > ais.size() - AI_RNG - 1; i--) {
			ais[i] = AI({ AI_LAYOUT });
		}


	}

	void save_ai_instance(AI& ai) {
		// saves the given ai instance into 'AI_BLOCKS.txt'
		save_ai_blocks(ai); 
	}

	bool load_saved_instance(AI& ai){
		// loads the AI's weights and biases saved in 'AI_BLOCKS.txt' into 'ai'
		return read_ai_blocks(ai); 
	}

	void initialize_new_ai_instances() {
		ais.clear(); 
		for (int i = 0; i < AI_COUNT; i++) {
			ais.push_back(AI({ AI_LAYOUT }));
		}
	}

	void cleanup() {
		delete utility_ai; 
	}
};


void save_ai_blocks(AI& ai) {

	std::ofstream text_file_write;

	text_file_write.open("AI_Blocks.txt", std::ofstream::trunc);

	text_file_write << XSTR(AI_LAYOUT) << "\n";
	for (int i = 0; i < ai.weights.size(); i++) {
		for (int i2 = 0; i2 < ai.weights[i].size(); i2++) {
			text_file_write << i << " " << i2 << " " << ai.weights[i][i2] << "\n";
		}
	}

	text_file_write << "-\n";


	for (int i = 0; i < ai.biases.size(); i++) {
		for (int i2 = 0; i2 < ai.biases[i].size(); i2++) {
			text_file_write << i << " " << i2 << " " << ai.biases[i][i2] << "\n";
		}
	}

	text_file_write.close();
}

bool read_ai_blocks(AI& ai) {

	std::ifstream text_file_read;

	text_file_read.open("AI_BLOCKS.txt");

	if (!text_file_read.is_open()) {
		// ai save file could not be found; return new random ai
		std::cout << "[read_ai_blocks] Could not open th File to read from!" << std::endl;
		return false;
	}

	std::string temp_read_string;
	double i, i2, w;

	bool weights = true;

	// Read first line of AI_Blocks (AI pattern) 
	getline(text_file_read, temp_read_string);

	// check if saved AI_Blocks are compatible with used AI
	if (temp_read_string != XSTR(AI_LAYOUT)) {
		// ai save file is incompatible with current ai type; return random ai
		std::cout << "[read_ai_blocks]AI Layouts do not match!" << std::endl;
		return false;
	}

	// if compatible: read AI weights and biases
	while (getline(text_file_read, temp_read_string)) {

		if (temp_read_string != "-") {

			parseInp2Space(temp_read_string, i, i2, w);

			if (weights) {
				ai.weights[i][i2] = w;
			}
			else {
				ai.biases[i][i2] = w;
			}
		}
		else {
			weights = false;
		}
	}


	text_file_read.close();

	std::cout << "[read_ai_blocks] Loaded AI successfully!" << std::endl;

	return true;
}

void parseInp2Space(std::string s, double& inp1, double& inp2, double& inp3) {
	//21 536 1234142 pos1 = 2 i = 6
	int pos1 = 0;

	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ' && pos1 == 0) {
			pos1 = i;
		}
		else {
			if (s[i] == ' ' && pos1 != 0) {
				inp1 = std::stod(s.substr(0, pos1));
				inp2 = std::stod(s.substr(pos1 + 1, i - (pos1 + 1)));
				inp3 = std::stod(s.substr(i + 1));
				return;
			}
		}
	}

}

void load_training_data(std::vector<std::array<bool, 100>>& data_array, std::vector<short>& solution_array) {

	std::ifstream text_file_read;
	std::string reading_string; 
	text_file_read.open("saved_maps.txt");

	if (!text_file_read.is_open()) {
		std::cout << "[load_training_data] could not open saved maps" << std::endl; 
		return; 
	}

	while (getline(text_file_read, reading_string)) {
		solution_array.push_back(reading_string[0] - (int)'0'); // get number at the begining as int
		data_array.push_back(str_to_bool_arr(reading_string)); 
	}

	text_file_read.close(); 

	return; 

}

std::array<bool, 100> str_to_bool_arr(std::string& inp) {

	std::array<bool, 100> bin_arr;
	std::fill_n(bin_arr.begin(), 100, 0); 

	for (int i = 1; i < inp.size(); i++) {
		bin_arr[i - 1] = inp[i] - (int)'0'; 
	}

	return bin_arr; 
}


#include "Variables.h"
#include "AI.h"
#include "AI_management_class.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

void loop_breaker(); 
void ai_training(); 
void ai_testing(); 
void load_training_data(std::vector<std::array<bool, 100>>&, std::vector<short>&);

extern std::array<bool, 100> canvas_transmitter; 
extern bool break_out = false; 

void start_ai(std::string option) {
	
	// srand needs to be seedet once per thread (this is the entrance to this thread)
	srand(time(NULL)); 

	// start ai training
	if (option == "train") {
		ai_training(); 
	}

	// start ai testing
	if (option == "test") {
		ai_testing(); 
	}

	// data is being cerated - no ai activation should happen
	if (option == "none") {
		return; 
	}

}

// train the ai on the dataset
void ai_training() {
	AI_manager manager;

	//std::string temp; 
	//std::cin >> temp; 
	double ai_score_percentage = 75;//stoi(temp);


	// load saved instance of ai
	if (manager.load_saved_instance(*manager.utility_ai)) {
		// adjust future ais based on loaded ais
		manager.initialize_new_ai_instances(); 

	}
	else {
		// could not load saved ai
		// create new instances of ais
		manager.initialize_new_ai_instances();

	}

	// load training data (possibly to much memory is being used) 
	std::vector<std::array<bool, 100>> training_data; 
	std::vector<short> solution_data; 
	load_training_data(training_data, solution_data); 

	std::thread break_thread(loop_breaker); 

	while (manager.max_ai_score < ((double)training_data.size() / 100.0) * ai_score_percentage && !break_out) {  

		manager.adjust_ai_weights_and_biases(*manager.utility_ai);


		manager.evaluate_ai_instances(training_data, solution_data);


		std::cout << "current score: " << manager.max_ai_score << "/" << ((double)training_data.size() / 100.0) * ai_score_percentage << std::endl; 

	}

	std::cout << "AI trained to score " << manager.max_ai_score << std::endl;
	
	save_ai_blocks(*manager.utility_ai); 

	std::cout << "saved AI" << std::endl; 

	break_thread.join();


	//manager.cleanup(); 
}

// test the ai on individual samples
void ai_testing() {
	AI trained_ai({ AI_LAYOUT });

	read_ai_blocks(trained_ai); 

	while (true) {

		std::vector<double> ai_input(canvas_transmitter.begin(), canvas_transmitter.end());

		std::vector<double> results = trained_ai.input({ ai_input }); // input data

		// get pos of max value in results
		std::vector<double>::iterator iterator = std::max_element(results.begin(), results.end());
		int pos_of_max = std::distance(results.begin(), iterator);

		std::cout << "AI said " << pos_of_max << std::endl; 

		std::this_thread::sleep_for(std::chrono::milliseconds(500)); 

	}
}

void loop_breaker() {
	char c; 

	std::cin >> c; 

	if (c == 'q') {
		break_out = true; 
	}
}
#include <iostream>
#include <thread>


void start_canvas(); 
void start_ai(std::string); 



int main(){

	//std::thread canvas_thread(start_canvas); 

	std::thread ai_thread(start_ai, "train"); 

	//canvas_thread.join(); 
	ai_thread.join(); 
}

/*
launch drawing thread

launch ai thread
	- decision weather training data is being creaed
	- weather ai is being trained
	- weather ai is being tested


load ai

create Instances

evaluate Instances

adjust instances (create based on best one) 
*/


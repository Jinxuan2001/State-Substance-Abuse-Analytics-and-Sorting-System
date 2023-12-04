/*
    Project 3: State Substance Abuse Analytics and Sorting System
    COP3530
    University of Florida
    Team Member: Jinxuan Liu and Vijjeara Long

	This program processes substance abuse data from a CSV file,
	which contains information on various types of substance abuse in each US state, particularly for the year 2018.
	It enables the user to sort this data by either the total number or the total rate of substance abuse, using different sorting algorithms.
	The program offers the flexibility to choose between these sorting algorithms and the sorting criteria (number or rate).
	Additionally, it analyzes and compares the efficiency of the different algorithms by measuring and reporting their operational times.

	Algorithms Used: QUICK SORT and HEAP SORT are used for sorting the states by the total number/rate of substance abuse;
	                 MERGE SORT is used for sorting different types of substance abuse in each state.



	Last Update: 04/12/2023
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <chrono>
#include <utility>

using namespace std;
using namespace std::chrono;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Structure to store state data and substance abuse information
struct StateData {
	string state_name;
	long state_population;
	int abuse_number;
	float abuse_rate;
	int alcohol_abuse;
	int tobacco_abuse;
	int cocaine_abuse;
	int marijuana_abuse;
	vector<pair<string, int>> drug_types;

	// Constructor
	StateData(string name, long population, int number, float rate, int alcohol, int tobacco, int cocaine, int marijuana) :
		state_name(name), state_population(population), abuse_number(number), abuse_rate(rate), 
		alcohol_abuse(alcohol), tobacco_abuse(tobacco), cocaine_abuse(cocaine), marijuana_abuse(marijuana) {}

	// Method to determine the substance with the highest abuse number
	string theWorstType();

	// Merge sort algorithm for sorting substance types in each state by the number of abuse
	void mergeSortDrugTypes(vector<pair<string, int>>& types, int start, int end);
	void merge(vector<pair<string, int>>& types, int left, int mid, int right);
};

string StateData::theWorstType() {
	drug_types.push_back(make_pair("Alcohol Abuse", alcohol_abuse));
	drug_types.push_back(make_pair("Tobacco Abuse", tobacco_abuse));
	drug_types.push_back(make_pair("Cocaine Abuse", cocaine_abuse));
	drug_types.push_back(make_pair("Marijuana Abuse", marijuana_abuse));

	mergeSortDrugTypes(drug_types, 0, drug_types.size() - 1);
	return drug_types[drug_types.size() - 1].first;
}

void StateData::mergeSortDrugTypes(vector<pair<string, int>>& types, int start, int end) {
	if (start < end) {
		int mid = (start + end) / 2;
		mergeSortDrugTypes(types, start, mid);
		mergeSortDrugTypes(types, mid + 1, end);
		merge(types, start, mid, end);
	}
}

void StateData::merge(vector<pair<string, int>>& types, int left, int mid, int right) {
	vector<pair<string, int>> leftSub(types.begin() + left, types.begin() + mid + 1);
	vector<pair<string, int>> rightSub(types.begin() + mid + 1, types.begin() + right + 1);

	int leftIndex = 0, rightIndex = 0, mergeIndex = left;

	while (leftIndex < leftSub.size() && rightIndex < rightSub.size()) {
		if (leftSub[leftIndex].second <= rightSub[rightIndex].second) {
			types[mergeIndex++] = leftSub[leftIndex++];
		}
		else {
			types[mergeIndex++] = rightSub[rightIndex++];
		}
	}

	while (leftIndex < leftSub.size()) {
		types[mergeIndex++] = leftSub[leftIndex++];
	}

	while (rightIndex < rightSub.size()) {
		types[mergeIndex++] = rightSub[rightIndex++];
	}
}


void swap(StateData* a, StateData* b) {
	StateData tmp = *a;
	*a = *b;
	*b = tmp;
}

int numberPartition(vector<StateData>& states, int low, int high) {
	int pivot = states[high].abuse_number;
	int up = low;
	int down = high - 1;

	while (up <= down) {
		while (up <= down && states[up].abuse_number <= pivot) {
			up++;
		}
		while (up <= down && states[down].abuse_number > pivot) {
			down--;
		}
		if (up < down) {
			swap(states[up], states[down]);
		}
	}

	swap(states[up], states[high]);
	return up;
}

int ratePartition(vector<StateData>& states, int low, int high) {
	float pivot = states[high].abuse_rate;
	int up = low;
	int down = high - 1;

	while (up <= down) {
		while (up <= down && states[up].abuse_rate <= pivot) {
			up++;
		}
		while (up <= down && states[down].abuse_rate > pivot) {
			down--;
		}
		if (up < down) {
			swap(states[up], states[down]);
		}
	}

	swap(states[up], states[high]);
	return up;
}

void quickSortByNumber(vector<StateData>& states, int low, int high) {
	if (low < high) {
		int pivot = numberPartition(states, low, high);
		quickSortByNumber(states, low, pivot - 1);
		quickSortByNumber(states, pivot + 1, high);
	}
}

void quickSortByRate(vector<StateData>& states, int low, int high) {
	if (low < high) {
		int pivot = ratePartition(states, low, high);
		quickSortByRate(states, low, pivot - 1);
		quickSortByRate(states, pivot + 1, high);
	}
}

void numberHeapify(vector<StateData>& states, int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && states[left].abuse_number > states[largest].abuse_number) {
		largest = left;
	}
	if (right < n && states[right].abuse_number > states[largest].abuse_number) {
		largest = right;
	}

	if (largest != i) {
		swap(states[i], states[largest]);
		numberHeapify(states, n, largest);
	}
}

void rateHeapify(vector<StateData>& states, int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && states[left].abuse_rate > states[largest].abuse_rate) {
		largest = left;
	}
	if (right < n && states[right].abuse_rate > states[largest].abuse_rate) {
		largest = right;
	}

	if (largest != i) {
		swap(states[i], states[largest]);
		rateHeapify(states, n, largest);
	}
}

void heapSortByNumber(vector<StateData>& states) {
	int n = states.size();

	for (int i = n / 2 - 1; i >= 0; i--) {
		numberHeapify(states, n, i);
	}

	for (int i = n - 1; i >= 0; i--) {
		swap(states[0], states[i]);
		numberHeapify(states, i, 0);
	}
}

void heapSortByRate(vector<StateData>& states) {
	int n = states.size();

	for (int i = n / 2 - 1; i >= 0; i--) {
		rateHeapify(states, n, i);
	}

	for (int i = n - 1; i >= 0; i--) {
		swap(states[0], states[i]);
		rateHeapify(states, i, 0);
	}
}

vector<StateData> readFile(const string& file_name) {
	vector<StateData> states;
	ifstream file(file_name);
	string line;
	string name;
	long population;
	int number;
	float rate;

	// Skip the first 817 lines, including the header, read data for 2018 only
	for (int i = 0; i < 817; i++) {
		getline(file, line);
	}

	// Process each subsequent line
	while (getline(file, line)) {
		stringstream stream(line);

		// Extract the state name
		if (!getline(stream, name, ',')) break;

		// Skip the second column
		stream.ignore(numeric_limits<streamsize>::max(), ',');

		// Reset population and number for each line
		population = 0;
		number = 0;

		// Add the data from columns C to E for population
		for (int i = 0; i < 3; i++) {
			string temp;
			if (!getline(stream, temp, ',')) break;
			temp.erase(remove(temp.begin(), temp.end(), '"'), temp.end());

			try {
				population += stol(temp);
			}
			catch (const invalid_argument& e) {
				cerr << "Invalid argument: " << e.what() << " when processing population for state " << name << endl;
				population = 0;
				break;
			}
			catch (const out_of_range& e) {
				cerr << "Out of range: " << e.what() << " when processing population for state " << name << endl;
				population = 0;
				break;
			}
		}

		int al = 0;
		int tb = 0;
		int cc = 0;
		int ma = 0;

		// Starting from column F, add every first three out of six columns to number
		int colIndex = 6; // Start from column F
		while (stream.good() && colIndex <= 53) { // Assuming column BA is the last one needed
			int sum = 0;
			for (int i = 0; i < 3; i++, colIndex++) {
				string temp;
				if (!getline(stream, temp, ',')) break;
				temp.erase(remove(temp.begin(), temp.end(), '"'), temp.end());

				try {
					// Add the data to the four different types of abuse in each state
					if (colIndex >= 6 && colIndex <= 8) {// Alcohol
						al += stoi(temp);
					}
					else if (colIndex >= 12 && colIndex <= 14) {// Alcohol
						al += stoi(temp);
					}
					else if (colIndex >= 18 && colIndex <= 20) {// Tobacco
						tb += stoi(temp);
					}
					else if (colIndex >= 24 && colIndex <= 26) {// Cocaine
						cc += stoi(temp);
					}
					else if (colIndex >= 30 && colIndex <= 32) {// Marijuana
						ma += stoi(temp);
					}
					else if (colIndex >= 36 && colIndex <= 38) {// Marijuana
						ma += stoi(temp);
					}
					else if (colIndex >= 42 && colIndex <= 44) {// Marijuana
						ma += stoi(temp);
					}
					else if (colIndex >= 48 && colIndex <= 50) {// Tobacco
						tb += stoi(temp);
					}

					sum += stoi(temp);
				}
				catch (const invalid_argument& e) {
					cerr << "Invalid argument: " << e.what() << " when processing number for state " << name << endl;
				}
				catch (const out_of_range& e) {
					cerr << "Out of range: " << e.what() << " when processing number for state " << name << endl;
				}
			}

			number += sum;

			// Skip the next three columns
			for (int i = 0; i < 3; i++, colIndex++) {
				stream.ignore(numeric_limits<streamsize>::max(), ',');
			}
		}

		// Calculate rate, ensuring no division by zero
		rate = (population > 0) ? static_cast<float>(number) * 1000.0f / static_cast<float>(population) : 0.0f;

		// Add the new StateData object to the vector
		states.emplace_back(name, population, number, rate, al, tb, cc, ma);
	}

	file.close();
	return states;
}

void printStatesNum(vector<StateData>& states) {
	for (StateData& state : states) {
		cout << state.state_name << ": Population = " << state.state_population
			<< ", Abuse Number = " << state.abuse_number << ", The Worst Type: "
			<< state.theWorstType() << endl;
	}
}

void printStatesRate(vector<StateData>& states) {
	for (StateData& state : states) {
		cout << state.state_name << ": Population = " << state.state_population
			<< ", Abuse Rate = " << state.abuse_rate << ", The Worst Type: "
			<< state.theWorstType() << endl;
	}
}

int main() {
	string file_name = "drugs.csv";
	vector<StateData> states = readFile(file_name);

	int choice1, choice2, choice3;
	bool cycle = true;

	while (cycle) {
		while (true) {
			cout << "Please choose a category:" << endl << "1. State Substance Abuse Number" << endl << "2. State Substance Abuse Rate" << endl;
			cin >> choice1;
			if (choice1 == 1 || choice1 == 2) {
				break;
			}
			else {
				cout << "Please enter a valid choice." << endl;;
			}
		}
		while (true) {
			cout << "Please choose a sorting method:" << endl << "1. Quick Sort" << endl << "2. Heap sort" << endl;
			cin >> choice2;
			if (choice2 == 1 || choice2 == 2) {
				break;
			}
			else {
				cout << "Please enter a valid choice." << endl;;
			}
		}


		if (choice1 == 1 && choice2 == 1) {
			auto start = high_resolution_clock::now();
			quickSortByNumber(states, 0, states.size() - 1);
			printStatesNum(states);
			cout << endl;
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "The quick sort method took: " << duration.count() << " microseconds." << endl << endl;
		}
		else if (choice1 == 1 && choice2 == 2) {
			auto start = high_resolution_clock::now();
			quickSortByRate(states, 0, states.size() - 1);
			printStatesRate(states);
			cout << endl;
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "The heap sort method took: " << duration.count() << " microseconds." << endl << endl;
		}
		else if (choice1 == 2 && choice2 == 1) {
			auto start = high_resolution_clock::now();
			heapSortByNumber(states);
			printStatesNum(states);
			cout << endl;
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "The quick sort method took: " << duration.count() << " microseconds." << endl << endl;
		}
		if (choice1 == 2 && choice2 == 2) {
			auto start = high_resolution_clock::now();
			heapSortByRate(states);
			printStatesRate(states);
			cout << endl;
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "The heap sort method took: " << duration.count() << " microseconds." << endl << endl;
		}

		cout << "Do you want to sort using another category and / or method?" << endl << "1. Yes" << endl << "2. No" << endl;
		cin >> choice3;

		if (choice3 == 1) {
			cycle = true;
		}
		else {
			cycle = false;
		}
	}
	return 0;
}

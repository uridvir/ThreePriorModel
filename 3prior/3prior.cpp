// 3prior.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ModelBackend.h"
#include <fstream>

int main(int argc, char* argv[])
{	
	if (argc == 2 && std::strcmp(argv[1], "--help") == 0) {
		std::cout << "To run the model:\n";
		std::cout << "3prior <input> <output>\n\n";

		std::cout << "<input>    - CSV file with prior inputs and election results\n";
		std::cout << "<output>   - CSV file for projections\n\n\n";


		std::cout << "To download an input template:\n";
		std::cout << "3prior -t [template]\n\n";

		std::cout << "[template] - CSV input template for priors and election results, by default \"template.csv\"\n";
		return 0;
	}

	if ((argc == 2 || argc == 3) && std::strcmp(argv[1], "-t") == 0) { //Template download mode
		std::ofstream out;
		try {
			if (argc == 2) {
				out.open("template.csv", std::ios::trunc);
			}
			else {
				out.open(argv[2], std::ios::trunc);
			}
			//TODO: Write template to file
			out.close();
		}
		catch (std::exception e) {
			std::cout << "File output error!\n";
			std::cout << e.what() << "\n";
		}
		return 0;
	}

	if (argc != 3) { //Bad usage
		std::cout << "For correct usage, type \"3prior --help\".\n";
		return 0;
	}
	else { //Run the model
		std::stringstream buff;
		std::ifstream in;
		try {
			in.open(argv[1]);
			buff << in.rdbuf();
			in.close();
		}
		catch (std::exception e) {
			std::cout << "File input error!\n";
			std::cout << e.what() << "\n";
			return 0;
		}
		std::string csvOut;
		try {
			csvOut = exportCSV(runModel(getInternals(parseCSV(buff.str()))));
		}
		catch (std::logic_error e) { //Catch exceptions thrown explicitly by the model
			std::cout << "Model error:\n";
			std::cout << e.what() << "\n";
			return 0;
		}
		std::ofstream out;
		try {
			out.open(argv[2], std::ios::trunc);
			out << csvOut;
			out.close();
		}
		catch (std::exception e) {
			std::cout << "File output error!\n";
			std::cout << e.what() << "\n";
			return 0;
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

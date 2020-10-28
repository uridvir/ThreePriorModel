#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <ios>

struct ExternalModelInputs {

	//Priors
	double pollingPercentDemocrat;
	double pollingPercentRepublican;
	double mailinPercentDemocrat;
	double mailinPercentRepublican;
	double mailinPercentThirdParty;
	double mailinTurnout;
	double earlyPercentDemocrat;
	double earlyPercentRepublican;
	double earlyPercentThirdParty;
	double earlyTurnout;
	double adjustConstant;
	double defectorsPercentDemocrat;
	double defectorsPercentRepublican;
	double independentPercentVotesRepublican;
	double totalTurnout;

	//Data
	int lines;
	std::vector<double> electiondayDemocratVotes;
	std::vector<double> electiondayRepublicanVotes;
	std::vector<double> electiondayThirdPartyVotes;
	std::vector<double> mailinDemocratVotes;
	std::vector<double> mailinRepublicanVotes;
	std::vector<double> mailinThirdPartyVotes;
	std::vector<double> earlyDemocratVotes;
	std::vector<double> earlyRepublicanVotes;
	std::vector<double> earlyThirdPartyVotes;

};

struct InternalModelInputs {
	
	//Priors
	double electiondayPriorLean;
	double mailinPriorLean;
	double earlyPriorLean;
	double electiondayTotalShare;
	double mailinTotalShare;
	double earlyTotalShare;

	//Data
	int lines;
	std::vector<double> electiondayLean;
	std::vector<double> mailinLean;
	std::vector<double> earlyLean;
	std::vector<double> electiondayShare;
	std::vector<double> mailinShare;
	std::vector<double> earlyShare;

};

ExternalModelInputs parseCSV(std::string contents);

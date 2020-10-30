#include "ModelBackend.h"
#undef max //Disables annoying max macro

ExternalModelInputs parseCSV(std::string contents) {
	std::istringstream sstream(contents);
	ExternalModelInputs inputsExt = { 0 };
	int currentLine = 1;
	while (!sstream.eof()) {
		switch (currentLine) {
		case 1:
		case 2:
		case 4:
		case 6:
		case 8:
		case 9:
		case 10:
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		case 3: //Polling % D, Polling % R, Mail-in % D, Mail-in % R, Mail-in % I, Mail-in turnout
			sstream >> inputsExt.pollingPercentDemocrat;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.pollingPercentRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinPercentDemocrat;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinPercentRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinPercentIndependent;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinTurnout;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		case 5: // , , Early % D, Early % R, Early % I, Early turnout
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyPercentDemocrat;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyPercentRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyPercentIndependent;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyTurnout;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		case 7: //Adjust constant,	, Defectors % D, Defectors % R,	Ind. % R, Total Turnout
			sstream >> inputsExt.adjust;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.defectorsPercentDemocrat;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.defectorsPercentRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.independentPercentVotesRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.totalTurnout;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		default: //Election-day D, Election-day R,	Election-day 3,	Mail-in D,	Mail-in R,	Mail-in 3,	Early D,	Early R,	Early 3
			inputsExt.lines = currentLine - 10;

			inputsExt.electiondayDemocratVotes.push_back(0);
			inputsExt.electiondayRepublicanVotes.push_back(0);
			inputsExt.electiondayThirdPartyVotes.push_back(0);
			inputsExt.mailinDemocratVotes.push_back(0);
			inputsExt.mailinRepublicanVotes.push_back(0);
			inputsExt.mailinThirdPartyVotes.push_back(0);
			inputsExt.earlyDemocratVotes.push_back(0);
			inputsExt.earlyRepublicanVotes.push_back(0);
			inputsExt.earlyThirdPartyVotes.push_back(0);

			sstream >> inputsExt.electiondayDemocratVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.electiondayRepublicanVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.electiondayThirdPartyVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinDemocratVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinRepublicanVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinThirdPartyVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyDemocratVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyRepublicanVotes[inputsExt.lines - 1];
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyThirdPartyVotes[inputsExt.lines - 1];

			sstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		currentLine++;
		if (sstream.fail()) {
			for (auto arr : { inputsExt.electiondayDemocratVotes, inputsExt.electiondayRepublicanVotes, inputsExt.electiondayThirdPartyVotes,
				inputsExt.mailinDemocratVotes, inputsExt.mailinRepublicanVotes, inputsExt.mailinThirdPartyVotes,
				inputsExt.earlyDemocratVotes, inputsExt.earlyRepublicanVotes, inputsExt.earlyThirdPartyVotes }) {
				arr.pop_back();
			}
			inputsExt.lines--;
			break;
		}
	}
	return inputsExt;
}

//TODO: Add divide-by-zero protection
/*
Converts data to mathematically convenient format (normalize % to 1, convert numbers into shares, etc.)
*/
InternalModelInputs getInternals(ExternalModelInputs inputsExt) {
	InternalModelInputs inputsInt = { 0 };

	inputsInt.mailinPriorLean = (inputsExt.mailinPercentRepublican + inputsExt.mailinPercentIndependent * inputsExt.independentPercentVotesRepublican / 100.0)
		/ (inputsExt.mailinPercentDemocrat + inputsExt.mailinPercentRepublican + inputsExt.mailinPercentIndependent);
	inputsInt.earlyPriorLean = (inputsExt.earlyPercentRepublican + inputsExt.earlyPercentIndependent * inputsExt.independentPercentVotesRepublican / 100.0)
		/ (inputsExt.earlyPercentDemocrat + inputsExt.earlyPercentRepublican + inputsExt.earlyPercentIndependent);

	inputsInt.mailinTotalShare = inputsExt.mailinTurnout / inputsExt.totalTurnout;
	inputsInt.earlyTotalShare = inputsExt.earlyTurnout / inputsExt.totalTurnout;

	inputsInt.pollLean = inputsExt.pollingPercentRepublican / 100.0 + (100.0 - inputsExt.pollingPercentDemocrat - inputsExt.pollingPercentRepublican) / 200.0;
	inputsInt.electiondayTotalShare = 1.0 - inputsInt.mailinTotalShare - inputsInt.earlyTotalShare;
	
	inputsInt.electiondayPriorLean = (inputsInt.pollLean - inputsInt.mailinPriorLean * inputsInt.mailinTotalShare
		- inputsInt.earlyPriorLean * inputsInt.earlyTotalShare) / inputsInt.electiondayTotalShare;

	inputsInt.adjust = inputsExt.adjust;

	inputsInt.lines = inputsExt.lines;
	for (int i = 0; i < inputsInt.lines; i++) {
		inputsInt.electiondayLean.push_back(inputsExt.electiondayRepublicanVotes[i] / (inputsExt.electiondayRepublicanVotes[i] + inputsExt.electiondayDemocratVotes[i]));
		inputsInt.electiondayShare.push_back((inputsExt.electiondayDemocratVotes[i] + inputsExt.electiondayRepublicanVotes[i] + inputsExt.electiondayThirdPartyVotes[i])
			/ (inputsInt.electiondayTotalShare * inputsExt.totalTurnout));

		inputsInt.mailinLean.push_back(inputsExt.mailinRepublicanVotes[i] / (inputsExt.mailinRepublicanVotes[i] + inputsExt.mailinDemocratVotes[i]));
		inputsInt.mailinShare.push_back((inputsExt.mailinDemocratVotes[i] + inputsExt.mailinRepublicanVotes[i] + inputsExt.mailinThirdPartyVotes[i]) / inputsExt.mailinTurnout);

		inputsInt.earlyLean.push_back(inputsExt.earlyRepublicanVotes[i] / (inputsExt.earlyRepublicanVotes[i] + inputsExt.earlyDemocratVotes[i]));
		inputsInt.earlyShare.push_back((inputsExt.earlyDemocratVotes[i] + inputsExt.earlyRepublicanVotes[i] + inputsExt.earlyThirdPartyVotes[i]) / inputsExt.earlyTurnout);
	}

	return inputsInt;
}

double getProjection(double priorLean, double lean, double share, double adjust) {
	double newPriorLean = (priorLean - adjust * lean * share) / (1 - adjust * share);
	double newPriorShare = 1 - share;
	if (newPriorLean < 0) {
		newPriorShare *= 1 + newPriorLean;
		newPriorLean = 0;
	}
	else if (newPriorLean > 1) {
		newPriorShare *= 2 - newPriorLean;
		newPriorLean = 1;
	}
	return newPriorLean * newPriorShare + lean * (1 - newPriorShare);
}

ModelOutputs runModel(InternalModelInputs inputs) {
	ModelOutputs outputs = { 0 };
	outputs.lines = inputs.lines;
	outputs.pollLean = inputs.pollLean;

	for (int i = 0; i < inputs.lines; i++) {
		outputs.shareReported.push_back(inputs.electiondayShare[i] * inputs.electiondayTotalShare + inputs.mailinShare[i] * inputs.mailinTotalShare
			+ inputs.earlyShare[i] * inputs.earlyTotalShare);
		if (outputs.shareReported[i] != 0) {
			outputs.leanReported.push_back((inputs.electiondayLean[i] * inputs.electiondayShare[i] * inputs.electiondayTotalShare 
				+ inputs.mailinLean[i] * inputs.mailinShare[i] * inputs.mailinTotalShare
				+ inputs.earlyLean[i] * inputs.earlyShare[i] * inputs.earlyTotalShare) / outputs.shareReported[i]);
		}
		else {
			outputs.leanReported.push_back(0.5);
		}
		outputs.projection.push_back(
			getProjection(inputs.electiondayPriorLean, inputs.electiondayLean[i], inputs.electiondayShare[i], inputs.adjust) * inputs.electiondayTotalShare +
			getProjection(inputs.mailinPriorLean, inputs.mailinLean[i], inputs.mailinShare[i], inputs.adjust) * inputs.mailinTotalShare +
			getProjection(inputs.earlyPriorLean, inputs.earlyLean[i], inputs.earlyShare[i], inputs.adjust) * inputs.earlyTotalShare
		);
	}

	return outputs;
}

std::string exportCSV(ModelOutputs outputs) {
	std::ostringstream sstream;
	sstream << "Share %,Lean % R,Projection % R,Polls % R\n";

	for (int i = 0; i < outputs.lines; i++) {
		sstream << outputs.shareReported[i] << ",";
		sstream << outputs.leanReported[i] << ",";
		sstream << outputs.projection[i] << ",";
		sstream << outputs.pollLean << "\n";
	}
	
	std::string str = sstream.str();
	return str.substr(0, str.length() - 1);
}
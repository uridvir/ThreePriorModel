#include "ModelBackend.h"
#undef max //Disables annoying max macro

/*
Processes input string (CSV file contents) as model inputs
*/
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
			inputsExt.lines--;
			break;
		}
	}
	return inputsExt;
}

/*
Converts data to mathematically convenient format (normalize % to 1, convert numbers into shares, etc.)
*/
InternalModelInputs getInternals(ExternalModelInputs inputsExt) {
	InternalModelInputs inputsInt = { 0 };

	inputsInt.mailinTotalShare = inputsExt.mailinTurnout / inputsExt.totalTurnout;
	if (inputsInt.mailinTotalShare == 0) { //There may be only 2 priors
		inputsInt.mailinPriorLean = 0.5;
	}
	else {
		inputsInt.mailinPriorLean = (inputsExt.mailinPercentRepublican + inputsExt.mailinPercentIndependent * inputsExt.independentPercentVotesRepublican / 100.0)
			/ (inputsExt.mailinPercentDemocrat + inputsExt.mailinPercentRepublican + inputsExt.mailinPercentIndependent);
	}

	inputsInt.earlyTotalShare = inputsExt.earlyTurnout / inputsExt.totalTurnout;
	if (inputsInt.earlyTotalShare == 0) { //There may be only 2 priors
		inputsInt.earlyPriorLean = 0.5;
	}
	else {
		inputsInt.earlyPriorLean = (inputsExt.earlyPercentRepublican + inputsExt.earlyPercentIndependent * inputsExt.independentPercentVotesRepublican / 100.0)
			/ (inputsExt.earlyPercentDemocrat + inputsExt.earlyPercentRepublican + inputsExt.earlyPercentIndependent);
	}

	inputsInt.pollLean = inputsExt.pollingPercentRepublican / 100.0 + (100.0 - inputsExt.pollingPercentDemocrat - inputsExt.pollingPercentRepublican) / 200.0;
	inputsInt.electiondayTotalShare = 1.0 - inputsInt.mailinTotalShare - inputsInt.earlyTotalShare;
	
	inputsInt.electiondayPriorLean = (inputsInt.pollLean - inputsInt.mailinPriorLean * inputsInt.mailinTotalShare
		- inputsInt.earlyPriorLean * inputsInt.earlyTotalShare) / inputsInt.electiondayTotalShare;

	//Either total turnout is set to zero, or the prior leans are NaN because all recorded voters are neither GOP, Dem, or Ind. (for either mail or early)
	//Since this is impossible if the priors were filled in correctly, throw an error
	//If mail-in or early votes are disabled by setting turnout to 0, this error should not get triggered
	if (isnan(inputsInt.electiondayPriorLean)) {
		throw std::logic_error("Prior inputs cause a divide-by-zero error. Double-check your priors and try again.");
	}

	inputsInt.adjust = inputsExt.adjust;

	inputsInt.lines = inputsExt.lines;
	for (int i = 0; i < inputsInt.lines; i++) {
		if (inputsExt.electiondayRepublicanVotes[i] + inputsExt.electiondayDemocratVotes[i] == 0) {
			inputsInt.electiondayLean.push_back(0.5);
		}
		else {
			inputsInt.electiondayLean.push_back(inputsExt.electiondayRepublicanVotes[i] / (inputsExt.electiondayRepublicanVotes[i] + inputsExt.electiondayDemocratVotes[i]));
		}
		
		//Don't need to check if election day turnout is 0, that would have already triggered an error
		inputsInt.electiondayShare.push_back((inputsExt.electiondayDemocratVotes[i] + inputsExt.electiondayRepublicanVotes[i] + inputsExt.electiondayThirdPartyVotes[i])
			/ (inputsInt.electiondayTotalShare * inputsExt.totalTurnout));

		if (inputsExt.mailinRepublicanVotes[i] + inputsExt.mailinDemocratVotes[i] == 0) {
			inputsInt.mailinLean.push_back(0.5);
		}
		else {
			inputsInt.mailinLean.push_back(inputsExt.mailinRepublicanVotes[i] / (inputsExt.mailinRepublicanVotes[i] + inputsExt.mailinDemocratVotes[i]));
		}

		if (inputsExt.mailinTurnout == 0) {
			inputsInt.mailinShare.push_back(0);
			//This wouldn't cause any model issues, but it's still bad data and the user should fix it
			if (inputsExt.mailinDemocratVotes[i] + inputsExt.mailinRepublicanVotes[i] + inputsExt.mailinThirdPartyVotes[i] != 0) {
				throw std::logic_error("Impossible mail-in voting results. Set mail-in turnout above 0 or mail-in vote tallies to 0 then try again.");
			}
		}
		else {
			inputsInt.mailinShare.push_back((inputsExt.mailinDemocratVotes[i] + inputsExt.mailinRepublicanVotes[i] + inputsExt.mailinThirdPartyVotes[i]) / inputsExt.mailinTurnout);
		}

		if (inputsExt.earlyRepublicanVotes[i] + inputsExt.earlyDemocratVotes[i] == 0) {
			inputsInt.earlyLean.push_back(0.5);
		}
		else {
			inputsInt.earlyLean.push_back(inputsExt.earlyRepublicanVotes[i] / (inputsExt.earlyRepublicanVotes[i] + inputsExt.earlyDemocratVotes[i]));
		}

		if (inputsExt.earlyTurnout == 0) {
			inputsInt.earlyShare.push_back(0);
			//This wouldn't cause any model issues, but it's still bad data and the user should fix it
			if (inputsExt.earlyDemocratVotes[i] + inputsExt.earlyRepublicanVotes[i] + inputsExt.earlyThirdPartyVotes[i] != 0) {
				throw std::logic_error("Impossible early in-person voting results. Set early in-person turnout above 0 or early in-person vote tallies to 0 then try again.");
			}
		}
		else {
			inputsInt.earlyShare.push_back((inputsExt.earlyDemocratVotes[i] + inputsExt.earlyRepublicanVotes[i] + inputsExt.earlyThirdPartyVotes[i]) / inputsExt.earlyTurnout);
		}
	}

	return inputsInt;
}

double getProjection(double priorLean, double lean, double share, double adjust) {
	if (adjust * share == 1) {
		return lean;
	}
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
	outputs.pollLean = inputs.pollLean * 100.0;

	for (int i = 0; i < inputs.lines; i++) {
		outputs.shareReported.push_back((inputs.electiondayShare[i] * inputs.electiondayTotalShare + inputs.mailinShare[i] * inputs.mailinTotalShare
			+ inputs.earlyShare[i] * inputs.earlyTotalShare) * 100.0);
		if (outputs.shareReported[i] == 0) {
			outputs.leanReported.push_back(50);
		}
		else {
			//Multiply by 10000 because shareReported is already normalized to 100
			outputs.leanReported.push_back(10000.0 * (inputs.electiondayLean[i] * inputs.electiondayShare[i] * inputs.electiondayTotalShare
				+ inputs.mailinLean[i] * inputs.mailinShare[i] * inputs.mailinTotalShare
				+ inputs.earlyLean[i] * inputs.earlyShare[i] * inputs.earlyTotalShare) / outputs.shareReported[i]);
		}
		outputs.projection.push_back((
			getProjection(inputs.electiondayPriorLean, inputs.electiondayLean[i], inputs.electiondayShare[i], inputs.adjust) * inputs.electiondayTotalShare +
			getProjection(inputs.mailinPriorLean, inputs.mailinLean[i], inputs.mailinShare[i], inputs.adjust) * inputs.mailinTotalShare +
			getProjection(inputs.earlyPriorLean, inputs.earlyLean[i], inputs.earlyShare[i], inputs.adjust) * inputs.earlyTotalShare) * 100.0
		);
	}

	return outputs;
}

/*
Exports the model outputs to a string (CSV file contents)
*/
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
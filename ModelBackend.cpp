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
		case 3: //Polling % D, Polling % R, Mail-in % D, Mail-in % R, Mail-in % 3, Mail-in turnout
			sstream >> inputsExt.pollingPercentDemocrat;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.pollingPercentRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinPercentDemocrat;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinPercentRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinPercentThirdParty;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.mailinTurnout;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		case 5: // , , Early % D, Early % R, Early % 3, Early turnout
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyPercentDemocrat;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyPercentRepublican;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyPercentThirdParty;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			sstream >> inputsExt.earlyTurnout;
			sstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		case 7: //Adjust constant,	, Defectors % D, Defectors % R,	Ind. % R, Total Turnout
			sstream >> inputsExt.adjustConstant;
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
		default:
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
	}
	return inputsExt;
}
#include <sstream>
#include <string>
#include <fstream>

#include "CSVManager.h"
#include "Constants.h"
#include "PointData.h"

CSVManager * CSVManager::mInstance = nullptr;

CSVManager::CSVManager()
{
}

CSVManager * CSVManager::sharedInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new CSVManager();
	}
	return mInstance;
}

CSVManager::~CSVManager()
{
	if (mInstance)
	{
		delete mInstance;
	}
	mInstance = nullptr;
}

bool CSVManager::loadCSV(std::vector<PointData> & pointVec)
{
	std::string tempStr;
	PointData data = PointData();

	std::ifstream csvFile(Constants::kInputCSVFile);

	if (csvFile.good())
	{
		while (std::getline(csvFile, tempStr, ','))
		{
			data.latitude = std::stod(tempStr);

			std::getline(csvFile, tempStr, ',');
			data.longitude = std::stod(tempStr);

			std::getline(csvFile, tempStr, '\n');
			data.timeStamp = std::stod(tempStr);

			pointVec.push_back(data);
		}
		return true;
	}
	else
	{
		printf("Failed to open CSV File\n");
		return false;
	}
}

bool CSVManager::writeCSV(std::vector<PointData> & pointVec)
{
	std::ofstream outputFile;

	outputFile.open(Constants::kOutputCSVFile);

	if (outputFile.is_open())
	{
		for (PointData data : pointVec)
		{
			if (data.shouldExport == false)
			{
				continue;
			}
			outputFile << data.latitude << ",";
			outputFile << data.longitude << ",";
			outputFile << data.timeStamp << ",";
			outputFile << data.standardDeviation << ",";

			outputFile << "\n";
		}

		outputFile.close();
		return true;
	}
	else
	{
		printf("Unable to write to output file, ensure that it is not open!\n");
		return false;
	}
}
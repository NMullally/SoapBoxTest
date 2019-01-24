#pragma once

#include <vector>

struct PointData;

// Class to handle loading the CSV Files
class CSVManager
{
public:
	// No real need for a singleton, could of just used static functions but just using it for demonstration
	static CSVManager * sharedInstance();

	CSVManager();
	~CSVManager();

	bool writeCSV(std::vector<PointData> & pointVec);
	bool loadCSV(std::vector<PointData> & pointVec);

private:
	static CSVManager * mInstance;
};


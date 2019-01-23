#pragma once

#include <vector>

struct PointData;

class CSVManager
{
public:
	static CSVManager * sharedInstance();

	CSVManager();
	~CSVManager();

	void writeCSV(std::vector<PointData> & pointVec);
	void loadCSV(std::vector<PointData> & pointVec);

private:
	static CSVManager * mInstance;
};


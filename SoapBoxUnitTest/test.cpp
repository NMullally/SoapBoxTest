#include "pch.h"
#include "../SoapBoxTest/CSVManager.h"
#include "../SoapBoxTest/FilterData.h"
#include "../SoapBoxTest/Constants.h"
#include "../SoapBoxTest/PointData.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(SampleSize, Non_Zero)
{
	EXPECT_FALSE(Constants::kSampleSize == 0);
}

TEST(InputFile, Not_Empty)
{
	EXPECT_FALSE(Constants::kInputCSVFile == "");
}

TEST(InputFile, Exists)
{
	std::ifstream csvFile(Constants::kInputCSVFile);

	EXPECT_TRUE(csvFile.good());
}

void loadValues(std::vector<PointData> &pointVec)
{
	PointData data = PointData();

	std::string tempString;
	std::ifstream csvFile(Constants::kInputCSVFile);

	if (csvFile.good())
	{
		while (std::getline(csvFile, tempString, ','))
		{
			data.latitude = std::stod(tempString);

			std::getline(csvFile, tempString, ',');
			data.longitude = std::stod(tempString);

			std::getline(csvFile, tempString, '\n');
			data.timeStamp = std::stod(tempString);

			pointVec.push_back(data);
		}
	}
}

TEST(InputFile, Valid_Values)
{
	std::vector<PointData> points = std::vector<PointData>();

	loadValues(points);

	for (PointData &point : points)
	{
		EXPECT_FALSE(point.latitude == 0);
		EXPECT_FALSE(point.longitude == 0);
		EXPECT_FALSE(point.timeStamp == 0);
	}
}

TEST(InputFile, Contains_Points)
{
	std::vector<PointData> points = std::vector<PointData>();

	loadValues(points);

	EXPECT_FALSE(points.size() == 0);
}
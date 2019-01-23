// SoapBoxTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


// Goal 
/*
Developer Code Test:
Develop an algorithm that, given a series of data points (latitude,longitude,timestamp) for a car journey from A->B, 
which will disregard potentially erroneous points (present in the dataset).

Try to demonstrate a knowledge of Object Oriented concepts in your answer. The solution should be completed using C++. 
The attached file contains the data points to check. Include unit test(s) to ensure that your code works as expected.
*/


// Small goals
/*
	Setup project
	Create algorimth with 5-6 data points 
	Read in CSV file
	Compare results with plotted data from Excel Sheet

*/

/*
51.52322643	-0.151978571	1326380114
51.52329041	-0.151610456	1326380117
51.5233599	-0.150170377	1326380129
51.52829021	-0.181102753	1326380144   // y value incorrect
51.52380974	-0.146383702	1326380159
51.51037158	-0.149173737	1326380169
51.52386074	-0.14610651	1326380171
51.52389856	-0.14596123	1326380190
 
*/

// Simple data structure for each point
struct DataPoint
{
	static int totalSize;

	int index;
	double longitude;
	double latitude;
	double timeStamp;

	DataPoint()
	{
		this->index = totalSize++;

	}

	DataPoint(float longitude, float latitude, double timeStamp)
	{
		this->index = totalSize++;
		this->longitude = longitude;
		this->latitude = latitude;
		this->timeStamp = timeStamp;
	}
};

int DataPoint::totalSize = 0;

// Has to be error tollerence
// 5% tolerence for x and y

const float kErrorRangeX = 0.01; 
const float kErrorRangeY = 0.01;

std::vector<DataPoint> mDataPoints;

void setupDataPoints()
{
	mDataPoints = std::vector<DataPoint>();

	mDataPoints = {
		DataPoint(51.52322643,	-0.151978571,	1326380114),
		DataPoint(51.52329041,	-0.151610456,	1326380117),
		DataPoint(51.5233599,	-0.150170377,	1326380129),
	//	DataPoint(51.52829021,	-0.181102753,	1326380144), //errored value
		DataPoint(51.52380974,	-0.146383702,	1326380159),
		DataPoint(51.51037158,	-0.149173737,	1326380169),
		DataPoint(51.52386074,	-0.14610651,	1326380171),
		DataPoint(51.52389856,	-0.14596123,	1326380190)
	};
}

const int kSampleSize = 4;


void calculateStandardDeviation()
{
	float xSum = 0;
	float ySum = 0;

	for (int i = 0; i < mDataPoints.size(); i++)
	{
		xSum += mDataPoints[i].longitude;
		ySum += mDataPoints[i].latitude;
	}

	float xMean = xSum / mDataPoints.size();
	float yMean = ySum / mDataPoints.size();

	float xVariance = 0;
	float yVariance = 0;

	for (int i = 0; i < mDataPoints.size(); i++)
	{
		xVariance += pow(mDataPoints[i].longitude - xMean, 2);
		yVariance += pow(mDataPoints[i].latitude - yMean, 2);
	}

	xVariance = xVariance / (mDataPoints.size() - 1);
	yVariance = yVariance / (mDataPoints.size() - 1);

	float xSD = std::sqrt(xVariance);
	float ySD = std::sqrt(yVariance);

}

void badAvergeCalculation2()
{
	int size = mDataPoints.size();
	int index = 0;

	for (int i = kSampleSize; i < mDataPoints.size(); )
	{
		DataPoint currentPoint = mDataPoints[i];

		auto iter = mDataPoints.begin();

		float sumX = 0;
		float sumY = 0;

		for (int x = i - kSampleSize; x < i; x++)
		{
			// we only want to sample points around a certain point
			if (x == i)
			{
				continue;
			}

			sumX += mDataPoints[x].longitude;
			sumY += mDataPoints[x].latitude;
		}


		float currX = mDataPoints[i].longitude;
		float currY = mDataPoints[i].latitude;

		int size = kSampleSize - 1;

		float xAvg = sumX / size;
		float yAvg = sumY / size;

		float absX = std::abs(currX - xAvg);
		float absY = std::abs(currY - yAvg);


		float xSD = std::sqrt(std::pow(std::abs(currX - sumX), 2) / size);
		float ySD = std::sqrt(std::pow(std::abs(currY - sumY), 2) / size);

		float totalDeviation = std::sqrt((std::pow(std::abs(currY - sumY), 2) + std::pow(std::abs(currX - sumX), 2)) / size);


		printf("x::%f, xAvg::%f, xAbs::%f \n", currX, xAvg, absX);
		printf("y::%f, yAvg::%f, yAbs::%f \n", currY, yAvg, absY);

		if ((absX > kErrorRangeY) || (absY > kErrorRangeY))
		{
			int index = currentPoint.index;
			printf("X Value is off %d \n", i);
			mDataPoints.erase(mDataPoints.begin() + i); // remove current point so it doesnt effect the avgs of the next ones.
		}
		else
		{
			i++;
		}

		printf("***************************** \n");
	}
}

void loadCSV()
{
	const char * csvFileName = "something.csv";

	DataPoint data = DataPoint();
	std::string str;

	std::ifstream csvFile(csvFileName);

	while (std::getline(csvFile, str, ','))
	{
		data.latitude = std::stod(str);

		std::getline(csvFile, str, ',');
		data.longitude = std::stod(str);

		std::getline(csvFile, str, ',');
		data.timeStamp = std::stod(str);

		mDataPoints.push_back(data);
	}
}

int main()
{
	std::cout << "Hello World!\n";

	setupDataPoints();

//	badAvergeCalculation2();

	simpleStandardDeviation();
}
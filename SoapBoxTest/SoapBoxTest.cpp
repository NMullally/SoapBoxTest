// SoapBoxTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <numeric>

#include "PointData.h"
#include "CSVManager.h"
#include "Constants.h"

/*
Developer Code Test:
Develop an algorithm that, given a series of data points (latitude,longitude,timestamp) for a car journey from A->B, 
which will disregard potentially erroneous points (present in the dataset).

Try to demonstrate a knowledge of Object Oriented concepts in your answer. The solution should be completed using C++. 
The attached file contains the data points to check. Include unit test(s) to ensure that your code works as expected.
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

std::vector<PointData> mPoints;

struct StandardDeviations
{
	double latitude = 0;
	double longitude = 0;
	double combined = 0;

	StandardDeviations() {}

	StandardDeviations(double latitude, double longitude, double combined)
	{
		this->latitude = latitude;
		this->longitude = longitude;
		this->combined = combined;
	}
};

void setupPointDatas()
{
	mPoints = std::vector<PointData>();

	CSVManager::sharedInstance()->loadCSV(mPoints);

	/*
	mPointDatas = {
		PointData(51.52322643,	-0.151978571,	1326380114),
		PointData(51.52329041,	-0.151610456,	1326380117),
		PointData(51.5233599,	-0.150170377,	1326380129),
	//	PointData(51.52829021,	-0.181102753,	1326380144), //errored value
		PointData(51.52380974,	-0.146383702,	1326380159),
		PointData(51.51037158,	-0.149173737,	1326380169),
		PointData(51.52386074,	-0.14610651,	1326380171),
		PointData(51.52389856,	-0.14596123,	1326380190)
	};
	*/
}

StandardDeviations calculateStandardDeviation(int currentIndex, int startIndex, int endIndex)
{
	startIndex = startIndex < 0 ? 0 : startIndex;
	endIndex = endIndex >= mPoints.size() ? mPoints.size() - 1 : endIndex;

	double latitudeSum = 0;
	double longitudeSum = 0;
	int totalSampleSize = Constants::kSampleSize * 2;
	totalSampleSize += 1;

	for (int i = startIndex; i <= endIndex; ++i)
	{
		latitudeSum += mPoints[i].latitude;
		longitudeSum += mPoints[i].longitude;
	}

	double latitudeMean = latitudeSum / totalSampleSize;
	double longitudeMean = longitudeSum / totalSampleSize;

	double latitudeVariance = 0;
	double longitudeVariance = 0;
	double totalVariance = 0;	

	latitudeVariance	= std::pow(mPoints[currentIndex].latitude - latitudeMean, 2);
	longitudeVariance	= std::pow(mPoints[currentIndex].longitude - longitudeMean, 2);
	totalVariance		= latitudeVariance + longitudeVariance;

	// because were only doing a small portion its N - 1 
	latitudeVariance	= latitudeVariance / (totalSampleSize - 1);
	longitudeVariance	= longitudeVariance / (totalSampleSize - 1);
	totalVariance		= totalVariance / (totalSampleSize - 1);

	double latitudeStandardDeviation	= std::sqrt(latitudeVariance);
	double longitudeStandardDeviation	= std::sqrt(longitudeVariance);
	double totalStandardDeviation		= std::sqrt(totalVariance);

	return StandardDeviations(latitudeStandardDeviation, longitudeStandardDeviation, totalStandardDeviation);
}

void calculateBeforeAndAfter()
{
	StandardDeviations currentStandardDev = StandardDeviations();
	StandardDeviations previousStandardDev = StandardDeviations();

	for (int i = 0; i < mPoints.size(); ++i)
	{
		currentStandardDev = calculateStandardDeviation(i, i - Constants::kSampleSize, i + Constants::kSampleSize);

		if (previousStandardDev.latitude != 0 && previousStandardDev.longitude != 0)
		{
			double latitudeDiff		= std::abs(currentStandardDev.latitude - previousStandardDev.latitude);
			double longitudeDiff	= std::abs(currentStandardDev.longitude - previousStandardDev.longitude);
			double totalDiff		= std::abs(currentStandardDev.combined - previousStandardDev.combined);

			printf("** xDiff :: %f, yDiff :: %f, totalDeviation :: %f\n", latitudeDiff, longitudeDiff, totalDiff);

			if (latitudeDiff > Constants::kMaxStandardDeviationX || longitudeDiff > Constants::kMaxStandardDeviationY)
			{
				mPoints[i].shouldExport = false;
				mPoints[i].standardDeviation = totalDiff;
			}
		}

		previousStandardDev = currentStandardDev;
	}
}

void workingFxn()
{
	struct PreviousIteration
	{
		double latitudeSD = 0;
		double longitudeSD = 0;
		double totalSD = 0;
	};

	PreviousIteration previousIteration = PreviousIteration();

	for (int i = 0; i < mPoints.size(); ++i)
	{
		double sumX = 0;
		double sumY = 0;

		int startIndex = (i - Constants::kSampleSize) < 0 ? 0 : i - Constants::kSampleSize;
		int endIndex = (i + Constants::kSampleSize) < mPoints.size() ? (i + Constants::kSampleSize) : mPoints.size();

		for (int index = startIndex; index < endIndex; ++index)
		{
			// we only want to sample points around a certain point
			if (index == i)
			{
				continue;
			}

			sumX += mPoints[index].latitude;
			sumY += mPoints[index].longitude;
		}

		double currX = mPoints[i].latitude;
		double currY = mPoints[i].longitude;

		int size = Constants::kSampleSize * 2;

		double xMean = sumX / size;
		double yMean = sumY / size;

		double absX = std::abs(currX - xMean);
		double absY = std::abs(currY - yMean);

		double xSD = std::sqrt(std::pow(std::abs(currX - sumX), 2) / (size - 1));
		double ySD = std::sqrt(std::pow(std::abs(currY - sumY), 2) / (size - 1));

		double totalDeviation = std::sqrt((std::pow(std::abs(currY - sumY), 2) + std::pow(std::abs(currX - sumX), 2)) / (size - 1));

		if (previousIteration.latitudeSD != 0 && previousIteration.longitudeSD != 0)
		{
			double totalDiff = abs(totalDeviation - previousIteration.totalSD);
			double xDiff = abs(xSD - previousIteration.latitudeSD);
			double yDiff = abs(ySD - previousIteration.longitudeSD);

			printf("** xDiff :: %f, yDiff :: %f, totalDeviation :: %f\n", xDiff, yDiff, totalDiff);

			if (yDiff > Constants::kMaxStandardDeviationX || xDiff > Constants::kMaxStandardDeviationY)
			{
				mPoints[i].shouldExport = false;
			}
		}

		mPoints[i].standardDeviation = totalDeviation;
		previousIteration.latitudeSD = xSD;
		previousIteration.longitudeSD = ySD;
		previousIteration.totalSD = totalDeviation;
	}
}

int main()
{
	std::cout << "Hello World!\n";

	setupPointDatas();

	calculateBeforeAndAfter();

	CSVManager::sharedInstance()->writeCSV(mPoints);
}
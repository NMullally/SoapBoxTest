// SoapBoxTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <numeric>

#include "PointData.h"
#include "CSVManager.h"
#include "Constants.h"

#define DEBUG_ENABLED 1

/*
Developer Code Test:
Develop an algorithm that, given a series of data points (latitude,longitude,timestamp) for a car journey from A->B, 
which will disregard potentially erroneous points (present in the dataset).

Try to demonstrate a knowledge of Object Oriented concepts in your answer. The solution should be completed using C++. 
The attached file contains the data points to check. Include unit test(s) to ensure that your code works as expected.
*/

std::vector<PointData> mPoints;
int totalNumPointsRemoved = 0;

// Simple struct to hold standard Deviation values.
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

/*
	Given a sample size (aka 2) it will iterator 2 before and after the current point, get a average deviation which
	is then compared with the previous deviation, if the gap is large enough its discarded. 

	Sample Standard Deviation formula is used below.

	In this function im calculating the X,Y and total deviation, which is probably not useful but potentially could be used instead of just checking
	the X and Y
*/
StandardDeviations calculateStandardDeviation(int currentIndex, int startIndex, int endIndex)
{
	startIndex = startIndex < 0 ? 0 : startIndex;
	endIndex = endIndex >= mPoints.size() ? mPoints.size() - 1 : endIndex;

	double latitudeSum = 0;
	double longitudeSum = 0;
	int totalSampleSize = endIndex - startIndex;
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

	// because were only doing small portions its N - 1 
	latitudeVariance	= latitudeVariance / (totalSampleSize - 1);
	longitudeVariance	= longitudeVariance / (totalSampleSize - 1);
	totalVariance		= totalVariance / (totalSampleSize - 1);

	double latitudeStandardDeviation	= std::sqrt(latitudeVariance);
	double longitudeStandardDeviation	= std::sqrt(longitudeVariance);
	double totalStandardDeviation		= std::sqrt(totalVariance);

	return StandardDeviations(latitudeStandardDeviation, longitudeStandardDeviation, totalStandardDeviation);
}

bool filterByRollingAverage()
{
	int numPointsRemoved = 0;

	StandardDeviations currentStandardDev = StandardDeviations();
	StandardDeviations previousStandardDev = StandardDeviations();

	for (int i = 0; i < mPoints.size(); )
	{
		currentStandardDev = calculateStandardDeviation(i, i - Constants::kSampleSize, i + Constants::kSampleSize);

		// First value is ignored since it has no previous values to use.
		if (previousStandardDev.latitude != 0 && previousStandardDev.longitude != 0)
		{
			double latitudeDiff		= std::abs(currentStandardDev.latitude - previousStandardDev.latitude);
			double longitudeDiff	= std::abs(currentStandardDev.longitude - previousStandardDev.longitude);
			double totalDiff		= std::abs(currentStandardDev.combined - previousStandardDev.combined);

#if DEBUG_ENABLED
			printf("** xDiff :: %f, yDiff :: %f, totalDeviation :: %f\n", latitudeDiff, longitudeDiff, totalDiff);
#endif
			// Checks if the lat or longitude deviation large enough, then it will be discarded.
			if (latitudeDiff > Constants::kMaxStandardDeviationX || longitudeDiff > Constants::kMaxStandardDeviationY)
			{
				numPointsRemoved++;
				mPoints[i].standardDeviation = totalDiff;
				mPoints.erase(mPoints.begin() + i);
			}
			else
			{
				++i;
			}
		}
		else
		{
			++i;
		}

		previousStandardDev = currentStandardDev;
	}
	totalNumPointsRemoved += numPointsRemoved;
	printf("%d Points removed from the data this iteration!\n", numPointsRemoved);

	return numPointsRemoved > 0;
}

// Can obtain slightly better data if we filter the data again after removing some errorous points.
void filterData(bool doMuliplePasses)
{
	if (doMuliplePasses)
	{
		// ensure that it doesnt become an infinite loop.
		int count = 0;
		while (filterByRollingAverage() == true && count < Constants::kMaxLoopAmount)
		{
			count++;
		}
	}
	else
	{
		filterByRollingAverage();
	}
}

int main()
{
	mPoints = std::vector<PointData>();

	if (CSVManager::sharedInstance()->loadCSV(mPoints) == false)
	{
		exit(EXIT_FAILURE);
	}
	
	filterData(Constants::kShouldDoMuliplePasses);

	printf("%d Points removed from the data total!\n", totalNumPointsRemoved);

	if (CSVManager::sharedInstance()->writeCSV(mPoints) == false)
	{
		return EXIT_FAILURE;
	}

	printf("\n** Graph Smoothing complete **\n");
}
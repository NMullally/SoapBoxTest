// SoapBoxTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <numeric>

#include "PointData.h"
#include "CSVManager.h"
#include "Constants.h"
#include "FilterData.h"

/*
Developer Code Test:
Develop an algorithm that, given a series of data points (latitude,longitude,timestamp) for a car journey from A->B, 
which will disregard potentially erroneous points (present in the dataset).

Try to demonstrate a knowledge of Object Oriented concepts in your answer. The solution should be completed using C++. 
The attached file contains the data points to check. Include unit test(s) to ensure that your code works as expected.
*/

int main()
{
	FilterData * filterData = new FilterData();

	if (CSVManager::sharedInstance()->loadCSV(filterData->mPoints) == false)
	{
		exit(EXIT_FAILURE);
	}
	
	filterData->runFilterAlgorithm(Constants::kShouldDoMuliplePasses);

	printf("%d Points removed from the data total!\n", filterData->getTotalPointsRemoved());

	if (CSVManager::sharedInstance()->writeCSV(filterData->mPoints) == false)
	{
		return EXIT_FAILURE;
	}

	printf("\n** Graph Smoothing complete **\n");
}
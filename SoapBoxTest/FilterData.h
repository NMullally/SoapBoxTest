#pragma once

#include <vector>

struct PointData;
struct StandardDeviations;


class FilterData
{
public:
	FilterData();
	~FilterData();

	int getTotalPointsRemoved();
	bool filterByRollingAverage();
	void runFilterAlgorithm(bool doMuliplePasses);

	StandardDeviations calculateStandardDeviation(int currentIndex, int startIndex, int endIndex);

	std::vector<PointData> mPoints;

private:
	int totalNumPointsRemoved = 0;
};


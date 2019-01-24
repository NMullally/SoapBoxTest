#pragma once

// Not sure on companys standards, but I like to keep configurable variables in one location if it makes sense
// that way theres less hassle trying to find certain values to modify.

#define DEBUG_ENABLED 1

namespace Constants
{
	const char * const kInputCSVFile	= "../Data/data_points.csv";
	const char * const kOutputCSVFile	= "../Data/output.csv";

	const float kMaxStandardDeviationX = 0.001f;
	const float kMaxStandardDeviationY = 0.001f;

	const int kSampleSize = 2;
	const int kMaxLoopAmount = 5;
	const bool kShouldDoMuliplePasses = true;
}

#pragma once

// Not sure on companys standards, but I like to keep configurable variables in one location if it makes sense
// that way theres less hassle trying to find certain values to modify.
namespace Constants
{
	const char * const kInputCSVFile	= "../Data/data_points.csv";
	const char * const kOutputCSVFile = "../Data/output.csv";

	const float kMaxStandardDeviationX = 0.0005f;
	const float kMaxStandardDeviationY = 0.0005f;

	// Will check two values before and after the current value.
	const int kSampleSize = 2;
}
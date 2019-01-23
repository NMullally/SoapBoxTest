#pragma once

// Simple structure to store data from CSV and for writing to a CSV

struct PointData
{
	// Flag on either the point should be exported or ignored.
	bool shouldExport;
	// Doubles rather than floats for precision.
	double latitude;
	double longitude;
	double timeStamp;

	// Could be useful to plot the SD for each point
	double standardDeviation;
};


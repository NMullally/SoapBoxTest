#pragma once

// Simple structure to store data from CSV and for writing to a CSV

struct PointData
{
	// Flag on either the point should be exported or ignored.
	bool shouldExport = true;
	// Doubles rather than floats for precision.
	double latitude	 = 0.0;
	double longitude = 0.0;
	double timeStamp = 0.0;

	// Could be useful to plot the SD for each point
	double standardDeviation = 0.0;

	PointData() {}
	PointData(double lat, double longitude, double timeStamp)
	{
		this->latitude	= lat;
		this->longitude = longitude;
		this->timeStamp = timeStamp;
	}
};


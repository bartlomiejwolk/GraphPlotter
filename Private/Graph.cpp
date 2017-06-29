/*
Copyright 2017 Bartlomiej Wolk (metroidsnes)
Licensed under MIT license. See the LICENSE file in the module root folder.
*/

#include "GraphPlotterModule.h"
#include "Graph.h"

const FColor FGp_Color::BgGreen(0, 99, 0, 63);
const FColor FGp_Color::BgYellow(255, 99, 0, 63);
const FColor FGp_Color::ReferenceLineRed(255, 99, 0, 127);
const FColor FGp_Color::GraphLine(241, 196, 15, 1);
const FColor FGp_Color::GraphText(255, 255, 255, 200);

FGp_Range::FGp_Range()
{
}

FGp_Range::FGp_Range(float min, float max) : Min(min), Max(max)
{
}

FGp_Graph::FGp_Graph()
{
}

FGp_Graph::FGp_Graph(int32 sampleSize) : SampleSize(sampleSize)
{
}

FGp_Graph::~FGp_Graph()
{
}

void FGp_Graph::AddDataPoint(float data)
{
	// cache
	RawDataPoint = data;

	// normalize
	float offsettedDataPoint = OffsetDataPoint(RawDataPoint);
	float normalizedDataPoint = NormalizeDataPoint(offsettedDataPoint);

	// store
	if (NormalizedDataPoints.Num() >= SampleSize)
	{
		NormalizedDataPoints.RemoveAt(0);
	}
	NormalizedDataPoints.Add(normalizedDataPoint);
}

float FGp_Graph::GetLastRawDataValue() const
{
	return RawDataPoint;
}

const TArray<float>& FGp_Graph::GetNormalizedData() const
{
	return NormalizedDataPoints;
}

/*
This must be calculated for each method call because the raw position value
could change at runtime (e.g. to indicate lowest value plotted on the graph).
*/
float FGp_Graph::GetNormalizedReferenceLineValue() const
{
	float offsettedPosValue = OffsetDataPoint(ReferenceLineConfig.PositionValue);
	float normalizedPosValue = NormalizeDataPoint(offsettedPosValue);

	return normalizedPosValue;
}

float FGp_Graph::GetRangeDelta() const
{
	float delta = FMath::Abs(Range.Max - Range.Min);
	return delta;
}

float FGp_Graph::OffsetDataPoint(float dataPoint) const
{
	float result = 0;
	if (Range.Min < 0)
	{
		result = dataPoint + FMath::Abs(Range.Min);
	}
	else if (Range.Min > 0)
	{
		result = dataPoint - Range.Min;
	}
	// zero
	else
	{
		return dataPoint;
	}
	return result;
}

float FGp_Graph::NormalizeDataPoint(float dataPoint) const
{
	float result;
	float delta = GetRangeDelta();
	result = dataPoint / delta;
	result = FMath::Clamp(result, 0.f, 1.f);
	return result;
}


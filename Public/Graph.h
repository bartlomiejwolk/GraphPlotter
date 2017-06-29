/* 
Copyright 2017 Bartlomiej Wolk (metroidsnes)
Licensed under MIT license. See the LICENSE file in the module root folder.
*/

#pragma once

#include "CoreMinimal.h"

/*
Delegate used to determine graph background color at runtime.
*/
DECLARE_DELEGATE_RetVal_OneParam(FColor, FGx_BgColorDelegate, float)

/*
Color constants for user convenience.
*/
static struct GRAPHPLOTTERMODULE_API FGp_Color
{
	static const FColor BgGreen;
	static const FColor BgYellow;
	static const FColor ReferenceLineRed;
	static const FColor GraphLine;
	static const FColor GraphText;
};

/*
Config variables related to the reference line.
Reference line is a fixed horizontal line drawn on the graph as a reference.
*/
struct FGp_ReferenceLineConfig
{
	/*
	If true, the line will be draw on the graph.
	*/
	bool Enabled = false;

	/*
	Raw value at which the line will be drawn.
	E.g. if your graph displays FPS ranging from <0; 60> then you can set this
	value to 30 to see when the FPS goes below 30.
	*/
	float PositionValue = 0;

	/*
	Color of the line.
	*/
	FColor Color = FGp_Color::ReferenceLineRed;
};

/*
Defines a range of max and min values.
*/
struct GRAPHPLOTTERMODULE_API FGp_Range
{
	FGp_Range();
	FGp_Range(float min, float max);

	float Max = 10.f;
	float Min = -10.f;
};

/*
Represents a debug graph.
Contains debug data and settings that define graph properties.
Only user can change its state. `FGp_GraphPlotter` has read-only access.
Properties not initialized by the constructor can be changed at runtime.

Usage:
Create instance of this class as a class field and initialize its properties
in the constructor.
Each frame call `AddDataPoint()` to add new data point to the graph.
Use `FGp_GraphPlotter::Plot()` to draw its content onto the screen.
*/
class GRAPHPLOTTERMODULE_API FGp_Graph
{
public:
	FGp_Graph();

	/*
	sampleSize Number of data points to buffer and display on the screen.
	*/
	FGp_Graph(int32 sampleSize);
	
	~FGp_Graph();

	/* 
	World position of the graph on the screen in pixels.
	Since graph include header, it's position of the header.
	Header contains title and the current debug value.
	*/
	FVector2D Position = FVector2D(20.f, 100.f);

	/*
	Size of the graph in pixels.
	*/
	FVector2D Size = FVector2D(100.f, 100.f);

	/*
	Graph background color.
	*/
	FColor BgColor = FGp_Color::BgGreen;

	/* 
	Graph title, displayed above the plotting area.
	*/
	FString Title = FString("unnamed");

	/*
	Defines a range of max and min values that can be displayed on the graph.
	E.g. for FPS it can be <0; 60>.
	*/
	FGp_Range Range;

	/* 
	Function called before drawing graph background.
	Bind a function or lambda to change graph background color at runtime.
	
	E.g.:
	MyGraph.BgColorDelegateInstance.BindLambda([](float fps)
	{
	return fps > 30 ? FColor::BgGreed : FColor::BgYellow;
	});
	*/
	FGx_BgColorDelegate BgColorDelegateInstance;

	/*
	Config for the reference line.
	Access it directly to set reference line properties.
	By default, reference line will not be displayed.
	*/
	FGp_ReferenceLineConfig ReferenceLineConfig;

	/*
	Adds new data point to the graph.
	Total number of stored points is limited by SampleSize.
	*/
	void AddDataPoint(float data);

	/*
	Returns last added raw data point.
	*/
	float GetLastRawDataValue() const;

	/*
	Returns all buffered data points normalized to range between <0; 1>.
	*/
	const TArray<float>& GetNormalizedData() const;

	/*
	Returns reference line value normalized to range between <0; 1>.
	*/
	float GetNormalizedReferenceLineValue() const;

private:
	/*
	Last added raw data point added by the user.
	*/
	float RawDataPoint;

	/*
	Stores normalized data points for plotting.
	Each point must be in range between <0; 1>.
	Total amount of stored points is limited by `SampleSize`.
	*/
	TArray<float> NormalizedDataPoints;

	/*
	Max amount of data points that can be buffered.
	*/
	const int32 SampleSize = 100;

	/*
	Returns absolute difference between min and max in the `Range`.
	Used to normalize data points.
	*/
	float GetRangeDelta() const;

	/*
	Offsets data point by adding a fixed value.
	All data points are offsetted to avoid negative values.
	*/
	float OffsetDataPoint(float dataPoint) const;

	/*
	Represents data point as a number between <0; 1>.
	*/
	float NormalizeDataPoint(float dataPoint) const;

};

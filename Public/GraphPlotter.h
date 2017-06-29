/*
Copyright 2017 Bartlomiej Wolk (metroidsnes)
Licensed under MIT license. See the LICENSE file in the module root folder.
*/

#pragma once

#include "CoreMinimal.h"
#include "Graph.h"

/* 
Draws debug graphs on the screen.
Requires instances of `FGp_Graph` and `UCanvas`.

Usage:
Create instance of `FGp_GraphPlotter` as a class field.
Every frame call `Plot(UCanvas, FGp_Graph)`.

Graph consists of a header (title and current debug value) and the plotting area.

<Title>
<Current debug value>
---------------------
+99				  00-
-					-
-		x			-
-	x		x	   x-
-x				x	-
-					-
-99					-
---------------------
 */
class GRAPHPLOTTERMODULE_API FGp_GraphPlotter
{
public:
	FGp_GraphPlotter();
	~FGp_GraphPlotter();

	/*
	Value in pixels used to offset text within the graph from the edges of the 
	graph background.
	*/
	const int32 TextOffsetFromGraphEdge = 2;

	/*
	Draws graph to the screen.
	*/
	void Plot(UCanvas* canvas, const FGp_Graph& graph);

private:
	/*
	Line spacing in pixels. Used in header.
	*/
	const int32 LineSpacing = 2;

	/*
	Canvas used to draw all graphs and texts.
	Initialized in `Plot()` with `UCanvas` instance passed by the user.
	*/
	UCanvas* Canvas = nullptr;

	/*
	Font used to draw all texts. Initialized in `Plot()`.
	*/
	UFont* Font = nullptr;

	/*
	Used when drawing text to define text properties (e.g. shadowing).
	Initialized in `Plot()`.
	*/
	FFontRenderInfo FontRendererInfo;

	/*
	Draws graph title.
	*/
	void DrawTitle(const FGp_Graph &graph) const;

	/*
	Draws raw value of the last data point stored in the graph.
	*/
	void DrawCurrentDebugValue(const FGp_Graph &graph, float textHeight) const;

	/*
	Draws graph background.
	*/
	void CreateBgTile(const FGp_Graph &graph, float plottingAreaWorldPosY) const;

	/*
	Draws maximum value of the graph range in the top-left corner of the graph.
	*/
	void DrawMaxValueText(const FGp_Graph &graph, float plottingAreaWorldPosY) const;

	/*
	Draws minimum value of the graph range in the bottom-left corner of the graph.
	*/
	void DrawMinValueText(const FGp_Graph &graph, float plottingAreaWorldPosY) const;

	/*
	Draws reference line to the screen.
	*/
	void DrawReferenceLine(const FGp_Graph &graph, float plottingAreaWorldPosY) const;

	/*
	Draws reference line value in the top-right corner of the graph.
	*/
	void DrawReferenceLineValueText(const FGp_Graph &graph, float plottingAreaWorldPosY) const;

	/*
	Draws data points. 
	Other graph elements like background, header and all texts are drawn separately.
	*/
	void PlotGraph(const FGp_Graph &graph, float plottingAreaWorldPosY) const;

};

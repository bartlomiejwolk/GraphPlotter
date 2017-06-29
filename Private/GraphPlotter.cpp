/*
Copyright 2017 Bartlomiej Wolk (metroidsnes)
Licensed under MIT license. See the LICENSE file in the module root folder.
*/

#include "GraphPlotterModule.h"
#include "GraphPlotter.h"
#include "CanvasItem.h"
#include "Utils.h"

FGp_GraphPlotter::FGp_GraphPlotter()
{
}

FGp_GraphPlotter::~FGp_GraphPlotter()
{
}

void FGp_GraphPlotter::Plot(UCanvas* canvas, const FGp_Graph& graph)
{
	if (!canvas)
	{
		return;
	}

	// init class fields
	Canvas = canvas;
	FontRendererInfo = Canvas->CreateFontRenderInfo(false, true);
	Font = GEngine->GetMediumFont();

	DrawTitle(graph);

	// get title text size
	float textWidth, textHeight;
	Canvas->TextSize(Font, graph.Title, textWidth, textHeight);

	// set drawing color
	Canvas->SetDrawColor(FColor::White);

	DrawCurrentDebugValue(graph, textHeight);

	// calculate Y pos of the plotting area
	float plottingAreaWorldPosY = graph.Position.Y + 2 * textHeight + LineSpacing;

	CreateBgTile(graph, plottingAreaWorldPosY);

	Canvas->SetDrawColor(FGp_Color::GraphText);

	DrawMaxValueText(graph, plottingAreaWorldPosY);
	DrawMinValueText(graph, plottingAreaWorldPosY);

	DrawReferenceLine(graph, plottingAreaWorldPosY);
	DrawReferenceLineValueText(graph, plottingAreaWorldPosY);

	PlotGraph(graph, plottingAreaWorldPosY);
}

void FGp_GraphPlotter::DrawTitle(const FGp_Graph &graph) const
{
	Canvas->SetDrawColor(FColor(255, 255, 0));
	Canvas->DrawText(
		Font,
		graph.Title,
		graph.Position.X,
		graph.Position.Y,
		1.f,
		1.f,
		FontRendererInfo);
}

void FGp_GraphPlotter::DrawCurrentDebugValue(const FGp_Graph &graph, float textHeight) const
{
	// calc. vert. world pos.
	float offsetY = textHeight + LineSpacing;
	float posY = graph.Position.Y + offsetY;

	// convert debug value to string
	float lastDebugValue = graph.GetLastRawDataValue();
	FString valueText = FString::SanitizeFloat(lastDebugValue);

	Canvas->DrawText(
		Font,
		valueText,
		graph.Position.X,
		posY,
		1.f,
		1.f,
		FontRendererInfo);
}

void FGp_GraphPlotter::CreateBgTile(const FGp_Graph &graph, float plottingAreaWorldPosY) const
{
	// get bg color from delegate
	FColor bgColor = graph.BgColor;
	FGx_BgColorDelegate del = graph.BgColorDelegateInstance;
	if (del.IsBound())
	{
		float currentDebugValue = graph.GetLastRawDataValue();
		bgColor = del.Execute(currentDebugValue);
	}

	// draw tile
	FVector2D pos(graph.Position.X, plottingAreaWorldPosY);
	FCanvasTileItem tileItem(
		pos,
		graph.Size,
		bgColor);
	tileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(tileItem);
}

void FGp_GraphPlotter::DrawMaxValueText(const FGp_Graph &graph, float plottingAreaWorldPosY) const
{
	FText valueText = FGp_Utils::GetFloatAsStringWithPrecision(graph.Range.Max, 0);
	float worldPosX = graph.Position.X + TextOffsetFromGraphEdge;
	float worldPosY = plottingAreaWorldPosY + LineSpacing;
	Canvas->DrawText(
		Font,
		valueText,
		worldPosX,
		worldPosY);
}


void FGp_GraphPlotter::DrawMinValueText(const FGp_Graph &graph, float plottingAreaWorldPosY) const
{
	FText valueText = FGp_Utils::GetFloatAsStringWithPrecision(graph.Range.Min, 0);

	// get text size
	float textWidth, textHeight;
	Canvas->TextSize(Font, graph.Title, textWidth, textHeight);

	float worldPosX = graph.Position.X + TextOffsetFromGraphEdge;
	float worldPosY = plottingAreaWorldPosY + (graph.Size.Y - textHeight - LineSpacing);
	Canvas->DrawText(
		Font,
		valueText,
		worldPosX,
		worldPosY);
}

void FGp_GraphPlotter::DrawReferenceLine(const FGp_Graph &graph, float plottingAreaWorldPosY) const
{
	if (!graph.ReferenceLineConfig.Enabled)
	{
		return;
	}

	// line position in plotting area
	float normalizedPosValue = graph.GetNormalizedReferenceLineValue();

	// calculate line position in world space
	float linePosOffsetY = graph.Size.Y - graph.Size.Y * normalizedPosValue; // Y line offset in the plotting area starting from the top
	float worldLinePosY = plottingAreaWorldPosY + linePosOffsetY;
	FVector2D startPos(graph.Position.X, worldLinePosY); // draw from left to right
	FVector2D endPos(graph.Position.X + graph.Size.X, worldLinePosY);

	// draw line
	FCanvasLineItem lineItem(startPos, endPos);
	lineItem.SetColor(graph.ReferenceLineConfig.Color);
	lineItem.Draw(Canvas->Canvas);
}

void FGp_GraphPlotter::DrawReferenceLineValueText(const FGp_Graph &graph, float plottingAreaWorldPosY) const
{
	if (!graph.ReferenceLineConfig.Enabled)
	{
		return;
	}

	// create text
	float posValue = graph.ReferenceLineConfig.PositionValue;
	FText posValueText = FGp_Utils::GetFloatAsStringWithPrecision(posValue, 0);

	// horizontal position offset (from graph right edge)
	float valueTextWidth, valueTextHeight;
	Canvas->TextSize(Font, posValueText.ToString(), valueTextWidth, valueTextHeight);
	float graphEdgeXOffset = graph.Size.X - valueTextWidth - TextOffsetFromGraphEdge; // X offset from left edge of the graph

	// text world position
	float valueTextWorldPosX = graph.Position.X + graphEdgeXOffset;
	float valueTextWorldPosY = plottingAreaWorldPosY + LineSpacing;

	// draw text
	Canvas->SetDrawColor(FGp_Color::GraphText);
	Canvas->DrawText(Font, posValueText, valueTextWorldPosX, valueTextWorldPosY);
}

void FGp_GraphPlotter::PlotGraph(const FGp_Graph &graph, float plottingAreaWorldPosY) const
{
	// get data to draw
	TArray<float> normalizedData = graph.GetNormalizedData();
	if (normalizedData.Num() < 2)
	{
		return;
	}

	/* Draw line between each pair of points.
	Draw from first (oldest) point to the last, from left to right. */
	for (int i = 0; i < normalizedData.Num() - 1; ++i)
	{
		// left point position
		float startPosXRatio = (float(i) / (normalizedData.Num() - 1));
		float startPosX = graph.Position.X + (startPosXRatio * graph.Size.X);
		float startPosY = plottingAreaWorldPosY + graph.Size.Y + (-graph.Size.Y * normalizedData[i]);
		FVector2D startPos(startPosX, startPosY);

		// right point position
		float endPosXRatio = ((i + 1.f) / (normalizedData.Num() - 1));
		float endPosX = graph.Position.X + (endPosXRatio * graph.Size.X);
		float endPosY = plottingAreaWorldPosY + graph.Size.Y + (-graph.Size.Y * normalizedData[i + 1]);
		FVector2D endPos(endPosX, endPosY);

		// draw line
		FCanvasLineItem lineItem(startPos, endPos);
		lineItem.SetColor(FGp_Color::GraphLine);
		lineItem.Draw(Canvas->Canvas);
	}
}

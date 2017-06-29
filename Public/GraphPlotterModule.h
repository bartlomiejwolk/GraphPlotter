#pragma once

#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(FGp_GraphPlotter, All, All)

class FGraphPlotterModule : public IModuleInterface
{
public:
	void StartupModule() override;
	void ShutdownModule() override;

};
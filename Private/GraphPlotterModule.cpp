#include "GraphPlotterModule.h"

IMPLEMENT_GAME_MODULE(FGraphPlotterModule, GraphPlotterModule);

DEFINE_LOG_CATEGORY(FGp_GraphPlotter)

#define LOCTEXT_NAMESPACE "GraphPlotter"

void FGraphPlotterModule::StartupModule()
{
	UE_LOG(FGp_GraphPlotter, Log, TEXT("Module loaded!"));
}

void FGraphPlotterModule::ShutdownModule()
{
	UE_LOG(FGp_GraphPlotter, Log, TEXT("Module unloaded"));
}

#undef LOCTEXT_NAMESPACE
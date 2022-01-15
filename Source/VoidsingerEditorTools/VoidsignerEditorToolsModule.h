#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FVoidsingerEditorToolsModule : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
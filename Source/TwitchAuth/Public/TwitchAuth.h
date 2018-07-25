// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FTwitchAuthModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

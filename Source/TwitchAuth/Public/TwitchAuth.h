// Copyright (c) 2018 fivefingergames.

#pragma once

#include "ModuleManager.h"

class FTwitchAuthModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
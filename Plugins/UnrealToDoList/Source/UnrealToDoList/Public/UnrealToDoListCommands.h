// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "UnrealToDoListStyle.h"

class FUnrealToDoListCommands : public TCommands<FUnrealToDoListCommands>
{
public:

	FUnrealToDoListCommands()
		: TCommands<FUnrealToDoListCommands>(TEXT("UnrealToDoList"), NSLOCTEXT("Contexts", "UnrealToDoList", "UnrealToDoList Plugin"), NAME_None, FUnrealToDoListStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
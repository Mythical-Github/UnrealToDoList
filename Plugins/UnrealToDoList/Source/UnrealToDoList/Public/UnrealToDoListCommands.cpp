// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealToDoListPrivatePCH.h"
#include "UnrealToDoListCommands.h"

#define LOCTEXT_NAMESPACE "FUnrealToDoListModule"

void FUnrealToDoListCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UnrealToDoList", "Bring up UnrealToDoList window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

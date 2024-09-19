// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExampleEditorWidgetPrivatePCH.h"
#include "ExampleEditorWidgetCommands.h"

#define LOCTEXT_NAMESPACE "FExampleEditorWidgetModule"

void FExampleEditorWidgetCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ExampleEditorWidget", "Bring up ExampleEditorWidget window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "ExampleEditorWidgetStyle.h"

class FExampleEditorWidgetCommands : public TCommands<FExampleEditorWidgetCommands>
{
public:

	FExampleEditorWidgetCommands()
		: TCommands<FExampleEditorWidgetCommands>(TEXT("ExampleEditorWidget"), NSLOCTEXT("Contexts", "ExampleEditorWidget", "ExampleEditorWidget Plugin"), NAME_None, FExampleEditorWidgetStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
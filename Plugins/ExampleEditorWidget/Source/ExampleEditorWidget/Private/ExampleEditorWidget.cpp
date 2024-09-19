// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExampleEditorWidgetPrivatePCH.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "ExampleEditorWidgetStyle.h"
#include "ExampleEditorWidgetCommands.h"
#include "LevelEditor.h"
#include "ExampleEditorWidget.h" // Your module's header file
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"
#include "Editor.h"

static const FName ExampleEditorWidgetTabName("ExampleEditorWidget");

#define LOCTEXT_NAMESPACE "FExampleEditorWidgetModule"

const TCHAR* WidgetClassPath = TEXT("/ExampleEditorWidget/Widgets/WBP_ExampleEditorWidget.WBP_ExampleEditorWidget_C"); // Consolidated path

void FExampleEditorWidgetModule::StartupModule()
{
	FExampleEditorWidgetStyle::Initialize();
	FExampleEditorWidgetStyle::ReloadTextures();

	FExampleEditorWidgetCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FExampleEditorWidgetCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FExampleEditorWidgetModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FExampleEditorWidgetModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FExampleEditorWidgetModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ExampleEditorWidgetTabName, FOnSpawnTab::CreateRaw(this, &FExampleEditorWidgetModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FExampleEditorWidgetTabTitle", "ExampleEditorWidget"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	CachedUserWidget = nullptr;
}

void FExampleEditorWidgetModule::ShutdownModule()
{
	FExampleEditorWidgetStyle::Shutdown();
	FExampleEditorWidgetCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ExampleEditorWidgetTabName);
}

TSharedRef<SDockTab> FExampleEditorWidgetModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	if (!CachedUserWidget)
	{
		UClass* UserWidgetClass = LoadObject<UClass>(nullptr, WidgetClassPath); // Single place for the widget path

		if (!UserWidgetClass)
		{
			FText ErrorText = FText::Format(
				LOCTEXT("ErrorLoadingWidget", "Failed to load UserWidget class from {0}"),
				FText::FromString(WidgetClassPath)
			);

			return SNew(SDockTab)
				.TabRole(ETabRole::NomadTab)
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(ErrorText)
				]
				];
		}

		UUserWidget* ExistingUserWidget = nullptr;
		for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
		{
			if (Itr->IsA(UserWidgetClass))
			{
				ExistingUserWidget = *Itr;
				break;
			}
		}

		if (ExistingUserWidget)
		{
			CachedUserWidget = ExistingUserWidget;
		}
		else
		{
			CachedUserWidget = CreateWidget<UUserWidget>(GEditor->GetEditorWorldContext().World(), UserWidgetClass);
		}

		if (!CachedUserWidget)
		{
			FText ErrorText = FText::Format(
				LOCTEXT("ErrorCreatingWidget", "Failed to create UserWidget instance from {0}"),
				FText::FromString(WidgetClassPath)
			);

			return SNew(SDockTab)
				.TabRole(ETabRole::NomadTab)
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(ErrorText)
				]
				];
		}
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			CachedUserWidget->TakeWidget()
		]
		];
}

void FExampleEditorWidgetModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(ExampleEditorWidgetTabName);
}

void FExampleEditorWidgetModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FExampleEditorWidgetCommands::Get().OpenPluginWindow);
}

void FExampleEditorWidgetModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FExampleEditorWidgetCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FExampleEditorWidgetModule, ExampleEditorWidget)

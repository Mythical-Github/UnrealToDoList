// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealToDoListPrivatePCH.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "UnrealToDoListStyle.h"
#include "UnrealToDoListCommands.h"
#include "LevelEditor.h"
#include "UnrealToDoList.h" // Your module's header file
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"
#include "Editor.h"

static const FName UnrealToDoListTabName("UnrealToDoList");

#define LOCTEXT_NAMESPACE "FUnrealToDoListModule"

const TCHAR* WidgetClassPath = TEXT("/UnrealToDoList/Widgets/WBP_UnrealToDoListEntry.WBP_UnrealToDoListEntry_C");

void FUnrealToDoListModule::StartupModule()
{
	FUnrealToDoListStyle::Initialize();
	FUnrealToDoListStyle::ReloadTextures();

	FUnrealToDoListCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUnrealToDoListCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FUnrealToDoListModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FUnrealToDoListModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FUnrealToDoListModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UnrealToDoListTabName, FOnSpawnTab::CreateRaw(this, &FUnrealToDoListModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FUnrealToDoListTabTitle", "UnrealToDoList"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	CachedUserWidget = nullptr;
}

void FUnrealToDoListModule::ShutdownModule()
{
	FUnrealToDoListStyle::Shutdown();
	FUnrealToDoListCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UnrealToDoListTabName);
}

TSharedRef<SDockTab> FUnrealToDoListModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
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

void FUnrealToDoListModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(UnrealToDoListTabName);
}

void FUnrealToDoListModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUnrealToDoListCommands::Get().OpenPluginWindow);
}

void FUnrealToDoListModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUnrealToDoListCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealToDoListModule, UnrealToDoList)

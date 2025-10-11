// Copyright Juan Quiroz


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitalValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	// Health
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	 (AuraAttributeSet->GetHealthAttribute()).AddLambda(
	  [this](const FOnAttributeChangeData& Data)
	  {
		OnHealthChanged.Broadcast(Data.NewValue);
	  }
	 );
	// Max Health
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	 (AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
	  [this](const FOnAttributeChangeData& Data)
	  {
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	  }
	 );

	// Mana
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	 (AuraAttributeSet->GetManaAttribute()).AddLambda(
	  [this](const FOnAttributeChangeData& Data)
	  {
		OnManaChanged.Broadcast(Data.NewValue);
	  }
	 );

	// Max Mana
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	 (AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
	  [this](const FOnAttributeChangeData& Data)
	  {
		OnMaxManaChanged.Broadcast(Data.NewValue);
	  }
	 );

	// Bind to the EffectAssetTags delegate in the AuraAbilitySystemComponent
	// We use Cast to convert the AbilitySystemComponent to our custom AuraAbilitySystemComponent
	// Then we add a lambda function to the EffectAssetTags delegate
	// The lambda function takes a FGameplayTagContainer as a parameter and iterates through the tags
	// For each tag, it creates a debug message and displays it on the screen for 8 seconds in blue color
	// This is useful for avoiding making member Function/variables for simple tasks
	// We capture object to use GetDataTableRowByTag function
	// We can also capture specific variables if needed by using [this, VariableName]
	
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// "Message.HealthPotion" .MatchesTag("Message") -> true
				// "Message.HealthPotion" .MatchesTag("Message.HealthPotion") -> true
				// "Message" .MatchesTag("Message.HealthPotion") -> false
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if(Tag.MatchesTag(MessageTag))
				{
					//since this function is template, we can use it to get any type of row from the data table
					//but we must specify the type when calling the function
					//in this case we want to get a row of type FUIWidgetRow
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}

			}
		}
	);
}

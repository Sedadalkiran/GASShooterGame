// Fill out your copyright notice in the Description page of Project Settings.


#include "GSGAbilitySystemComponent.h"

#include "EnhancedInputComponent.h"
#include "GSGGameplayAbility.h"
#include "InputAction.h"


UGSGAbilitySystemComponent::UGSGAbilitySystemComponent()
{
}

void UGSGAbilitySystemComponent::EnhancedInputAction(const FInputActionInstance& ActionInstace)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (UGSGGameplayAbility* PlayerAbility = Cast<UGSGGameplayAbility>(Spec.Ability))
		{
			bool bInputConsumed = false;
			if (PlayerAbility->InputAction == ActionInstace.GetSourceAction())
			{
				if (!Spec.IsActive())
				{
					if (PlayerAbility->bActivateOnInput)
					{
						// Ability is not active, so try to activate it
						bInputConsumed = TryActivateAbility(Spec.Handle);
					}
				}
			}
		}
	}
}

void UGSGAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);


	if (UGSGGameplayAbility* PlayerAbility = Cast<UGSGGameplayAbility>(AbilitySpec.Ability))
	{
		if (PlayerAbility->InputAction)
		{
			if (APawn* Pawn = GetOwner<APawn>())
			{
				if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(
					Pawn->InputComponent))
				{
					PlayerEnhancedInputComponent->BindAction<UGSGAbilitySystemComponent>(
						PlayerAbility->InputAction, ETriggerEvent::Triggered, this,
						&UGSGAbilitySystemComponent::EnhancedInputAction);
				}
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "GSGAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASSHOOTERGAME_API UGSGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGSGAbilitySystemComponent();
	void EnhancedInputAction(const FInputActionInstance& ActionInstace);
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
protected:
	

public:
	bool bCharacterAbilitiesGiven = false;
};

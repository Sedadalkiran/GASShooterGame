// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSGGameplayAbility.h"
#include "UObject/Object.h"
#include "GSGGameplayAbility_Fire.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTERGAME_API UGSGGameplayAbility_Fire : public UGSGGameplayAbility
{
	
	GENERATED_BODY()
public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
	
	UFUNCTION(BlueprintCallable)
	void StartWeaponTargeting();

	UFUNCTION(BlueprintImplementableEvent)
	void PerformLocalTargeting(FHitResult& OutHits);

	// Called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
	
private:

	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;
};

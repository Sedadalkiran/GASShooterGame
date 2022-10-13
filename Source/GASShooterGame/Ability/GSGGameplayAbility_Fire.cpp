// Fill out your copyright notice in the Description page of Project Settings.


#include "GSGGameplayAbility_Fire.h"

#include "AbilitySystemComponent.h"

void UGSGGameplayAbility_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* AbilityComp = CurrentActorInfo->AbilitySystemComponent.Get();
	check(AbilityComp);
	OnTargetDataReadyCallbackDelegateHandle = AbilityComp->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnTargetDataReadyCallback);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGSGGameplayAbility_Fire::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* AbilityComp = CurrentActorInfo->AbilitySystemComponent.Get();
	check(AbilityComp);
	// When ability ends, consume target data and remove delegate
	AbilityComp->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).Remove(OnTargetDataReadyCallbackDelegateHandle);
	AbilityComp->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGSGGameplayAbility_Fire::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData,
	FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilityComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FScopedPredictionWindow    ScopedPrediction(MyAbilityComponent);

		// Take ownership of the target data to make sure no callbacks into game code invalidate it out from under us
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority();
		if (bShouldNotifyServer)
		{
			MyAbilityComponent->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), LocalTargetDataHandle, ApplicationTag, MyAbilityComponent->ScopedPredictionKey);
		}
        
		// See if we still have ammo
		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			// Let the blueprint do stuff like apply effects to the targets
			OnWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			K2_EndAbility();
		}
	}

	// We've processed the data
	MyAbilityComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}

void UGSGGameplayAbility_Fire::StartWeaponTargeting()
{
	UAbilitySystemComponent* AbilityComp = CurrentActorInfo->AbilitySystemComponent.Get();
	check(AbilityComp);
	FScopedPredictionWindow ScopedPrediction(AbilityComp, CurrentActivationInfo.GetActivationPredictionKey());/*Server client bekleme*/
	FHitResult OutHit;
	PerformLocalTargeting(OutHit);
	FGameplayAbilityTargetDataHandle TargetData;
	if(OutHit.IsValidBlockingHit())
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
		NewTargetData->HitResult = OutHit;
		TargetData.Add(NewTargetData);
	}
	// Process the target data immediately
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}

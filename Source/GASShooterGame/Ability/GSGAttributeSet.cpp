// Fill out your copyright notice in the Description page of Project Settings.


#include "GSGAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GSGAbilitySystemComponent.h"
#include "GASShooterGame/GASShooterGameCharacter.h"
#include "Net/UnrealNetwork.h"

void UGSGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UGSGAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UGSGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSGAttributeSet, Health, OldHealth);
}

void UGSGAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const//Attribute değerii değişmeden önce çağrılır.
{
	Super::PreAttributeBaseChange(Attribute, NewValue);//Yeni attribute değeri NewValue

	if(Attribute==GetHealthAttribute() && NewValue<0.f)
	{
		NewValue=0.f;
	}
		
}

void UGSGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(GetHealth()<=0.f)
	{
		AGASShooterGameCharacter* OwnerCharacter = Cast<AGASShooterGameCharacter>(GetOwningActor());
		if (UAbilitySystemComponent* AbilitySystemComponent = OwnerCharacter->GetAbilitySystemComponent())
		{
			const bool bIsDead =  AbilitySystemComponent->HasMatchingGameplayTag(OwnerCharacter->DeathTag);
			if (!bIsDead)
			{
				if (AbilitySystemComponent)
				{
					FGameplayEventData Payload;
					Payload.EventTag = OwnerCharacter->DeathTag;
					Payload.Target = AbilitySystemComponent->GetAvatarActor();
					Payload.EventMagnitude = Data.EvaluatedData.Magnitude;

					FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
					AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
				}
			}
		}
	}
	
}

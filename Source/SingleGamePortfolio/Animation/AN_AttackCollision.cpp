// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_AttackCollision.h"

void UAN_AttackCollision::Notify(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackInterface* AttackCharacter = 
		Cast<IAttackInterface>(MeshComp->GetOwner());

	if (AttackCharacter)
	{
		AttackCharacter->AttackCollisionCheckOnce(Offset, Radius, AttackType);
	}
}
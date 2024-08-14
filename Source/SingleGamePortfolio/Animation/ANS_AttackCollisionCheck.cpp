// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AttackCollisionCheck.h"
#include "Character/PlayerCharacter.h"
#include "Item/PlayerWeapon.h"
#include "Components/CapsuleComponent.h"

void UANS_AttackCollisionCheck::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player)
	{
		mOwningCharacter = Player;
		APlayerWeapon* PlayerWeapon = mOwningCharacter->GetWeapon();
		PlayerWeapon->SetPrevCollisionPos
		((PlayerWeapon->GetCollisonEndPos() + PlayerWeapon->GetCollisionStartPos()) * 0.5f);
		if(!bMonsterCollisionBlock)
			mOwningCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	}
}

void UANS_AttackCollisionCheck::NotifyTick(USkeletalMeshComponent* MeshComp, 
	UAnimSequenceBase* Animation, float FrameDeltaTime, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (mOwningCharacter)
	{
		mOwningCharacter->AttackCollisionCheck();
	}
}

void UANS_AttackCollisionCheck::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (mOwningCharacter)
	{
		mOwningCharacter->ResetAttackedCharacters();
		if (!bMonsterCollisionBlock)
			mOwningCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}

}

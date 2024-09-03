// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AttackCollisionCheck.h"
#include "Character/PlayerCharacter.h"
#include "Item/PlayerWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Interface/AttackInterface.h"
#include "Character/Enemy/StelaeKnight.h"

void UANS_AttackCollisionCheck::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	mOwningCharacter = Cast<ACharacterBase>(MeshComp->GetOwner());
	APlayerCharacter* Player = Cast<APlayerCharacter>(mOwningCharacter);
	if (Player)
	{
		APlayerWeapon* PlayerWeapon = Player->GetWeapon();
		PlayerWeapon->SetPrevCollisionPos
		((PlayerWeapon->GetCollisionEndPos() + PlayerWeapon->GetCollisionStartPos()) * 0.5f);
		if(!bMonsterCollisionBlock)
			mOwningCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
		return;
	}
	AStelaeKnight* Boss = Cast<AStelaeKnight>(mOwningCharacter);
	if (Boss)
	{
		Boss->SetPrevCollisionPos((Boss->GetCollisionEndPos() + Boss->GetCollisionStartPos()) * 0.5f);
	}
}

void UANS_AttackCollisionCheck::NotifyTick(USkeletalMeshComponent* MeshComp, 
	UAnimSequenceBase* Animation, float FrameDeltaTime, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (mOwningCharacter)
	{
		Cast<IAttackInterface>(mOwningCharacter)->AttackCollisionCheck(AttackType);
	}
}

void UANS_AttackCollisionCheck::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (mOwningCharacter)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(mOwningCharacter);
		if (Player)
		{
			Player->ResetAttackedCharacters();
			if (!bMonsterCollisionBlock)
				mOwningCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
			return;
		}
		AStelaeKnight* Boss = Cast<AStelaeKnight>(mOwningCharacter);
		if (Boss)
		{
			Boss->SetPrevCollisionPos((Boss->GetCollisionEndPos() + Boss->GetCollisionStartPos()) * 0.5f);
		}
	}

}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectMPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API AProjectMPlayerState : public APlayerState
{
	GENERATED_BODY()

	

public:
	AProjectMPlayerState();
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 _points);

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Score, BlueprintReadWrite,Category = "Score", meta = (AllowPrivateAccess = "true"))
	int32 _score;

	
	void OnRep_Score();
};

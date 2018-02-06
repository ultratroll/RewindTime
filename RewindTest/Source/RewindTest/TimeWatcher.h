// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
#include "TimeWatcher.generated.h"

class ActorComponent;

/*
	.
*/
USTRUCT(BlueprintType)
struct FRecordData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FVector_NetQuantize Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FVector_NetQuantize LookAtTarget;

	//For GC
	void Destroy()
	{
		;
	}

	//Constructor
	FRecordData()
	{
		Position = FVector_NetQuantize();
		Rotation = FRotator();
		LookAtTarget = FVector_NetQuantize();
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REWINDTEST_API UTimeWatcher : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeWatcher();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Record data for all the character history. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave, meta = (AllowPrivateAccess = "true"))
	TArray<FRecordData> Records;

	/* This is component is allowed to save records?. */
	bool bCanSaveRecords;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void SaveCurrentRecord();

};

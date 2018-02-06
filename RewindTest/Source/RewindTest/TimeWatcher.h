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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FName AnimationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		float AnimationPosition;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	TArray<FRecordData> Records;

	/* This is component is allowed to save records?. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	bool bCanSaveRecords= false;

	/* Times for second we are going to record data. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	int RecordSavingFrequency = 16;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Lets save the current position, rotation, target, animation name and point of that animation. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void SaveCurrentRecord();

	/* Lets start saving data. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void StartRecording();

	/* Lets stop recording data. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void StopRecording();

};

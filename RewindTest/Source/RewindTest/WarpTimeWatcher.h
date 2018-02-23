// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WarpTimeWatcher.generated.h"

class ActorComponent;

/** Structure to record and hold historic data. */
USTRUCT(BlueprintType)
struct FWarpRecordData
{ // Save time for each record
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FVector_NetQuantize Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FVector_NetQuantize Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
		FName AnimationName; // probably will use actions instead

	void Destroy()
	{
		;
	}

	FWarpRecordData()
	{
		;
	}
};

/** 
	The time watcher component serves to record historic data and reproduce it backwards or forward at the needed speeds. Interpolating the record data. 
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REWINDTEST_API UWarpTimeWatcher : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWarpTimeWatcher();

protected:
	virtual void BeginPlay() override;

	/** Record data for all the character history. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	TArray<FWarpRecordData> Records;

	/** This is component is allowed to save records?. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	bool bCanSaveRecords;

	/** This is component is rewinding records?. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	bool bIsRewinding;

	/** This is component is replaying records?. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	bool bIsReplaying;

	/** Times for second we are going to record data. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	int RecordSavingFrequency = 16;

	/** Current record data index being reproduced (in rewind or replay). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Record, meta = (AllowPrivateAccess = "true"))
	int CurrentRecordIndex = 0;

	/** Handle to manage the recording timer. */
	FTimerHandle RecordTimer;

private:
	/** If we are currently interpolating bettwen records. */
	bool bIsInterpolatingRecords;

	/** For internal use. The diference in time bettwen two records. */
	float RecordDeltaTime;

	/** For internal use. The current time bettwen two records being interpolated. */
	float CurrentRecordDeltaTime;

	/** For internal use. The real delta time. */
	float RealDeltaTime;

	/** For internal use. The real time. */
	float RealTime;

	/** Update the real delta time. */
	void UpdateRealDeltaTime();

	/** Rewind rate. Regular rewind speed at 1. */
	float RewindRate= 1.0f;

	/** Replay rate. Regular replay speed at 1. */
	float ReplayRate = 1.0f;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Lets save the current position, rotation, target, animation name and point of that animation. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void SaveCurrentRecord();

	/** Lets start saving data. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void StartRecording();

	/** Lets stop recording data. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void StopRecording();

	/** Rewind. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void Rewind();

	/** Replay. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void Replay();

	/**
	*	Setter rewind rate.
	*	@param	Rate	Rate to be applied.
	*/
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
		void SetRewindRate(float Rate);

	/** 
	*	Setter replay rate. 
	*	@param	Rate	Rate to be applied.
	*/
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
		void SetReplayRate(float Rate);

	/** 
	*	Lets apply the record data in the given index. 
	*	@param	Index	Index of record to be applied.
	*/
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void ApplyRecordByIndex(int Index);

	/**
	*	Lets apply the record data.
	*	@param	Record	Record to be applied.
	*/
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
	void ApplyRecord(FWarpRecordData Record);

	/**
	*	Lets interpolate bettwen two records and apply the result.
	*	@param	StartRecord	Record to be applied.
	*	@param	EndRecord	Record to be applied.
	*/
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Projectile)
		void InterpolateRecords(FWarpRecordData StartRecord, FWarpRecordData EndRecord, float Alpha);
};

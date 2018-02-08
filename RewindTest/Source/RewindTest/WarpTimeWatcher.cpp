// Fill out your copyright notice in the Description page of Project Settings.

#include "WarpTimeWatcher.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UWarpTimeWatcher::UWarpTimeWatcher()
{
	PrimaryComponentTick.bCanEverTick = true;

	Records=  TArray<FWarpRecordData>();
}


void UWarpTimeWatcher::BeginPlay()
{
	Super::BeginPlay();
	Records.Empty();
	StartRecording();
}


void UWarpTimeWatcher::UpdateRealDeltaTime()
{
	RealDeltaTime = UGameplayStatics::GetRealTimeSeconds(GetWorld()) -RealTime;
	RealTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

void UWarpTimeWatcher::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsRewinding)
	{
		if ((CurrentRecordIndex - 1) >=0 )
		{
			if (!bIsInterpolatingRecords)
			{
				RecordDeltaTime = Records[CurrentRecordIndex].Time - Records[CurrentRecordIndex - 1].Time;
				CurrentRecordDeltaTime = RecordDeltaTime;
				bIsInterpolatingRecords = true;
			}
			UpdateRealDeltaTime();
			CurrentRecordDeltaTime -= RealDeltaTime; // DeltaTime
			float Alpha = (1 - (CurrentRecordDeltaTime / RecordDeltaTime)); // This DeltaTime wont serve us in pause, will calculate our own with getrealtime

			UE_LOG(LogTemp, Warning, TEXT(">> Before %d :: %f"), CurrentRecordIndex, Alpha);

			InterpolateRecords(Records[CurrentRecordIndex], Records[CurrentRecordIndex - 1], Alpha);
			if (Alpha >= 1)
			{
				bIsInterpolatingRecords= false;
				CurrentRecordIndex--;
			}
		}
		else {
			// Is not possible to continue rewinding trough records 
			bIsRewinding = false;
		}

		/* old way
		ApplyRecordByIndex(CurrentRecordIndex);
		CurrentRecordIndex--;
		if (CurrentRecordIndex < 0) 
			bIsRewinding = false;*/
			
	}
}

void UWarpTimeWatcher::SaveCurrentRecord()
{
	if (!bCanSaveRecords) 
		return;
	FWarpRecordData Record = FWarpRecordData();
	Record.Position = this->GetOwner()->GetActorLocation();
	Record.Rotation = this->GetOwner()->GetActorRotation();
	Record.Time		= UGameplayStatics::GetRealTimeSeconds(GetWorld());
	Records.Add(Record);
}

void UWarpTimeWatcher::StartRecording()
{
	bCanSaveRecords = true;
	GetWorld()->GetTimerManager().SetTimer(RecordTimer, this, &UWarpTimeWatcher::SaveCurrentRecord, 0.05f, true); // 1/RecordSavingFrequency
}

void UWarpTimeWatcher::StopRecording()
{
	bCanSaveRecords = false;
	GetWorld()->GetTimerManager().ClearTimer(RecordTimer);
}

void UWarpTimeWatcher::Rewind()
{
	StopRecording();
	bIsRewinding = true;
	CurrentRecordIndex = Records.Num()-1;
}

void UWarpTimeWatcher::Replay()
{
	StopRecording();
}

void UWarpTimeWatcher::ApplyRecordByIndex(int Index)
{
	this->GetOwner()->SetActorLocation(Records[Index].Position);
	this->GetOwner()->SetActorRotation(Records[Index].Rotation);
}

void UWarpTimeWatcher::ApplyRecord(FWarpRecordData Record)
{
	this->GetOwner()->SetActorLocation(Record.Position);
	this->GetOwner()->SetActorRotation(Record.Rotation);
}

void UWarpTimeWatcher::InterpolateRecords(FWarpRecordData StartRecord, FWarpRecordData EndRecord, float Alpha)
{
	FWarpRecordData Record = FWarpRecordData();
	Record.Position = FMath::Lerp(StartRecord.Position, EndRecord.Position, Alpha);
	Record.Rotation = FMath::Lerp(StartRecord.Rotation, EndRecord.Rotation, Alpha);
	ApplyRecord(Record);
}



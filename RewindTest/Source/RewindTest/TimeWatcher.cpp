// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeWatcher.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTimeWatcher::UTimeWatcher()
{
	PrimaryComponentTick.bCanEverTick = true;

	Records=  TArray<FRecordData>();
}


// Called when the game starts
void UTimeWatcher::BeginPlay()
{
	Super::BeginPlay();
	Records.Empty();
	StartRecording();
}


// Called every frame
void UTimeWatcher::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsRewinding)
	{
		ApplyRecord(CurrentRecordIndex);
		
		UE_LOG(LogTemp, Warning, TEXT("<< Rewinding data %d of %d"), CurrentRecordIndex, Records.Num() - 1);
		CurrentRecordIndex--;
		if (CurrentRecordIndex < 0) bIsRewinding = false;
	}
}

void UTimeWatcher::SaveCurrentRecord()
{
	if (!bCanSaveRecords) return;
	FRecordData Record = FRecordData();
	Record.Position = this->GetOwner()->GetActorLocation();
	Record.Rotation = this->GetOwner()->GetActorRotation();
	Record.AnimationName = "";
	Records.Add(Record);
	//UE_LOG(LogTemp, Warning, TEXT("Recording data %d"), Records.Num());
}

void UTimeWatcher::StartRecording()
{
	bCanSaveRecords = true;
	GetWorld()->GetTimerManager().SetTimer(RecordTimer, this, &UTimeWatcher::SaveCurrentRecord, 0.05f, true); // 1/RecordSavingFrequency
}

void UTimeWatcher::StopRecording()
{
	bCanSaveRecords = false;
	GetWorld()->GetTimerManager().ClearTimer(RecordTimer);
}

void UTimeWatcher::Rewind()
{
	StopRecording();
	bIsRewinding = true;
	CurrentRecordIndex = Records.Num()-1;
	UE_LOG(LogTemp, Warning, TEXT("<< Lets rewind "));
	/*for(int i= Index-1; i>=0; i--)
	{ 
		
		if (i>=0) ApplyRecord(i);
	}*/
}

void UTimeWatcher::Replay()
{
	StopRecording();
}

void UTimeWatcher::ApplyRecord(int Index)
{
	UE_LOG(LogTemp, Warning, TEXT(">> Lets apply Record data %d"), Index);
	this->GetOwner()->SetActorLocation(Records[Index].Position);
	this->GetOwner()->SetActorRotation(Records[Index].Rotation);
}


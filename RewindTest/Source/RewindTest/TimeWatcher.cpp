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
}

void UTimeWatcher::SaveCurrentRecord()
{
	if (!bCanSaveRecords) return;
	//UE_LOG(LogTemp, Warning, TEXT("Recording data"));
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
	int Index = Records.Num();
	for(int i= Index; i>=0; i--)
	{ 
		ApplyRecord(Index);
	}
}

void UTimeWatcher::Replay()
{
	StopRecording();
}

void UTimeWatcher::ApplyRecord(int Index)
{
	this->GetOwner()->SetActorLocation(Records[Index].Position);
	this->GetOwner()->SetActorRotation(Records[Index].Rotation);
}


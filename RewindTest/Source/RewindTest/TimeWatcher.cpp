// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeWatcher.h"
#include "Components/ActorComponent.h"

// Sets default values for this component's properties
UTimeWatcher::UTimeWatcher()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Records=  TArray<FRecordData>();
}


// Called when the game starts
void UTimeWatcher::BeginPlay()
{
	Super::BeginPlay();
	Records.Empty();
	bCanSaveRecords = true;
}


// Called every frame
void UTimeWatcher::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTimeWatcher::SaveCurrentRecord()
{
	FRecordData Record = FRecordData();
	Record.Position = this->GetOwner()->GetActorLocation();
	Record.Rotation = this->GetOwner()->GetActorRotation();
	Record.AnimationName = "";
	Records.Add(Record);
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



class AMeshGenerator;
/**
 * 
 */

class ASSIGNMENT_5_6_API FAsyncScatterTask : public FNonAbandonableTask
{
public:
	//FAsyncScatterTask();
	FAsyncScatterTask(AMeshGenerator* InScatterActor);

	void DoWork();
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncScatterTask, STATGROUP_ThreadPoolAsyncTasks);
	}


private:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMeshGenerator* ScatterActor;
	
};


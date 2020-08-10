// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQLiteDatabaseConnection.h"
#include "WeatherAppTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DatabaseFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogDatabase, Log, All);

/**
 * 
 */
UCLASS()
class UDatabaseFunctionLibrary final : public UObject
{
    GENERATED_BODY()

    static void CreateDatabase();

    static FString GetPathToDatabase();

    static FSQLiteDatabaseConnection Database;

public:
    static void OpenDatabaseConnection();

    static void CloseDatabaseConnection();

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static TArray<int> GetAllTracks();

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static void InsertTracks(const TArray<int> Tracks);

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static void RemoveTracks(const TArray<int> Tracks);

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static int GetHistory();

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static void InsertOrUpdateCityInfo(const FWeatherDataForecast5& Data);

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static void RemoveWeatherByCityId(int CityId);

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static FWeatherDataForecast5 GetWeatherByCityId(int CityId);

    UFUNCTION(BlueprintCallable, Category="Database|Queries")
    static void UpdateHistory();
};

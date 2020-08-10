// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WeatherAppTypes.h"
#include "../../../../../../../../Soft/UE_4.25/Engine/Plugins/Runtime/Database/SQLiteSupport/Source/SQLiteSupport/Public/SQLiteDatabaseConnection.h"

#include "WeatherAppBlueprintLibrary.generated.h"

class FSQLiteDatabase;
DECLARE_DYNAMIC_DELEGATE(FOnRequestSuccess);

DECLARE_DYNAMIC_DELEGATE(FOnExecuted);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCurrentWeatherReceived, const FWeatherDataCurrent&, Weather);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnForecastWeatherReceived, const FWeatherDataForecast5&, Weather);

DECLARE_DELEGATE(FReq);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnBadRequest, const FString&, Code, const FString&, Description);

DECLARE_LOG_CATEGORY_CLASS(LogWeatherApp, Log, All);

/**
 * 
 */
UCLASS()
class UWeatherAppBlueprintLibrary final : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static TMap<int, FSearchCity> GetCitiesBySubstringName(const FString Name);

    FORCENOINLINE static bool LoadSearchCities();

    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static void GetCurrentWeatherForCityById(const int CityId, const FOnCurrentWeatherReceived& ResponseCallback,
                                             const FOnBadRequest& BadRequestCallback);

    static TSharedRef<IHttpRequest> ConfigureRequest(const int CityId, const bool bGetCurrentTime);

    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static void GetForecast5WeatherForCityById(int CityId, const FOnForecastWeatherReceived& ResponseCallback,
                                               const FOnBadRequest& BadRequestCallback);

    static void GetCurrentWeatherForCityById_HttpRequestComplete(const FHttpRequestPtr HttpRequest,
                                                                 const FHttpResponsePtr HttpResponse,
                                                                 const bool bSucceeded,
                                                                 const FOnCurrentWeatherReceived ResponseCallback,
                                                                 const FOnBadRequest BadRequestCallback);

    static void GetForecastWeatherForCityById_HttpRequestComplete(const FHttpRequestPtr HttpRequest,
                                                                  const FHttpResponsePtr HttpResponse,
                                                                  const bool bSucceeded,
                                                                  const FOnForecastWeatherReceived ResponseCallback,
                                                                  const FOnBadRequest BadRequestCallback);

    static void CheckIsInternetAvailable_HttpRequestComplete(
        const FHttpRequestPtr HttpRequest,
        const FHttpResponsePtr HttpResponse, const bool bSucceeded, const FOnExecuted OnExecuted);

    static TArray<FSearchCity> SearchCitiesData;

    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static void InitializationWeatherApp();

    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static void PrepareForExit();

    UFUNCTION(BlueprintPure, Category="WeatherApp|Helpers")
    static FString ConvertKelvinsToDegreesString(const float Kelvins);

    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static void CheckIsInternetAvailable(const FOnExecuted& OnExecuted);

    static bool bIsInternetAvailable;

    UFUNCTION(BlueprintPure, Category="WeatherApp|Helpers")
    static bool IsInternetAvailable();

    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static FWeatherDataCurrent GetWeatherForCurrentTime();

public:
    UFUNCTION(BlueprintCallable, Category="WeatherApp|Helpers")
    static FSearchCity FindSearchCityById(int CityId);

    UFUNCTION(BlueprintPure, Category="WeatherApp|Helpers")
    static FDateTime GetDatetimeFromInt(const int64 Time);
};

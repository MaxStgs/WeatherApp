// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherAppBlueprintLibrary.h"
#include "DatabaseFunctionLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "Kismet/KismetTextLibrary.h"

TArray<FSearchCity> UWeatherAppBlueprintLibrary::SearchCitiesData = TArray<FSearchCity>();
bool UWeatherAppBlueprintLibrary::bIsInternetAvailable = false;

void UWeatherAppBlueprintLibrary::InitializationWeatherApp()
{
    if (SearchCitiesData.Num() == 0)
    {
        const bool bSuccess = LoadSearchCities();
        if (!bSuccess)
        {
            UE_LOG(LogWeatherApp, Error,
                   TEXT("UWeatherAppBlueprintLibrary::GetCitiesBySubstringName() LoadSearchCities() returned false"));
        }
    }

    UDatabaseFunctionLibrary::OpenDatabaseConnection();
}

void UWeatherAppBlueprintLibrary::PrepareForExit()
{
    UDatabaseFunctionLibrary::CloseDatabaseConnection();
}

FString UWeatherAppBlueprintLibrary::ConvertKelvinsToDegreesString(const float Kelvins)
{
    const float Degrees = Kelvins - 273.15;
    const FText Text = UKismetTextLibrary::Conv_FloatToText(Degrees, HalfToEven);
    return Text.ToString().Replace(TEXT(","), TEXT("."));
}

void UWeatherAppBlueprintLibrary::CheckIsInternetAvailable(const FOnExecuted& OnExecuted)
{
    TSharedRef<IHttpRequest> HttpRequest = ConfigureRequest(-1, false);

    HttpRequest->OnProcessRequestComplete().BindStatic(
        &UWeatherAppBlueprintLibrary::CheckIsInternetAvailable_HttpRequestComplete, OnExecuted);
    HttpRequest->ProcessRequest();
}

bool UWeatherAppBlueprintLibrary::IsInternetAvailable()
{
    return bIsInternetAvailable;
}

FWeatherDataCurrent UWeatherAppBlueprintLibrary::GetWeatherForCurrentTime()
{
    return {};
}

TMap<int, FSearchCity> UWeatherAppBlueprintLibrary::GetCitiesBySubstringName(const FString Name)
{
    TMap<int, FSearchCity> Result = TMap<int, FSearchCity>();

    // Is loaded
    if (SearchCitiesData.Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                         "UWeatherAppBlueprintLibrary::GetCitiesBySubstringName() SearchCitiesData is empty");
    }

    for (auto JsonCity : SearchCitiesData)
    {
        if (JsonCity.Name.Contains(Name))
        {
            Result.Add(JsonCity.Id, JsonCity);
        }
    }

    return Result;
}

bool UWeatherAppBlueprintLibrary::LoadSearchCities()
{
    // Load data from Json
    const FString PathToFileJson = FPaths::ProjectDir() / "Extras" / "city.list.json";
    FString CitiesString;
    const bool Result = FFileHelper::LoadFileToString(CitiesString, *PathToFileJson);

    if (!Result)
    {
        return false;
    }

    FJsonObjectConverter::JsonArrayStringToUStruct<FSearchCity>(*CitiesString, &SearchCitiesData, 0, 0);
    return true;
}

void UWeatherAppBlueprintLibrary::GetCurrentWeatherForCityById(const int CityId,
                                                               const FOnCurrentWeatherReceived& ResponseCallback,
                                                               const FOnBadRequest& BadRequestCallback)
{
    TSharedRef<IHttpRequest> HttpRequest = ConfigureRequest(CityId, true);

    HttpRequest->OnProcessRequestComplete().BindStatic(
        &UWeatherAppBlueprintLibrary::GetCurrentWeatherForCityById_HttpRequestComplete,
        ResponseCallback, BadRequestCallback);
    HttpRequest->ProcessRequest();
}

void UWeatherAppBlueprintLibrary::GetForecast5WeatherForCityById(const int CityId,
                                                                 const FOnForecastWeatherReceived& ResponseCallback,
                                                                 const FOnBadRequest& BadRequestCallback)
{
    TSharedRef<IHttpRequest> HttpRequest = ConfigureRequest(CityId, false);

    HttpRequest->OnProcessRequestComplete().BindStatic(
        &UWeatherAppBlueprintLibrary::GetForecastWeatherForCityById_HttpRequestComplete,
        ResponseCallback, BadRequestCallback);
    HttpRequest->ProcessRequest();
}

void UWeatherAppBlueprintLibrary::CheckIsInternetAvailable_HttpRequestComplete(
    const FHttpRequestPtr HttpRequest,
    const FHttpResponsePtr HttpResponse,
    const bool bSucceeded, const FOnExecuted OnExecuted)
{
    if (HttpResponse->GetResponseCode() != 0)
    {
        bIsInternetAvailable = true;
    }
    else
    {
        bIsInternetAvailable = false;
    }

    if (!OnExecuted.ExecuteIfBound())
    {
        UE_LOG(LogWeatherApp, Error, TEXT("CheckIsInternetAvailable_HttpRequestComplete() OnExecuted not Executed"));
    }
}

void UWeatherAppBlueprintLibrary::GetCurrentWeatherForCityById_HttpRequestComplete(const FHttpRequestPtr HttpRequest,
    const FHttpResponsePtr HttpResponse, const bool bSucceeded, const FOnCurrentWeatherReceived ResponseCallback,
    const FOnBadRequest BadRequestCallback)
{
    FWeatherDataCurrent Weather;

    const FString ResponseStr = HttpResponse->GetContentAsString();

    FJsonObjectConverter::JsonObjectStringToUStruct<FWeatherDataCurrent>(*ResponseStr, &Weather, 0, 0);

    const bool Result = ResponseCallback.ExecuteIfBound(Weather);
    if (Result)
    {
        UE_LOG(LogWeatherApp, Log,
               TEXT("UWeatherAppBlueprintLibrary::GetForecastWeatherForCityById() ResponseCallback not executed"));
    }
}

void UWeatherAppBlueprintLibrary::GetForecastWeatherForCityById_HttpRequestComplete(
    const FHttpRequestPtr HttpRequest,
    const FHttpResponsePtr HttpResponse, const bool bSucceeded, const FOnForecastWeatherReceived ResponseCallback,
    const FOnBadRequest BadRequestCallback)
{
    if (HttpResponse->GetResponseCode() != 200)
    {
        const bool bResult = BadRequestCallback.ExecuteIfBound(
            *FString::Printf(TEXT("%d"), HttpResponse->GetResponseCode()), "");
        if (!bResult)
        {
            UE_LOG(LogWeatherApp, Log,
                   TEXT("UWeatherAppBlueprintLibrary::GetForecastWeatherForCityById() BadRequest not executed"));
        }
        return;
    }

    FWeatherDataForecast5 Weather;

    const FString ResponseStr = HttpResponse->GetContentAsString();

    if (!FJsonObjectConverter::JsonObjectStringToUStruct<FWeatherDataForecast5>(
        *ResponseStr, &Weather, 0, 0))
    {
        const bool bResult = BadRequestCallback.ExecuteIfBound(
            *FString::Printf(TEXT("%d"), HttpResponse->GetResponseCode()), "");
        if (!bResult)
        {
            UE_LOG(LogWeatherApp, Log,
                   TEXT("UWeatherAppBlueprintLibrary::GetForecastWeatherForCityById() BadRequest not executed"));
        }
        return;
    }

    UDatabaseFunctionLibrary::InsertOrUpdateCityInfo(Weather);
    const bool bResult = ResponseCallback.ExecuteIfBound(Weather);
    if (!bResult)
    {
        UE_LOG(LogWeatherApp, Log,
               TEXT("UWeatherAppBlueprintLibrary::GetForecastWeatherForCityById() ResponseCallback not executed"));
    }
}

TSharedRef<IHttpRequest> UWeatherAppBlueprintLibrary::ConfigureRequest(const int CityId, const bool bGetCurrentTime)
{
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

    const FString AppID = "8a156ac591cd2f1478d6d5e3d38b2db6";

    FString Url;

    if (bGetCurrentTime)
    {
        // Example: https://api.openweathermap.org/data/2.5/weather?id=1486209&appid=8a156ac591cd2f1478d6d5e3d38b2db6
        Url = FString::Printf(
            TEXT("https://api.openweathermap.org/data/2.5/weather?id=%i&appid=%s"), CityId, *AppID);
    }
    else
    {
        // Example: https://api.openweathermap.org/data/2.5/forecast?id=1486209&appid=8a156ac591cd2f1478d6d5e3d38b2db6
        Url = FString::Printf(
            TEXT("https://api.openweathermap.org/data/2.5/forecast?id=%i&appid=%s"), CityId, *AppID);
    }

    HttpRequest->SetURL(Url);
    HttpRequest->SetVerb(TEXT("GET"));

    return HttpRequest;
}

FSearchCity UWeatherAppBlueprintLibrary::FindSearchCityById(const int CityId)
{
    for (auto SearchCity : SearchCitiesData)
    {
        if (SearchCity.Id == CityId)
        {
            return SearchCity;
        }
    }

    FSearchCity NotFoundCity;
    NotFoundCity.Id = -1;
    return NotFoundCity;
}

FDateTime UWeatherAppBlueprintLibrary::GetDatetimeFromInt(const int64 Time)
{
    return FDateTime::FromUnixTimestamp(Time);
}

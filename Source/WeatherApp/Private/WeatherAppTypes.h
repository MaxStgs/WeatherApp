// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeatherAppTypes.generated.h"

USTRUCT(BlueprintType)
struct FWeatherCityCoord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Longitude;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Latitude;
};

USTRUCT(BlueprintType)
struct FWeatherType
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Id;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Main;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Description;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Icon;
};

USTRUCT(BlueprintType)
struct FWeatherMain
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Temp;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float FeelsLike;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Temp_Min;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Temp_Max;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Pressure;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Humidity;
};

USTRUCT(BlueprintType)
struct FWeatherWind
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Speed;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Degrees;
};

USTRUCT(BlueprintType)
struct FWeatherRain
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float OneHour;
};

USTRUCT(BlueprintType)
struct FWeatherClouds
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int All;
};

USTRUCT(BlueprintType)
struct FWeatherSys
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Type;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Id;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Country;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Sunrise;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Sunset;
};

USTRUCT(BlueprintType)
struct FWeatherDataCurrent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherCityCoord Coord;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    TArray<FWeatherType> Weathers;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Base;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherMain Main;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Visibility;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherWind Wind;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherRain Rain;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherClouds Clouds;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Dt;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherSys Sys;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Timezone;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Id;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Cod;
};

USTRUCT(BlueprintType)
struct FWeatherSnow
{
    GENERATED_BODY()

    /** Snow volume for last 3 hours */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float ThreeHours;
};

// Search City

USTRUCT(BlueprintType)
struct FSearchCity
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Id;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Country;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherCityCoord CityCoord;

    FSearchCity() :
        Id(0), Name(""), Country(""), CityCoord(FWeatherCityCoord())
    {
    }
};

// Forecast5

/** Structure City for Forecast5 API */
USTRUCT(BlueprintType)
struct FCityForecast5
{
    GENERATED_BODY()

    /** City ID */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Id;

    /** City name */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Name;

    /** City coordinates */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherCityCoord Coord;

    /** Country Code (GB, JP etc.) */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Country;

    /** Shift in seconds from UTC */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Timezone;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Sunrise;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Sunset;
};

/**
 * Main data with temperature for Forecast5
 */
USTRUCT(BlueprintType)
struct FWeatherMainForecast5
{
    GENERATED_BODY()

    /** Temperature.
     *  Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Temp;

    /** Temperature.
     *  This temperature parameter accounts for the human perception of weather.
     *  Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Feels_Like;

    /** Minimum temperature at the moment of calculation.
     *  This is minimal forecasted temperature (within large megalopolises and urban areas), use this parameter optionally.
     *  Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Temp_Min;

    /** Maximum temperature at the moment of calculation.
     *  This is maximal forecasted temperature (within large megalopolises and urban areas),
     *  use this parameter optionally.
     *  Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Temp_Max;

    /** Atmospheric pressure on the sea level by default, hPa */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Pressure;

    /** Atmospheric pressure on the sea level, hPa */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Sea_Level;

    /** Atmospheric pressure on the ground level, hPa */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Grnd_Level;

    /** Humidity, % */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Humidity;

    /** API Internal Param */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    float Temp_Kf;
};

/**
 * Structure with date about Weather for some time
 */
USTRUCT(BlueprintType)
struct FWeatherForecast5
{
    GENERATED_BODY()

    /** Time of data forecasted, unix, UTC */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Dt;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherMainForecast5 Main;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    TArray<FWeatherType> Weather;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherClouds Clouds;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherWind Wind;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherSnow Snow;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FWeatherSys Sys;

    /** Time of data forecasted, ISO, UTC */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Dt_Txt;
};


/**
 * Structure-response from server from Forecast API
 */
USTRUCT(BlueprintType)
struct FWeatherDataForecast5
{
    GENERATED_BODY()

    /** Internal API parameter */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FString Cod;

    /** Internal API parameter */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Message;

    /** Count items from API */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    int Count;

    /** Items */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    TArray<FWeatherForecast5> List;

    /** City */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
    FCityForecast5 City;
};

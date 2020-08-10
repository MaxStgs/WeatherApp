// Fill out your copyright notice in the Description page of Project Settings.


#include "DatabaseFunctionLibrary.h"


#include "JsonObjectConverter.h"
#include "WeatherAppBlueprintLibrary.h"

FSQLiteDatabaseConnection UDatabaseFunctionLibrary::Database = FSQLiteDatabaseConnection();

void UDatabaseFunctionLibrary::OpenDatabaseConnection()
{
#if UE_EDITOR
    CreateDatabase();
#else
    if (!FPaths::FileExists(GetPathToDatabase()))
    {
        CreateDatabase();
    }
    else
    {
        Database.Open(*GetPathToDatabase(), TEXT(""), TEXT(""));
    }
#endif
}

void UDatabaseFunctionLibrary::CreateDatabase()
{
    Database.Open(*GetPathToDatabase(), TEXT(""), TEXT(""));

    // Create History Table
    const FString CreateHistoryQuery = "CREATE TABLE history (timestamp TEXT);";
    if (Database.Execute(*CreateHistoryQuery))
    {
        const FString Query = "INSERT INTO history VALUES (strftime('%s', 'now', 'localtime'))";
        Database.Execute(*Query);
    }

    // Weather table
    const FString CreateCitiesQuery = "CREATE TABLE Weather (city_id INTEGER PRIMARY KEY, json TEXT);";
    Database.Execute(*CreateCitiesQuery);

    // Need 2 base City - Moscow, Perm their Id's = 524894, 511196
    const FString Query = "CREATE TABLE Tracks (city_id INTEGER PRIMARY KEY);";
    const bool bResult = Database.Execute(*Query);
    if (bResult)
    {
        const TArray<int> BasicCitiesId = {524894, 511196};
        InsertTracks(BasicCitiesId);
    }
}

FString UDatabaseFunctionLibrary::GetPathToDatabase()
{
    return FPaths::ProjectDir() / "Extras" / "database.db";
}

void UDatabaseFunctionLibrary::CloseDatabaseConnection()
{
    Database.Close();
}

void UDatabaseFunctionLibrary::InsertTracks(const TArray<int> Tracks)
{
    FString Query = "INSERT INTO Tracks VALUES ";
    for (auto CityId : Tracks)
    {
        if (CityId < 0)
        {
            UE_LOG(LogDatabase, Error, TEXT("InsertTracks found element :%i"), CityId);
            continue;
        }
        const FSearchCity City = UWeatherAppBlueprintLibrary::FindSearchCityById(CityId);
        Query += FString::Printf(TEXT("(%i),"), City.Id);
    }
    Query.RemoveFromEnd(",");
    Query.Append(";");

    UE_LOG(LogTemp, Warning, TEXT("Execute: %s"), *Query);
    Database.Execute(*Query);
}

TArray<int> UDatabaseFunctionLibrary::GetAllTracks()
{
    const FString Query = "SELECT city_id FROM Tracks;";
    FSQLiteResultSet* ResultSet;
    Database.Execute(*Query, ResultSet);

    if (ResultSet == nullptr)
    {
        return TArray<int>();
    }

    TArray<int> Tracks;

    for (FDataBaseRecordSet::TIterator Iter(ResultSet); Iter; ++Iter)
    {
        int CityId = Iter->GetInt(TEXT("city_id"));
        Tracks.Add(CityId);
    }

    return Tracks;
}

void UDatabaseFunctionLibrary::RemoveTracks(const TArray<int> Tracks)
{
    for (auto CityId : Tracks)
    {
        const FString Query = *FString::Printf(TEXT("DELETE FROM Tracks WHERE city_id == %i"), CityId);
        UE_LOG(LogTemp, Warning, TEXT("Execute: %s"), *Query)
        RemoveWeatherByCityId(CityId);
        Database.Execute(*Query);
    }
}

void UDatabaseFunctionLibrary::UpdateHistory()
{
    // strftime('%s', 'now', 'localtime') is not working, using workaround
    const FString Query = FString::Printf(
        TEXT("UPDATE History SET timestamp = %lld;"), FDateTime::Now().ToUnixTimestamp());
    Database.Execute(*Query);
}

int UDatabaseFunctionLibrary::GetHistory()
{
    const FString Query = "SELECT timestamp FROM History;";
    FSQLiteResultSet* ResultSet;
    Database.Execute(*Query, ResultSet);
    if (ResultSet == nullptr)
    {
        return -1;
    }

    for (const FDataBaseRecordSet::TIterator Iter(ResultSet); Iter;)
    {
        return Iter->GetInt(TEXT("timestamp"));
    }
    return -1;
}

void UDatabaseFunctionLibrary::InsertOrUpdateCityInfo(const FWeatherDataForecast5& Data)
{
    FString DataString;
    if (!FJsonObjectConverter::UStructToJsonObjectString<FWeatherDataForecast5>(Data, DataString, 0, 0))
    {
        UE_LOG(LogDatabase, Error,
               TEXT(
                   "UDatabaseFunctionLibrary::AddCityInfo() FJsonObjectConverter::JsonObjectStringToUString return false"
               ));
        return;
    }
    const FString Query = FString::Printf(TEXT("REPLACE INTO Weather VALUES (%i, '%s')"), Data.City.Id, *DataString);
    UE_LOG(LogDatabase, Warning, TEXT("%s"), *Query);
    Database.Execute(*Query);
}

void UDatabaseFunctionLibrary::RemoveWeatherByCityId(const int CityId)
{
    const FString Query = *FString::Printf(TEXT("DELETE FROM Weather WHERE city_id == %i"), CityId);
    Database.Execute(*Query);
}

FWeatherDataForecast5 UDatabaseFunctionLibrary::GetWeatherByCityId(const int CityId)
{
    const FString Query = *FString::Printf(TEXT("SELECT json FROM Weather WHERE city_id == %i"), CityId);
    FSQLiteResultSet* ResultSet;
    Database.Execute(*Query, ResultSet);
    if (ResultSet == nullptr)
    {
        return {};
    }

    FString JsonString;
    for (const FDataBaseRecordSet::TIterator Iter(ResultSet); Iter;)
    {
        JsonString = Iter->GetString(TEXT("json"));
        break;
    }


    FWeatherDataForecast5 Weather;
    if (!FJsonObjectConverter::JsonObjectStringToUStruct<FWeatherDataForecast5>(*JsonString, &Weather, 0, 0))
    {
        UE_LOG(LogDatabase,
               Error, TEXT(
                   "UDatabaseFunctionLibrary::GetWeatherByCityId() FJsonObjectConverter::JsonObjectStringToUString return false"
               ));
        return {};
    }
    return Weather;
}

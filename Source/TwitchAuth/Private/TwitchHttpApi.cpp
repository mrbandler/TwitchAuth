// Copyright (c) 2018 fivefingergames.

#include "TwitchHttpApi.h"

FHttpModule* UTwitchHttpApi::Http = &FHttpModule::Get();

const FString UTwitchHttpApi::API_BASE_URL = "https://api.twitch.tv/kraken";
const FString UTwitchHttpApi::ACCESS_TOKEN_KEY = "access_token=";
const FString UTwitchHttpApi::ACCESS_TOKEN_URI_CONTAINS = "http://localhost/#access_token";

const FString UTwitchHttpApi::USER_ENDPOINT = "/user";
const FString UTwitchHttpApi::CHANNEL_ENDPOINT = "/users?login=";
const FString UTwitchHttpApi::SUBSCRIPTION_ENDPOINT = "/users/$1/subscriptions/$2";
const FString UTwitchHttpApi::FOLLOWING_ENDPOINT = "/users/$1/follows/channels/$2";

TSharedRef<IHttpRequest> UTwitchHttpApi::CreateHttpRequest(const FString& ClientId, const FString& AccessToken, const FString& Endpoint, EHttpVerb Verb)
{
    // First let's created the result.
    TSharedRef<IHttpRequest> result = Http->CreateRequest();

    // Set the endpoint URL and the authorization header.
    const FString url = UTwitchHttpApi::API_BASE_URL + Endpoint;
    result->SetURL(url);
    result->SetHeader(TEXT("Authorization"), TEXT("OAuth " + AccessToken));
    result->SetHeader(TEXT("Client-ID"), ClientId);
    result->SetHeader(TEXT("Accept"), TEXT("application/vnd.twitchtv.v5+json"));

    // Last but not least set the HTTP verb.
    FString verbStr = UTwitchHttpApi::GetHttpVerbStr(Verb);
    result->SetVerb(verbStr);

    // And finally return the thing.
    return result;
}

FString UTwitchHttpApi::ExtractAccessToken(const FString& AccessTokenUrl)
{
    FString result = "";

    // First find the hash that marks the access token and 
    // add the key string length to the found index.
    int32 leftIndex = AccessTokenUrl.Find("#");
    leftIndex += ACCESS_TOKEN_KEY.Len() + 1;

    int32 rightIndex = AccessTokenUrl.Find("&");

    int32 charCount = rightIndex - leftIndex;
    result = AccessTokenUrl.Mid(leftIndex, charCount);

    return result;
}

FString UTwitchHttpApi::GetAuthenticationUrl(const FString& ClientId, bool ForceVerify)
{
    FString Url = "https://id.twitch.tv/oauth2/authorize";
    Url += "?client_id=" + ClientId;
    Url += "&redirect_uri=http://localhost";
    Url += "&response_type=token";
    if(ForceVerify == true)
    {
        Url += "&force_varify=true";
    }
    Url += "&scope=user_read user_subscriptions";

    return Url;
}

FString UTwitchHttpApi::GetHttpVerbStr(EHttpVerb Verb)
{
    // Init the result.
    FString result = "";

    // Set the result variable based on the given enum value.
    switch(Verb)
    {
        case EHttpVerb::Get: result = "GET";
            break;
        case EHttpVerb::Post: result = "POST";
            break;
        case EHttpVerb::Put: result = "PUT";
            break;
        case EHttpVerb::Patch: result = "PATCH";
            break;
        case EHttpVerb::Delete: result = "DELETE";
            break;
        default:
            break;
    }

    // Finally return the result.
    return result;
}

bool UTwitchHttpApi::IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
    // First check the easy stuff. 
    if(bWasSuccessful == false || Response.IsValid() == false) return false;

    // If the response code is OK 200 return true.
    if(EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        return true;
    }
    else
    {
        // Otherwise we log a warning to the console and return false.
        UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
        return false;
    }
}

FString UTwitchHttpApi::ExtractTwitchChannelUserFromResponseBody(const FString& ResponseBody)
{
    int32 index = ResponseBody.Find("[") + 1;
    int32 charCount = (ResponseBody.Len() - 2) - index;
    return ResponseBody.Mid(index, charCount);
}
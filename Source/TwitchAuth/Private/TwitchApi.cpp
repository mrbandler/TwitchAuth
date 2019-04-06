// Copyright (c) 2018 fivefingergames.

#include "TwitchApi.h"

const FString UTwitchApi::API_BASE_URL = TEXT("https://api.twitch.tv/kraken");
const FString UTwitchApi::ACCESS_TOKEN_KEY = TEXT("access_token=");
const FString UTwitchApi::ACCESS_TOKEN_URI_CONTAINS = TEXT("http://localhost/#access_token");

const FString UTwitchApi::USER_ENDPOINT = TEXT("/user");
const FString UTwitchApi::CHANNEL_ENDPOINT = TEXT("/users?login=");
const FString UTwitchApi::SUBSCRIPTION_ENDPOINT = TEXT("/users/$1/subscriptions/$2");
const FString UTwitchApi::FOLLOWING_ENDPOINT = TEXT("/users/$1/follows/channels/$2");

TSharedRef<IHttpRequest> UTwitchApi::CreateHttpRequest(const FString& ClientId, const FString& AccessToken, const FString& Endpoint, ETwitchHttpVerb Verb)
{
    FHttpModule* http = &FHttpModule::Get();

    // First let's created the result.
    TSharedRef<IHttpRequest> result = http->CreateRequest();

    // Set the endpoint URL and the authorization header.
    const FString url = UTwitchApi::API_BASE_URL + Endpoint;
    result->SetURL(url);
    result->SetHeader(TEXT("Authorization"), TEXT("OAuth " + AccessToken));
    result->SetHeader(TEXT("Client-ID"), ClientId);
    result->SetHeader(TEXT("Accept"), TEXT("application/vnd.twitchtv.v5+json"));

    // Last but not least set the HTTP verb.
    FString verbStr = UTwitchApi::GetHttpVerbStr(Verb);
    result->SetVerb(verbStr);

    // And finally return the thing.
    return result;
}

FString UTwitchApi::ExtractAccessToken(const FString& AccessTokenUrl)
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

FString UTwitchApi::GetAuthenticationUrl(const FString& ClientId, bool ForceVerify)
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

FString UTwitchApi::GetHttpVerbStr(ETwitchHttpVerb Verb)
{
    // Init the result.
    FString result = "";

    // Set the result variable based on the given enum value.
    switch(Verb)
    {
        case ETwitchHttpVerb::Get: result = "GET";
            break;
        case ETwitchHttpVerb::Post: result = "POST";
            break;
        case ETwitchHttpVerb::Put: result = "PUT";
            break;
        case ETwitchHttpVerb::Patch: result = "PATCH";
            break;
        case ETwitchHttpVerb::Delete: result = "DELETE";
            break;
        default:
            break;
    }

    // Finally return the result.
    return result;
}

bool UTwitchApi::IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful)
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

FString UTwitchApi::ExtractTwitchChannelUserFromResponseBody(const FString& ResponseBody)
{
    int32 index = ResponseBody.Find("[") + 1;
    int32 charCount = (ResponseBody.Len() - 2) - index;
    return ResponseBody.Mid(index, charCount);
}



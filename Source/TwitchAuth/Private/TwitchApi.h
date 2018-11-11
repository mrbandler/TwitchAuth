// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "TwitchApi.generated.h"

/**
* @enum    EHttpVerb
*
* @brief   Values that represent HTTP verbs.
*/
UENUM()
enum class EHttpVerb : uint8
{
    Get,
    Post,
    Put,
    Patch,
    Delete
};

/**
* @enum    EEndpoint
*
* @brief   Values that represent Twitch API endpoints.
*/
UENUM()
enum class EEndpoint : uint8
{
    None,
    User,
    Channels,
    Subscriptions,
    Following
};

/**
* @enum    ETwitchRequest
*
* @brief   Values that represent different request that can be made to the Twitch API.
*/
UENUM()
enum class ETwitchRequest : uint8
{
    None,
    Subscription,
    Following
};

/**
 * 
 */
UCLASS()
class TWITCHAUTH_API UTwitchApi : public UObject
{
	GENERATED_BODY()
	
public:

    /** @brief   Base URL of the Twitch HTTP API. */
    static const FString API_BASE_URL;

    /** @brief   The access token key within the access token URI. */
    static const FString ACCESS_TOKEN_KEY;

    /** @brief   Substring contained in the redirect URI. */
    static const FString ACCESS_TOKEN_URI_CONTAINS;


    /** @brief   The Twitch user API endpoint. */
    static const FString USER_ENDPOINT;

    /** @brief   The Twitch channel API endpoint */
    static const FString CHANNEL_ENDPOINT;

    /** @brief   The Twitch subscription API endpoint */
    static const FString SUBSCRIPTION_ENDPOINT;

    /** @brief   The Twitch following API endpoint */
    static const FString FOLLOWING_ENDPOINT;

    /**
    * @fn  static FString UTwitchHttpApi::ExtractTwitchChannelUserFromResponseBody(const FString& ResponseBody);
    *
    * @brief   Extracts the Twitch channel user from a given response body.
    *
    * @param   ResponseBody    The response body.
    *
    * @return  The extracted twitch channel user from response body.
    */
    static FString ExtractTwitchChannelUserFromResponseBody(const FString& ResponseBody);

    /**
    * @fn  static FString UTwitchHttpApi::GetAuthenticationUrl(const FString& ClientId, bool ForceVerify);
    *
    * @brief   Returns authentication URL.
    *
    * @param   ClientId    Identifier for the client.
    * @param   ForceVerify Flag, if the authentication should be verified again.
    *
    * @return  The authentication URL.
    */
    static FString GetAuthenticationUrl(const FString& ClientId, bool ForceVerify);

    /**
    * @fn  static FString UTwitchHttpApi::ExtractAccessToken(const FString& RedirectUri);
    *
    * @brief   Extracts the access token from a given redirect URI.
    *
    * @param   RedirectUri URI.
    *
    * @return  The extracted access token.
    */
    static FString ExtractAccessToken(const FString& RedirectUri);

    /**
    * @fn  static FString UTwitchHttpApi::GetHttpVerbStr(EHttpVerb Verb);
    *
    * @brief   Gets HTTP verb string.
    *
    * @param   Verb    The verb.
    *
    * @return  The HTTP verb string.
    */
    static FString GetHttpVerbStr(EHttpVerb Verb);

    /**
    * @fn  static bool UTwitchHttpApi::IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful);
    *
    * @brief   Checks if a response is valid
    *
    * @param   Response        The response.
    * @param   bWasSuccessful  True if the operation was a success, false if it failed.
    *
    * @return  True if the response is valid, false if not.
    */
    static bool IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful);

    /**
    * @fn  static TSharedRef<IHttpRequest> UTwitchHttpApi::CreateHttpRequest(const FString& ClientId, const FString& AccessToken, const FString& Endpoint, EHttpVerb Verb);
    *
    * @brief   Creates HTTP request.
    *
    * @param   ClientId    Identifier for the client.
    * @param   AccessToken The access token.
    * @param   Endpoint    The HTTP endpoint.
    * @param   Verb        The HTTP verb.
    *
    * @return  The new HTTP request.
    */
    static TSharedRef<IHttpRequest> CreateHttpRequest(const FString& ClientId, const FString& AccessToken, const FString& Endpoint, EHttpVerb Verb);
	
};

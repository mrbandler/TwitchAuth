// Copyright (c) 2018 fivefingergames.

#include "TwitchAuthComponent.h"
#include "Engine.h"
#include "CoreMinimal.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "WebBrowser.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Private/TwitchHttpApi.h"
#include "TwitchAuthTypes.h"

DEFINE_LOG_CATEGORY(LogTwitchAuth);

UTwitchAuthComponent::UTwitchAuthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma region Public

#pragma region Blueprint Interaction

FTwitchUser UTwitchAuthComponent::GetUser() const
{
    return m_TwitchUser;
}

void UTwitchAuthComponent::Authenticate(UWebBrowser* WebBrowser)
{
    if(m_AccessToken != "")
    {
        ExecuteGetUserRequest();
    }
    else
    {
        if(WebBrowser)
        {
            m_WebBrowser = WebBrowser;
            FString url = UTwitchHttpApi::GetAuthenticationUrl(ClientId, bForceVerify);
            m_WebBrowser->OnUrlChanged.AddDynamic(this, &UTwitchAuthComponent::HandleOnUrlChanged);
            m_WebBrowser->LoadURL(url);
        }
        else
        {
            FTwitchError error;
            error.status = 404;
            error.error = "Browser Widget not found";
            error.message = "Browser widget could not be found";
            m_LastError = error;

            LogError(m_LastError);
        }
    }
}

void UTwitchAuthComponent::IsUserSubscribedToChannel(const FString& ChannelName)
{
    m_TwitchRequest = ETwitchRequest::Subscription;
    ExecuteGetChannelRequest(ChannelName);
}

void UTwitchAuthComponent::IsUserFollowingChannel(const FString& ChannelName)
{
    m_TwitchRequest = ETwitchRequest::Following;
    ExecuteGetChannelRequest(ChannelName);
}

FString UTwitchAuthComponent::GetAccessToken() const
{
    return m_AccessToken;
}

void UTwitchAuthComponent::SetAccessToken(const FString& AccessToken)
{
    m_AccessToken = AccessToken;
}

void UTwitchAuthComponent::ClearAccessToken()
{
    m_AccessToken = "";
}

FTwitchError UTwitchAuthComponent::GetLastError() const
{
    return m_LastError;
}

UWebBrowser* UTwitchAuthComponent::GetWebBrowser() const
{
    return m_WebBrowser;
}

#pragma endregion // Blueprint Interaction

#pragma endregion // Public

#pragma region Protected

void UTwitchAuthComponent::LogError(const FTwitchError& twitchError) const
{
    FString logMessage = twitchError.error + "(" + FString::FromInt(twitchError.status) + "): " + twitchError.message;
    UE_LOG(LogTwitchAuth, Error, TEXT("%s"), *logMessage);
}

#pragma endregion // Protected

#pragma region Private

#pragma  region Web Browser Widget

void UTwitchAuthComponent::HandleOnUrlChanged(const FText& InText)
{
    FString url = InText.ToString();
    if(url.Contains(UTwitchHttpApi::ACCESS_TOKEN_URI_CONTAINS) == true)
    {
        OnAccessTokenPageLoaded.Broadcast();
        m_AccessToken = UTwitchHttpApi::ExtractAccessToken(url);
        ExecuteGetUserRequest();
    }
    else
    {
        OnAuthenticationPageLoaded.Broadcast();
    }
}

#pragma endregion //Web Browser Widget

#pragma region Twitch HTTP API

void UTwitchAuthComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // When the response comes in we first check if it is valid.
    if(UTwitchHttpApi::IsResponseValid(Response, bWasSuccessful) == true)
    {
        // Based on the last endpoint we send a request to we handle the response.
        switch(m_LastEndpoint)
        {
            case EEndpoint::User:
                m_LastEndpoint = EEndpoint::None;

                HandleGetUserResponse(Request, Response);
                break;

            case EEndpoint::Channels:
                m_LastEndpoint = EEndpoint::None;

                HandleGetChannelResponse(Request, Response);
                break;

            case EEndpoint::Subscriptions:
                m_LastEndpoint = EEndpoint::None;

                HandleCheckUserSubscriptionResponse(Request, Response);
                break;

            case EEndpoint::Following:
                m_LastEndpoint = EEndpoint::None;

                HandleCheckUserFollowingResponse(Request, Response);
                break;

            default:
                break;
        }
    }
    else
    {
        FString responseBody = Response->GetContentAsString();
        FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchError>(responseBody, &m_LastError, 0, 0);

        switch(m_LastEndpoint)
        {
            case EEndpoint::Following:
                LogError(m_LastError);
                OnUserFollowingChannel.Broadcast(false, m_TwitchFollow);
                break;

            case EEndpoint::Subscriptions:
                LogError(m_LastError);
                OnUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
                break;

            case EEndpoint::Channels:
                LogError(m_LastError);
                OnUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
                break;

            case EEndpoint::User:
                LogError(m_LastError);
                OnUserAuthenticated.Broadcast(false);
                break;
        }
    }
}

#pragma endregion // HTTP API

#pragma region Twitch API Endpoints

void UTwitchAuthComponent::ExecuteGetUserRequest()
{
    // Create a request with the correct endpoint.
    TSharedRef<IHttpRequest> request = UTwitchHttpApi::CreateHttpRequest(ClientId, m_AccessToken, UTwitchHttpApi::USER_ENDPOINT, EHttpVerb::Get);
    request->OnProcessRequestComplete().BindUObject(this, &UTwitchAuthComponent::OnResponseReceived);

    // Set the last endpoint so that the response handler knows what to do.
    m_LastEndpoint = EEndpoint::User;

    // Send the request.
    request->ProcessRequest();
}

void UTwitchAuthComponent::HandleGetUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    // Get the response body as a string.
    const FString responseBody = Response->GetContentAsString();

    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchUser>(responseBody, &m_TwitchUser, 0, 0) == true)
    {
        OnUserAuthenticated.Broadcast(true);
    }
    else
    {
        OnUserAuthenticated.Broadcast(false);
    }
}

void UTwitchAuthComponent::ExecuteGetChannelRequest(const FString& ChannelName)
{
    const FString endpoint = UTwitchHttpApi::CHANNEL_ENDPOINT + ChannelName;
    TSharedRef<IHttpRequest> request = UTwitchHttpApi::CreateHttpRequest(ClientId, m_AccessToken, endpoint, EHttpVerb::Get);
    request->OnProcessRequestComplete().BindUObject(this, &UTwitchAuthComponent::OnResponseReceived);

    m_LastEndpoint = EEndpoint::Channels;

    request->ProcessRequest();
}

void UTwitchAuthComponent::HandleGetChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    // Get the response body as a string.
    FString responseBody = Response->GetContentAsString();
    responseBody = UTwitchHttpApi::ExtractTwitchChannelUserFromResponseBody(responseBody);

    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchChannelUser>(responseBody, &m_TwitchChannelUser, 0, 0) == true)
    {
        switch(m_TwitchRequest)
        {
            case ETwitchRequest::Subscription: ExecuteCheckUserSubscriptionRequest(m_TwitchUser, m_TwitchChannelUser);
                break;
            case ETwitchRequest::Following: ExecuteCheckUserFollowingRequest(m_TwitchUser, m_TwitchChannelUser);
                break;
        }

        m_TwitchRequest = ETwitchRequest::None;
    }
    else
    {
        FTwitchError twitchError;
        twitchError.error = "Not Found";
        twitchError.status = 404;
        twitchError.message = "Channel could not be found";

        m_LastError = twitchError;

        LogError(m_LastError);
        OnUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
    }
}

void UTwitchAuthComponent::ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel)
{
    FString endpoint = UTwitchHttpApi::SUBSCRIPTION_ENDPOINT;
    endpoint = endpoint.Replace(TEXT("$1"), *TwitchUser._id);
    endpoint = endpoint.Replace(TEXT("$2"), *TwitchChannel._id);

    TSharedRef<IHttpRequest> request = UTwitchHttpApi::CreateHttpRequest(ClientId, m_AccessToken, endpoint, EHttpVerb::Get);
    request->OnProcessRequestComplete().BindUObject(this, &UTwitchAuthComponent::OnResponseReceived);

    m_LastEndpoint = EEndpoint::Subscriptions;

    request->ProcessRequest();
}

void UTwitchAuthComponent::HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    const FString responseBody = Response->GetContentAsString();

    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchSubscription>(responseBody, &m_TwitchSubscription, 0, 0) == true)
    {
        OnUserSubscribedToChannel.Broadcast(true, m_TwitchSubscription);
    }
}

void UTwitchAuthComponent::ExecuteCheckUserFollowingRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel)
{
    FString endpoint = UTwitchHttpApi::FOLLOWING_ENDPOINT;
    endpoint = endpoint.Replace(TEXT("$1"), *TwitchUser._id);
    endpoint = endpoint.Replace(TEXT("$2"), *TwitchChannel._id);

    TSharedRef<IHttpRequest> request = UTwitchHttpApi::CreateHttpRequest(ClientId, m_AccessToken, endpoint, EHttpVerb::Get);
    request->OnProcessRequestComplete().BindUObject(this, &UTwitchAuthComponent::OnResponseReceived);

    m_LastEndpoint = EEndpoint::Following;

    request->ProcessRequest();
}

void UTwitchAuthComponent::HandleCheckUserFollowingResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    const FString responseBody = Response->GetContentAsString();

    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchFollow>(responseBody, &m_TwitchFollow, 0, 0) == true)
    {
        OnUserFollowingChannel.Broadcast(true, m_TwitchFollow);
    }
}

#pragma endregion // Twitch API Endpoints

#pragma endregion // Private
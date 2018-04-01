// Copyright (c) 2018 fivefingergames.

#include "TwitchAuthActor.h"
#include "CoreMinimal.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/WebBrowser/Public/SWebBrowser.h"
#include "Widgets/SWeakWidget.h" 
#include "Widget.h"
#include "Json.h"
#include "JsonUtilities.h"

ATwitchAuthActor::ATwitchAuthActor()
{
    // This actor does not need to tick.
    PrimaryActorTick.bCanEverTick = false;

    // Init member variables.
    m_Http = &FHttpModule::Get();
}

#pragma region UE4 Overrides
    // Nothing to do here...
#pragma endregion // UE4 Overrides

#pragma region Public

#pragma region Blueprint Interaction

/************************************************************************/
/*                                                                      */
/************************************************************************/
FTwitchUser ATwitchAuthActor::GetSignedInTwitchUser()
{
    return m_TwitchUser;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::StartUserSignIn()
{
    if(m_AccessToken != "")
    {
        ExecuteGetTwitchUserRequest();
    }
    else 
    {  
        TSharedPtr<SWidget> widget = CreateWebBrowserWidget();
        AddWidgetToViewport(widget);
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::IsTwitchUserSubscribedToChannel(FTwitchUser TwitchUser, FString ChannelName)
{
    ExecuteGetTwitchChannelRequest(ChannelName);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

FTwitchError ATwitchAuthActor::GetLastError()
{
    return m_LastError;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
FString ATwitchAuthActor::GetAccessToken()
{
    return m_AccessToken;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::SetAccessToken(FString AccessToken)
{
    m_AccessToken = AccessToken;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::ClearAccessToken()
{
    m_AccessToken = "";
}

#pragma endregion // Blueprint Interaction

#pragma endregion // Public

#pragma region Protected

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::LogError(const FTwitchError& twitchError)
{
    FString logMessage = twitchError.error + "(" + FString::FromInt(twitchError.status) + "): " + twitchError.message;
    UE_LOG(LogTemp, Warning, TEXT("%s"), *logMessage);
}

#pragma endregion // Protected

#pragma region Private

#pragma  region Web Browser Widget

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::EnableCursor(bool bEnable)
{
    const UWorld* world = GetWorld();
    if (world != nullptr)
    {
        APlayerController* playerController = world->GetFirstPlayerController();
        playerController->bShowMouseCursor = bEnable;
        playerController->bEnableClickEvents = bEnable;
        playerController->bEnableMouseOverEvents = bEnable;
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::AddWidgetToViewport(TSharedPtr<SWidget> Widget)
{
    EnableCursor(true);
    WeakWidget = SNew(SWeakWidget).PossiblyNullContent(Widget.ToSharedRef());
    GEngine->GameViewport->AddViewportWidgetContent(WeakWidget.ToSharedRef());
    Widget->SetVisibility(EVisibility::Visible);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::RemoveWidgetFromViewport(TSharedPtr<SWidget> Widget)
{
    EnableCursor(false);
    GEngine->GameViewport->RemoveViewportWidgetContent(Widget.ToSharedRef());
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
TSharedPtr<SWidget> ATwitchAuthActor::CreateWebBrowserWidget()
{
    FString url = GetTwitchSigninUrl();
    WebBrowserWidget = SNew(SWebBrowser)
        .InitialURL(url)
        .ShowControls(false)
        .SupportsTransparency(true)
        .OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged));
    
    return WebBrowserWidget;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::HandleOnUrlChanged(const FText & InText)
{
    FString url = InText.ToString();
    if(url.Contains(AccessTokenUriContainsStr) == true)
    {
        m_AccessToken = GetAccessToken(url);
        RemoveWidgetFromViewport(WeakWidget);
        ExecuteGetTwitchUserRequest();
    }
    else
    {
        OnSignInPageLoaded.Broadcast();
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
FString ATwitchAuthActor::GetAccessToken(const FString & AccessTokenUrl)
{
    FString result = "";

    // First find the hash that marks the access token and 
    // add the key string length to the found index.
    int32 leftIndex = AccessTokenUrl.Find("#");
    leftIndex += AccessTokenKey.Len() + 1;

    int32 rightIndex = AccessTokenUrl.Find("&");

    int32 charCount = rightIndex - leftIndex;
    result = AccessTokenUrl.Mid(leftIndex, charCount);

    return result;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
FString ATwitchAuthActor::GetTwitchSigninUrl()
{
    FString Url = "https://id.twitch.tv/oauth2/authorize";
    Url += "?client_id=" + ClientID;
    Url += "&redirect_uri=https://localhost";
    Url += "&response_type=token";
    if(ForceVerify == true) 
    {
        Url += "&force_varify=true";
    }
    Url += "&scope=user_read user_subscriptions";

    return Url;
}

#pragma endregion //Web Browser Widget

#pragma region Twitch HTTP API

/************************************************************************/
/*                                                                      */
/************************************************************************/
TSharedRef<IHttpRequest> ATwitchAuthActor::CreateHttpRequest(FString Endpoint, EHttpVerb Verb)
{ 
    // First let's created the result.
    TSharedRef<IHttpRequest> result = m_Http->CreateRequest();

    // Set the endpoint URL and the authorization header.
    FString url = m_ApiBaseUrl + Endpoint;
    result->SetURL(url);
    result->SetHeader(TEXT("Authorization"), TEXT("OAuth " + m_AccessToken));
    result->SetHeader(TEXT("Client-ID"), ClientID);
    result->SetHeader(TEXT("Accept"), TEXT("application/vnd.twitchtv.v5+json"));

    // Last but not least set the HTTP verb.
    FString verbStr = GetHttpVerbStr(Verb);
    result->SetVerb(verbStr);

    // In the final touch we bind the callback function to recieve the response.
    result->OnProcessRequestComplete().BindUObject(this, &ATwitchAuthActor::OnResponseReceived);

    // And finally return the thing.
    return result;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
FString ATwitchAuthActor::GetHttpVerbStr(EHttpVerb Verb)
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool ATwitchAuthActor::IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful)
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // When the response comes in we first check if it is valid.
    if(IsResponseValid(Response, bWasSuccessful) == true)
    {
        // Based on the last endpoint we send a request to we handle the response.
        switch(m_LastEndpoint)
        {
            case EEndpoint::User: 
                m_LastEndpoint = EEndpoint::None;

                HandleGetTwitchUserResponse(Request, Response);
                break;

            case EEndpoint::Channels:
                m_LastEndpoint = EEndpoint::None;

                HandleGetTwitchChannelResponse(Request, Response);
                break;

            case EEndpoint::Subscriptions:
                m_LastEndpoint = EEndpoint::None;

                HandleCheckUserSubscriptionResponse(Request, Response);
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
            case EEndpoint::Subscriptions:
                LogError(m_LastError);
                OnTwitchUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
                break;

            case EEndpoint::Channels:
                LogError(m_LastError);
                OnTwitchUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
                break;

            case EEndpoint::User:
                LogError(m_LastError);
                OnUserSignedIn.Broadcast(false);
                break;
        }
    }
}

#pragma endregion // HTTP API

#pragma region Twitch API Endpoints

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::ExecuteGetTwitchUserRequest()
{
    // Create a request with the correct endpoint.
    TSharedRef<IHttpRequest> request = CreateHttpRequest(m_UserEndpoint, EHttpVerb::Get);

    // Set the last endpoint so that the response handler knows what to do.
    m_LastEndpoint = EEndpoint::User;

    // Send the request.
    request->ProcessRequest();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::HandleGetTwitchUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    // Get the response body as a string.
    const FString responseBody = Response->GetContentAsString();

    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchUser>(responseBody, &m_TwitchUser, 0, 0) == true)
    {
        OnUserSignedIn.Broadcast(true);
    }
    else
    {
        OnUserSignedIn.Broadcast(false);
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::ExecuteGetTwitchChannelRequest(FString ChannelName)
{
    const FString endpoint = m_ChannelEndpoint + ChannelName;
    TSharedRef<IHttpRequest> request = CreateHttpRequest(endpoint, EHttpVerb::Get);
    m_LastEndpoint = EEndpoint::Channels;
    request->ProcessRequest();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::HandleGetTwitchChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    // Get the response body as a string.
    FString responseBody = Response->GetContentAsString();
    responseBody = RetrieveTwitchChannelUserFromResponseBody(responseBody);

    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchChannelUser>(responseBody, &m_TwitchChannelUser, 0, 0) == true)
    {
        ExecuteCheckUserSubscriptionRequest(m_TwitchUser, m_TwitchChannelUser);
    }
    else
    {
        FTwitchError twitchError;
        twitchError.error = "Not Found";
        twitchError.status = 404;
        twitchError.message = "Channel could not be found";

        m_LastError = twitchError;

        LogError(m_LastError);
        OnTwitchUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel)
{
    FString endpoint = m_SubscriptionEndpoint;
    endpoint = endpoint.Replace(TEXT("$1"), *TwitchUser._id);
    endpoint = endpoint.Replace(TEXT("$2"), *TwitchChannel._id);

    TSharedRef<IHttpRequest> request = CreateHttpRequest(endpoint, EHttpVerb::Get);
    m_LastEndpoint = EEndpoint::Subscriptions;
    request->ProcessRequest();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ATwitchAuthActor::HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    const FString responseBody = Response->GetContentAsString();

    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchSubscription>(responseBody, &m_TwitchSubscription, 0, 0) == true)
    {
        OnTwitchUserSubscribedToChannel.Broadcast(true, m_TwitchSubscription);
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
FString ATwitchAuthActor::RetrieveTwitchChannelUserFromResponseBody(const FString& ResponseBody) const
{
    int32 index     = ResponseBody.Find("[") + 1;
    int32 charCount = (ResponseBody.Len() - 2) - index;
    return ResponseBody.Mid(index, charCount);
}

#pragma endregion // Twitch API Endpoints

#pragma endregion // Private



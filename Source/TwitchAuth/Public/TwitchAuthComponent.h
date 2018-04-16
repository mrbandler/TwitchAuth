// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TwitchAuthTypes.h"
#include "WebBrowser.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Private/TwitchHttpApi.h"
#include "TwitchAuthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAuthenticationPageLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAccessTokenPageLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserAuthenticated, bool, bAuthenticated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUserSubscribedToChannel, bool, bSubscribed, FTwitchSubscription, TwitchSubscription);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
/**
 * @class   TWITCHAUTH_API TwitchAuthComponent.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthComponent.h
 *
 * @brief   Twitch authentication actor component.
 */
class TWITCHAUTH_API UTwitchAuthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

    /**
     * @fn  UTwitchAuthComponent::UTwitchAuthComponent();
     *
     * @brief   Default constructor.
     */
	UTwitchAuthComponent();

    /** Client ID from your Twitch application. */
    UPROPERTY(EditDefaultsOnly, Category = "Twitch Auth")
    FString ClientId;

    /** Flag, to force the authentication verification every time the authentication runs. */
    UPROPERTY(EditDefaultsOnly, Category = "Twitch Auth")
    bool bForceVerify = true;

    /** Will be fired when the authentication process finishes. */
    UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
    FOnUserAuthenticated OnUserAuthenticated;

    /** Will be fired when the subscription verification process finishes. */
    UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
    FOnUserSubscribedToChannel OnUserSubscribedToChannel;

    /** Will be fired when the authentication page has finished loading in the given browser. */
    UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
    FOnAuthenticationPageLoaded OnAuthenticationPageLoaded;

    UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
    /** Will be fired when the access token page has finished loading in the given browser. */
    FOnAccessTokenPageLoaded OnAccessTokenPageLoaded;

    /**
     * @fn  void UTwitchAuthComponent::Authenticate(UWebBrowser* WebBrowser);
     *
     * @brief   Starts the authentication process with a given browser widget.
     *
     * @param [in,out]  WebBrowser  Web browser that will be used to display the Twitch authentication web page.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth", meta = (ToolTip = "Start the authentication process with a given browser widget."))
    void Authenticate(UWebBrowser* WebBrowser);

    /**
     * @fn  FTwitchUser UTwitchAuthComponent::GetUser() const;
     *
     * @brief   Returns the authenticated Twitch user.
     *
     * @return  The authenticated Twitch user.
     */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth", meta = (ToolTip = "Returns the authenticated Twitch user."))
    FTwitchUser GetUser() const;

    /**
     * @fn  void UTwitchAuthComponent::IsUserSubscribedToChannel(const FString& ChannelName);
     *
     * @brief   Check if the authenticated Twitch user is subscribed to a given channel.
     *
     * @param   ChannelName Name of the channel.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth", meta = (ToolTip = "Check if the authenticated Twitch user is subscribed to a given channel."))
    void IsUserSubscribedToChannel(const FString& ChannelName);

    /**
     * @fn  FString UTwitchAuthComponent::GetAccessToken() const;
     *
     * @brief   Returns the retrieved access token from the authentication process.
     *
     * @return  The access token.
     */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth", meta = (ToolTip = "Returns the retrieved access token from the authentication process."))
    FString GetAccessToken() const;

    /**
     * @fn  void UTwitchAuthComponent::SetAccessToken(const FString& AccessToken);
     *
     * @brief   Sets the access token to be used in the authentication process.
     *
     * @param   AccessToken The access token.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth", meta = (ToolTip = "Sets the access token to be used in the authentication process."))
    void SetAccessToken(const FString& AccessToken);

    /**
     * @fn  void UTwitchAuthComponent::ClearAccessToken();
     *
     * @brief   Clears the access token.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth", meta = (ToolTip = "Clears the access token."))
    void ClearAccessToken();

    /**
     * @fn  FTwitchError UTwitchAuthComponent::GetLastError() const;
     *
     * @brief   Returns the last error.
     *
     * @return  The last error.
     */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth", meta = (ToolTip = "Returns the last error."))
    FTwitchError GetLastError() const;

    /**
     * @fn  UWebBrowser* UTwitchAuthComponent::GetWebBrowser() const;
     *
     * @brief   Returns the used web browser.
     *
     * @return  Null if it fails, else the web browser.
     */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth", meta = (ToolTip = "Returns the used web browser."))
    UWebBrowser* GetWebBrowser() const;

protected:

    #pragma region Blueprint Interaction

    /** The last error */
    FTwitchError m_LastError;

    /** The access token */
    FString m_AccessToken;

    #pragma endregion // Blueprint Interaction

    /**
     * @fn  void UTwitchAuthComponent::LogError(const FTwitchError& twitchError) const;
     *
     * @brief   Logs an given error.
     *
     * @param   twitchError The twitch error.
     */
    void LogError(const FTwitchError& twitchError) const;

    #pragma region HTTP API

    /**
     * @fn  void UTwitchAuthComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
     *
     * @brief   Will be called when every a request sends back an response. This is the place to handle any incoming responses.
     *
     * @param   Request         The request.
     * @param   Response        The response.
     * @param   bWasSuccessful  True if the was operation was a success, false if it failed.
     */
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    #pragma endregion // HTTP API;

private:

    #pragma region Web Browser Widget

    /** @brief   The web browser that is used to display the authentication pages. */
    UWebBrowser* m_WebBrowser;

    /**
     * @fn  void UTwitchAuthComponent::HandleOnUrlChanged(const FText& InText);
     *
     * @brief   Handles the on URL change from the web browser.
     *
     * @param   InText  The new URL.
     */
    UFUNCTION()
    void HandleOnUrlChanged(const FText& InText);

    #pragma endregion // Web Browser Widget

    #pragma region Twitch API Endpoints

    /** The last endpoint a request was send to. */
    EEndpoint m_LastEndpoint = EEndpoint::None;

    /** The authenticated Twitch user from the Authentication process. */
    FTwitchUser m_TwitchUser;

    /** The Twitch channel from the IsUserSubscribedToChannel process. */
    FTwitchChannelUser m_TwitchChannelUser;

    /** The Twitch subscription from the IsUserSubscribedToChannel process. */
    FTwitchSubscription m_TwitchSubscription;

    /**
     * @fn  void UTwitchAuthComponent::ExecuteGetUserRequest();
     *
     * @brief   Executes the get user request.
     */
    void ExecuteGetUserRequest();

    /**
     * @fn  void UTwitchAuthComponent::HandleGetUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
     *
     * @brief   Handles the get user response
     *
     * @param   Request     The request.
     * @param   Response    The response.
     */
    void HandleGetUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    /**
     * @fn  void UTwitchAuthComponent::ExecuteGetChannelRequest(const FString& ChannelName);
     *
     * @brief   Executes the get channel request.
     *
     * @param   ChannelName Name of the channel.
     */
    void ExecuteGetChannelRequest(const FString& ChannelName);

    /**
     * @fn  void UTwitchAuthComponent::HandleGetChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
     *
     * @brief   Handles the get channel response.
     *
     * @param   Request     The request.
     * @param   Response    The response.
     */
    void HandleGetChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    /**
     * @fn  void UTwitchAuthComponent::ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel);
     *
     * @brief   Executes the check user subscription request.
     *
     * @param   TwitchUser      The twitch user.
     * @param   TwitchChannel   The twitch channel.
     */
    void ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel);

    /**
     * @fn  void UTwitchAuthComponent::HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
     *
     * @brief   Handles the check user subscription response.
     *
     * @param   Request     The request.
     * @param   Response    The response.
     */
    void HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    #pragma endregion // Twitch API Endpoints

};

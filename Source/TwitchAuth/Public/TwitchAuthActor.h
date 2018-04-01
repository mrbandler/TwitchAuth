// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "TwitchAuthActor.generated.h"

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
 * @brief   Values that represent all possible endpoints to the Twitch API.
 */
UENUM()
enum class EEndpoint : uint8
{
    None,
    User,
    Channels,
    Subscriptions
};

/**
 * @struct  TWITCHAUTH_API TwitchAuthActor.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthActor.h
 *
 * @brief   Twitch user notification settings.
 */
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchUserNotifications
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) bool push;
    UPROPERTY(BlueprintReadOnly) bool email;

    FTwitchUserNotifications() {}
};

/**
 * @struct  TWITCHAUTH_API TwitchAuthActor.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthActor.h
 *
 * @brief   Twich channel user.
 */
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchChannelUser
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString display_name;
    UPROPERTY(BlueprintReadOnly) FString _id;
    UPROPERTY(BlueprintReadOnly) FString name;
    UPROPERTY(BlueprintReadOnly) FString type;
    UPROPERTY(BlueprintReadOnly) FString bio;
    UPROPERTY(BlueprintReadOnly) FString created_at;
    UPROPERTY(BlueprintReadOnly) FString updated_at;
    UPROPERTY(BlueprintReadOnly) FString logo;

    FTwitchChannelUser() {}
};

/**
 * @struct  TWITCHAUTH_API TwitchAuthActor.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthActor.h
 *
 * @brief   Normal Twitch user.
 */
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchUser : public FTwitchChannelUser
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString email;
    UPROPERTY(BlueprintReadOnly) bool email_verified;
    UPROPERTY(BlueprintReadOnly) bool partnered;
    UPROPERTY(BlueprintReadOnly) bool twitter_connected;
    UPROPERTY(BlueprintReadOnly) FTwitchUserNotifications notifications;

    FTwitchUser() {}
};

/**
 * @struct  TWITCHAUTH_API TwitchAuthActor.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthActor.h
 *
 * @brief   Twitch channel.
 */
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchChannel
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) bool mature;
    UPROPERTY(BlueprintReadOnly) FString status;
    UPROPERTY(BlueprintReadOnly) FString broadcaster_language;
    UPROPERTY(BlueprintReadOnly) FString game;
    UPROPERTY(BlueprintReadOnly) FString language;
    UPROPERTY(BlueprintReadOnly) FString name;
    UPROPERTY(BlueprintReadOnly) FString created_at;
    UPROPERTY(BlueprintReadOnly) FString updated_at;
    UPROPERTY(BlueprintReadOnly) FString _id;
    UPROPERTY(BlueprintReadOnly) FString logo;
    UPROPERTY(BlueprintReadOnly) FString video_banner;
    UPROPERTY(BlueprintReadOnly) FString profile_banner;
    UPROPERTY(BlueprintReadOnly) FString profile_banner_background_color;
    UPROPERTY(BlueprintReadOnly) bool partner;
    UPROPERTY(BlueprintReadOnly) FString url;
    UPROPERTY(BlueprintReadOnly) int32 views;
    UPROPERTY(BlueprintReadOnly) int32 followers;

    FTwitchChannel() {}
};

/**
 * @struct  TWITCHAUTH_API TwitchAuthActor.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthActor.h
 *
 * @brief   Twitch subscription.
 */
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchSubscription
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString created_at;
    UPROPERTY(BlueprintReadOnly) FString _id;
    UPROPERTY(BlueprintReadOnly) FString sub_plan;
    UPROPERTY(BlueprintReadOnly) FString sub_plan_name;
    UPROPERTY(BlueprintReadOnly) bool is_gift;
    UPROPERTY(BlueprintReadOnly) FTwitchChannel channel;

    FTwitchSubscription() {}
};

USTRUCT(BlueprintType)

/**
 * @struct  TWITCHAUTH_API TwitchAuthActor.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthActor.h
 *
 * @brief   Twitch error from the Twitch API.
 */
struct TWITCHAUTH_API FTwitchError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString error;
    UPROPERTY(BlueprintReadOnly) int32 status;
    UPROPERTY(BlueprintReadOnly) FString message;

    FTwitchError() {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSignInPageLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserSignedIn, bool, SignedIn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTwitchUserSubscribedToChannel, bool, bSubscribed, FTwitchSubscription, TwitchSubscription);


UCLASS()
/**
 * @class   TWITCHAUTH_API TwitchAuthActor.h
 *          d:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthActor.h
 *
 * @brief   With this actor you can build your complete Twitch user sign in process.
 */
class TWITCHAUTH_API ATwitchAuthActor : public AActor
{
	GENERATED_BODY()
	
public:	

    /**
     * @fn  ATwitchAuthActor::ATwitchAuthActor();
     *
     * @brief   Default constructor.
     */
	ATwitchAuthActor();

    /** @brief   Client ID that is used to authenticate the Twitch user. */
    UPROPERTY(EditAnywhere, Category = "Twitch Auth")
    FString ClientID;

    /** @brief   Flag, whether the user should be verified every time. */
    UPROPERTY(EditAnywhere, Category = "Twitch Auth")
    bool ForceVerify = true;

    /** @brief   Event that will be fired when the Twitch user signed in. */
    UPROPERTY(BlueprintAssignable)
    FOnUserSignedIn OnUserSignedIn;

    /** @brief   Event that will be fired after the Twitch user sub to channel method. */
    UPROPERTY(BlueprintAssignable)
    FOnTwitchUserSubscribedToChannel OnTwitchUserSubscribedToChannel;
    
    /** @brief    Event that will be fired when the sign in page will be loaded up. */
    UPROPERTY(BlueprintAssignable)
    FOnSignInPageLoaded OnSignInPageLoaded;

    /**
     * @fn  void ATwitchAuthActor::StartUserSignIn();
     *
     * @brief   Start the sign in process with web browser if the user is not yet authenticated..
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    void StartUserSignIn();

    /**
     * @fn  FTwitchUser ATwitchAuthActor::GetSignedInTwitchUser();
     *
     * @brief   Returns a struct of the signed in Twitch user.
     *
     * @return  The signed in twitch user.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    FTwitchUser GetSignedInTwitchUser();

    /**
     * @fn  void ATwitchAuthActor::IsTwitchUserSubscribedToChannel(FTwitchUser TwitchUser, FString ChannelName);
     *
     * @brief   Starts a checking process if a given Twitch user is a subscriber to a given channel.
     *          This function will trigger the OnTwitchUserSubscribedToChannel event.
     *
     * @param   TwitchUser  The twitch user.
     * @param   ChannelName Name of the channel.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    void IsTwitchUserSubscribedToChannel(FTwitchUser TwitchUser, FString ChannelName);

    /**
     * @fn  FString ATwitchAuthActor::GetAccessToken();
     *
     * @brief   Returns the retrieved access token from the sign in process.
     *
     * @return  The access token.
     */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth")
    FString GetAccessToken();
    
    /**
     * @fn  void ATwitchAuthActor::SetAccessToken(FString AccessToken);
     *
     * @brief   Sets the access token if it has been saved to disk.
     *
     * @param   AccessToken The access token.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    void SetAccessToken(FString AccessToken);

    /**
    * @fn  void ATwitchAuthActor::ClearAccessToken();
    *
    * @brief   Clears the access token
    */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    void ClearAccessToken();

    /**
     * @fn  FError ATwitchAuthActor::GetLastError();
     *
     * @brief   Returns the last written error.
     *
     * @return  The last error.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    FTwitchError GetLastError();

protected:

    #pragma region UE4 Overrides
    // Nothing to do here...
    #pragma endregion

    #pragma region Blueprint Interaction

    /** @brief   This is always the last error message from within the current state of the actor. */
    FTwitchError m_LastError;
    
    /** @brief   Access token that has been received from the Twitch sign in. */
    FString m_AccessToken;

    /**
     * @fn  void ATwitchAuthActor::LogError(const FTwitchError& twitchError);
     *
     * @brief   Logs an error.
     *
     * @param   twitchError The twitch error.
     */
    void LogError(const FTwitchError& twitchError);

    #pragma endregion // Blueprint Interaction

    #pragma region HTTP API

    /** @brief   HTTP module reference. */
    FHttpModule* m_Http;

    /**
    * @fn  TSharedRef<IHttpRequest> ATwitchAuthActor::CreateHttpRequest(FString Endpoint, EHttpVerb Verb);
    *
    * @brief   Creates a HTTP request.
    *
    * @param   Endpoint    Endpoint the request should connect to.
    * @param   Verb        HTTP Verb that the request should use.
    *
    * @return  Created HTTP request reference.
    */
    TSharedRef<IHttpRequest> CreateHttpRequest(FString Endpoint, EHttpVerb Verb);

    /**
    * @fn  FString ATwitchAuthActor::GetHttpVerbStr(EHttpVerb Verb);
    *
    * @brief   Returns a HTTP verb string based on a given HTTP verb enum.
    *
    * @param   Verb    HTTP verb enum value.
    *
    * @return  HTTP verb string.
    */
    FString GetHttpVerbStr(EHttpVerb Verb);

    /**
    * @fn  bool ATwitchAuthActor::IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful);
    *
    * @brief   Checks if a given response is valid.
    *
    * @param   Response        Response to check.
    * @param   bWasSuccessful  Flag, whether the request went fully through.
    *
    * @return  Flag, whether the response is valid or not.
    */
    bool IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful);

    /**
    * @fn  void ATwitchAuthActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    *
    * @brief   Common callback function for any HTTP requests made.
    *
    * @param   Request         HTTP request that generated this response.
    * @param   Response        HTTP Response from the requested server.
    * @param   bWasSuccessful  Flag, whether or not the request went through successfully.
    */
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    #pragma endregion // HTTP API

private:

    #pragma region Web Browser Widget

    /**
     * @brief   Access token key definition which is used to cut out the access token from the
     *          redirect URI.
     */
    FString AccessTokenKey = "access_token=";

    /** @brief   Access token URI string to check if a given URI is the correct one. */
    FString AccessTokenUriContainsStr = "https://localhost/#access_token";

    /** @brief   Shared pointer to the created container widget which contains the web browser widget. */
    TSharedPtr<SWidget> WeakWidget;

    /** @brief   Shared pointer to the created browser widget. */
    TSharedPtr<class SWebBrowser> WebBrowserWidget;

    /**
     * @fn  void ATwitchAuthActor::EnableCursor(bool bEnable);
     *
     * @brief   Enables the cursor.
     *
     * @param   bEnable True to enable, false to disable.
     */
    void EnableCursor(bool bEnable);

    /**
     * @fn  FString ATwitchAuthActor::GetTwitchSigninUrl();
     *
     * @brief   Returns the sign in URL from Twitch services.
     *
     * @return  URL for the sign in page.
     */
    FString GetTwitchSigninUrl();

    /**
     * @fn  FString ATwitchAuthActor::GetAccessToken(const FString& RedirectUri);
     *
     * @brief   Retrieves the access token from the redirect URI that is generated after the sign in
     *          process.
     *
     * @param   RedirectUri Redirect URI.
     *
     * @return  Cutted access token sub-string.
     */
    FString GetAccessToken(const FString& RedirectUri);

    /**
     * @fn  TSharedPtr<SWidget> ATwitchAuthActor::CreateWebBrowserWidget();
     *
     * @brief   Creates the web browser widget.
     *
     * @return  Created web browser widget.
     */
    TSharedPtr<SWidget> CreateWebBrowserWidget();

    /**
     * @fn  void ATwitchAuthActor::AddWidgetToViewport(TSharedPtr<SWidget> Widget);
     *
     * @brief   Adds a given widget to the viewport.
     *
     * @param   Widget  Widget to add to the viewport.
     */
    void AddWidgetToViewport(TSharedPtr<SWidget> Widget);

    /**
     * @fn  void ATwitchAuthActor::RemoveWidgetFromViewport(TSharedPtr<SWidget> Widget);
     *
     * @brief   Removes a given widget from the viewport, given that it is currently in the viewport.
     *
     * @param   Widget  Widget to remove from the viewport.
     */
    void RemoveWidgetFromViewport(TSharedPtr<SWidget> Widget);

    /**
     * @fn  void ATwitchAuthActor::HandleOnUrlChanged(const FText& InText);
     *
     * @brief   Handler method which is called by the web browser when the URL changes.
     *
     * @param   InText  New URL.
     */
    void HandleOnUrlChanged(const FText& InText);
    
    #pragma endregion // Web Browser Widget

    #pragma region Twitch API Endpoints

    /** @brief   Base API URL. */
    FString m_ApiBaseUrl = "https://api.twitch.tv/kraken";

    /** @brief   Twitch API Endpoints. */
    FString m_UserEndpoint = "/user";
    FString m_ChannelEndpoint = "/users?login=";
    FString m_SubscriptionEndpoint = "/users/$1/subscriptions/$2";

    /** @brief   Keeps track of what endpoint was last requested to. */
    EEndpoint m_LastEndpoint = EEndpoint::None;

    /** @brief   Signed in Twitch user from the browser module. */
    FTwitchUser m_TwitchUser;

    /** @brief   Twitch channel user from the GetTwitchChannel request. */
    FTwitchChannelUser m_TwitchChannelUser;

    /** @brief   Twitch subscription from the CheckUserSubcription request. */
    FTwitchSubscription m_TwitchSubscription;

    /**
     * @fn  FString ATwitchAuthActor::RetrieveTwitchChannelUserFromResponseBody(const FString& ResponseBody) const;
     *
     * @brief   Pulls out the JSON from the channel request.
     *
     * @param   ResponseBody    JSON response body from the request.
     *
     * @return  Cleared JSON.
     */
    FString RetrieveTwitchChannelUserFromResponseBody(const FString& ResponseBody) const;

    /**
     * @fn  void ATwitchAuthActor::ExecuteGetTwitchUserRequest();
     *
     * @brief   Executes the GET /user endpoint request.
     */
    void ExecuteGetTwitchUserRequest();

    /**
     * @fn  void ATwitchAuthActor::HandleGetTwitchUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
     *
     * @brief   Handles the GET /user endpoint response.
     *
     * @param   Request     Request made.
     * @param   Response    Returned response.
     */
    void HandleGetTwitchUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    /**
     * @fn  void ATwitchAuthActor::ExecuteGetTwitchChannelRequest(FString ChannelName);
     *
     * @brief   Executes the GET /user endpoint request.
     *
     * @param   ChannelName Name of the channel.
     */
    void ExecuteGetTwitchChannelRequest(FString ChannelName);

    /**
     * @fn  void ATwitchAuthActor::HandleGetTwitchChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
     *
     * @brief   Handles the GET /user endpoint response.
     *
     * @param   Request     Request made.
     * @param   Response    Returned response.
     */
    void HandleGetTwitchChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    /**
     * @fn  void ATwitchAuthActor::ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel);
     *
     * @brief   Executes the GET /user endpoint request.
     *
     * @param   TwitchUser      The twitch user.
     * @param   TwitchChannel   The twitch channel.
     */
    void ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel);

    /**
     * @fn  void ATwitchAuthActor::HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
     *
     * @brief   Handles the GET /user endpoint response.
     *
     * @param   Request     Request made.
     * @param   Response    Returned response.
     */
    void HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    #pragma endregion // Twitch API Endpoints

};
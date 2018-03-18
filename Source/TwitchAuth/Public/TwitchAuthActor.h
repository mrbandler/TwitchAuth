// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "TwitchAuthActor.generated.h"

/// <summary>
/// All needed HTTP verbs.
/// </summary>
UENUM()
enum class EHttpVerb : uint8
{
	Get, 
	Post,
	Put,
	Patch,
	Delete
};

/// <summary>
/// All possible endpoints.
/// </summary>
UENUM()
enum class EEndpoint : uint8
{
    None,
    User,
    Channels,
    Subscriptions
};

USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchUserNotifications
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) bool push;
    UPROPERTY(BlueprintReadOnly) bool email;

    FTwitchUserNotifications() {}
};

/// <summary>
/// This struct represents a Twitch channel user.
/// </summary>
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

/// <summary>
/// This struct represents the signed in Twitch user.
/// </summary>
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
};

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
};

/// <summary>
/// With this actor you can build your complete Twitch user sign in process.
/// </summary>
UCLASS()
class TWITCHAUTH_API ATwitchAuthActor : public AActor
{
	GENERATED_BODY()
	
public:	
    
    /// /// <summary>
    /// Default constructor.
    /// </summary>
	ATwitchAuthActor();

    /// <summary>
    /// Client ID that is used to authenticate the Twitch user.
    /// </summary>
    UPROPERTY(EditAnywhere, Category = "Twitch Auth")
    FString ClientID;

    /// <summary>
    /// Flag, wheter the user should be verified every time.
    /// </summary>
    UPROPERTY(EditAnywhere, Category = "Twitch Auth")
    bool ForceVerify = true;

    /// <summary>
    /// Twitch user access token that can be optained via the StartUserSignIn method.
    /// If the access token has been saved to disk it can be set here to skip the sign in process.
    /// </summary>
    UPROPERTY(EditAnywhere, Category = "Twitch Auth")
    FString AccessToken;

    /// <summary>
    /// Start the sign  in process with web browser and all.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    void StartUserSignIn();

    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    FString GetLastErrorMessage();

    /// <summary>
    /// Returns the signed in twitch user.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    FTwitchUser GetSignedInTwitchUser();

    UFUNCTION(BlueprintCallable, Category = "Twitch Auth")
    void IsTwitchUserSubscribedToChannel(FTwitchUser TwitchUser, FString ChannelName);

protected:

    #pragma region UE4 Overrides

    /// /// <summary>
    /// Will be called when this actor will be spawned.
    /// </summary>
	virtual void BeginPlay() override;

    #pragma endregion

    #pragma region Blueprint Interaction

    /// <summary>
    /// This is always the last error message from within the current state of the actor.
    /// </summary>
    FString m_LastErrorMessage;

    /// <summary>
    /// This is a custom event which is fired when the user signed in successfully.
    /// </summary>
    UFUNCTION(BlueprintNativeEvent, Category = "Twitch Auth")
    void OnUserSignedIn();
    void OnUserSignedIn_Implementation() {}

    UFUNCTION(BlueprintNativeEvent, Category = "Twitch Auth")
    void OnTwitchUserSubscribedToChannel(bool bSubscribed, FTwitchSubscription TwitchSubscription);
    void OnTwitchUserSubscribedToChannel_Implementation(bool bSubscribed, FTwitchSubscription TwitchSubscription) {}

    #pragma endregion // Blueprint Interaction

private:

    #pragma region HTTP API

    /// <summary>
    /// HTTP module reference.
    /// </summary>
	FHttpModule* m_Http;

    /// <summary>
    /// Creates a HTTP request.
    /// </summary>
    /// <param name="Endpoint">Endpoint the request should connect to.</param>
    /// <param name="Verb">HTTP Verb that the request should use.</param>
    /// <returns>Created HTTP request reference.</returns>
    TSharedRef<IHttpRequest> CreateHttpRequest(FString Endpoint, EHttpVerb Verb);

    /// <summary>
    /// Returns a HTTP verb string based on a given HTTP verb enum.
    /// </summary>
    /// <param name="Verb">HTTP verb enum value.</param>
    /// <returns>HTTP verb string.</returns>
    FString GetHttpVerbStr(EHttpVerb Verb);
	
    /// <summary>
    /// Checks if a given response is valid.
    /// </summary>
    /// <param name="Response">Response to check.</param>
    /// <param name="bWasSuccessful">Flag, wheter the request went fully through.</param>
    /// <returns>Flag, wheter the response is valid or not.</returns>
    bool IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful);


    /// <summary>
    /// Common callback function for any HTTP requests made.
    /// </summary>
    /// <param name="Request">HTTP request that generated this response.</param>
    /// <param name="Response">HTTP Response from the requested server.</param>
    /// <param name="bWasSuccessful">Flag, wheter or not the request went through successfully.</param>
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    
    #pragma endregion // HTTP API

    #pragma region Web Browser Widget

    /// <summary>
    /// Access token key definition which is used to cut out the access token from the redirect URI.
    /// </summary>
    FString AccessTokenKey = "access_token=";

    /// <summary>
    /// Access token URI string to check if a given URI is the correct one.
    /// </summary>
    FString AccessTokenUriContainsStr = "https://localhost/#access_token";

    /// <summary>
    /// Shared pointer to the created container widget which contains the web browser widget.
    /// </summary>
    TSharedPtr<SWidget> WeakWidget;

    /// <summary>
    /// Shared pointer to the created browser widget.
    /// </summary>
    TSharedPtr<class SWebBrowser> WebBrowserWidget;

    /// <summary>
    /// Returns the sign in URL from Twitch services.
    /// </summary>
    /// <returns>URL for the sign in page.</returns>
    FString GetTwitchSigninUrl();

    /// <summary>
    /// Retrieves the access token from the redirect URI that is generated after the sign in process.
    /// </summary>
    /// <param name="RedirectUri">Redirect URI.</param>
    /// <returns>Cutted access token sub-string.</returns>
    FString GetAccessToken(const FString& RedirectUri);

    /// <summary>
    /// Creates the web browser widget.
    /// </summary>
    /// <returns>Created web browser widget.</returns>
    TSharedPtr<SWidget> CreateWebBrowserWidget();

    /// <summary>
    /// Adds a given widget to the viewport.
    /// </summary>
    /// <param name="Widget">Widget to add to the viewport.</param>
    void AddWidgetToViewport(TSharedPtr<SWidget> Widget);

    /// <summary>
    /// Removes a given widget from the viewport, given that it is currently in the viewport.
    /// </summary>
    /// <param name="Widget">Widget to remove from the viewport.</param>
    void RemoveWidgetFromViewport(TSharedPtr<SWidget> Widget);

    /// <summary>
    /// Handler method which is called by the web browser when the URL changes.
    /// </summary>
    /// <param name="InText">New URL.</param>
    void HandleOnUrlChanged(const FText& InText);
    
    #pragma endregion // Web Browser Widget

    #pragma region Twitch API Endpoints

    /// <summary>
    /// Base API URL.
    /// </summary>
    FString m_ApiBaseUrl = "https://api.twitch.tv/kraken";

    /// <summary>
    /// Twitch API Endpoints.
    /// </summary>
    FString m_UserEndpoint = "/user";
    FString m_ChannelEndpoint = "/users?login=";
    FString m_SubscriptionEndpoint = "/users/$1/subscriptions/$2";

    /// <summary>
    /// Keeps track of what endpoint was last requested to.
    /// </summary>
    EEndpoint m_LastEndpoint = EEndpoint::None;

    /// <summary>
    /// Signed in Twitch user from the browser module.
    /// </summary>
    FTwitchUser m_TwitchUser;

    /// <summary>
    /// Twitch channel user from the GetTwitchChannel request.
    /// </summary>
    FTwitchChannelUser m_TwitchChannelUser;

    FTwitchSubscription m_TwitchSubscription;

    FString RetrieveTwitchChannelUserFromResponseBody(const FString& ResponseBody) const;

    /// <summary>
    /// Executes the GET /user endpoint request.
    /// </summary>
    void ExecuteGetTwitchUserRequest();

    /// <summary>
    /// Handles the GET /user endpoint response.
    /// </summary>
    /// <param name="Request">Request made.</param>
    /// <param name="Response">Returned response.</param>
    void HandleGetTwitchUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    /// <summary>
    /// Executes the GET /user endpoint request.
    /// </summary>
    void ExecuteGetTwitchChannelRequest(FString ChannelName);

    /// <summary>
    /// Handles the GET /user endpoint response.
    /// </summary>
    /// <param name="Request">Request made.</param>
    /// <param name="Response">Returned response.</param>
    void HandleGetTwitchChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    /// <summary>
    /// Executes the GET /user endpoint request.
    /// </summary>
    void ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel);

    /// <summary>
    /// Handles the GET /user endpoint response.
    /// </summary>
    /// <param name="Request">Request made.</param>
    /// <param name="Response">Returned response.</param>
    void HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    #pragma endregion // Twitch API Endpoints

};
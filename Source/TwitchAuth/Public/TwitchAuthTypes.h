// Copyright (c) 2018 fivefingergames.

#pragma once
 
#include "TwitchAuthTypes.generated.h"

/**
* Twitch user notification settings.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchUserNotifications
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool push;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool email;

    FTwitchUserNotifications() {}
};

/**
* Twich channel user.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchChannelUser
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString display_name;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString _id;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString name;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString type;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString bio;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString created_at;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString updated_at;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString logo;

    FTwitchChannelUser() {}
};

/**
* Normal Twitch user.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchUser : public FTwitchChannelUser
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString email;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool email_verified;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool partnered;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool twitter_connected;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FTwitchUserNotifications notifications;

    FTwitchUser() {}
};

/**
* Twitch channel.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchChannel
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool mature;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString status;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString broadcaster_language;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString game;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString language;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString name;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString created_at;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString updated_at;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString _id;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString logo;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString video_banner;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString profile_banner;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString profile_banner_background_color;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool partner;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString url;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") int32 views;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") int32 followers;

    FTwitchChannel() {}
};

/**
* Twitch subscription.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchSubscription
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString created_at;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString _id;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString sub_plan;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString sub_plan_name;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool is_gift;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FTwitchChannel channel;

    FTwitchSubscription() {}
};

/**
* Twitch follow.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchFollow
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString created_at;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") bool notifications;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FTwitchChannel channel;

    FTwitchFollow() {}
};

/**
* Twitch error from the Twitch API.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString error;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") int32 status;
    UPROPERTY(BlueprintReadOnly, Category = "Twitch Auth") FString message;

    FTwitchError() {}
};
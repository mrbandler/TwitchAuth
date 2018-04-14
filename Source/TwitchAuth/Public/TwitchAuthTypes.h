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

    UPROPERTY(BlueprintReadOnly) bool push;
    UPROPERTY(BlueprintReadOnly) bool email;

    FTwitchUserNotifications() {}
};

/**
* Twich channel user.
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
* Normal Twitch user.
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
* Twitch channel.
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
* Twitch subscription.
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

/**
* Twitch error from the Twitch API.
*/
USTRUCT(BlueprintType)
struct TWITCHAUTH_API FTwitchError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString error;
    UPROPERTY(BlueprintReadOnly) int32 status;
    UPROPERTY(BlueprintReadOnly) FString message;

    FTwitchError() {}
};
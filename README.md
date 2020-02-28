# Twitch Auth

### The plugin is available for **$4.99** on the [**UE4 Marketplace**](https://www.unrealengine.com/marketplace/en-US/product/twitch-auth), please consider purchasing there to support further development of the plugin.

[![Download on UE4 Marketplace](https://img.shields.io/badge/UE4-Marketplace-lightgrey.svg)](https://www.unrealengine.com/marketplace/twitch-auth) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/98eab67a0480440a825f6b910f54d67f)](https://app.codacy.com/app/mrbandler/TwitchAuth?utm_source=github.com&utm_medium=referral&utm_content=mrbandler/TwitchAuth&utm_campaign=Badge_Grade_Dashboard) [![Donate with Bitcoin](https://en.cryptobadges.io/badge/micro/3KGsDx52prxWciBkfNJYBkXaTJ6GUURP2c)](https://en.cryptobadges.io/donate/3KGsDx52prxWciBkfNJYBkXaTJ6GUURP2c) [![Donate with Litecoin](https://en.cryptobadges.io/badge/micro/LcHsJH13A8PmHJQwpbWevGUebZwhWNMXgS)](https://en.cryptobadges.io/donate/LcHsJH13A8PmHJQwpbWevGUebZwhWNMXgS) [![Donate with Ethereum](https://en.cryptobadges.io/badge/micro/0xd6Ffc89Bc87f7dFdf0ef1aefF956634d4B7451c8)](https://en.cryptobadges.io/donate/0xd6Ffc89Bc87f7dFdf0ef1aefF956634d4B7451c8) [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.me/mrbandler/)

![TwitchAuth](https://github.com/mrbandler/TwitchAuth/raw/master/Images/%231.png)

## Table Of Content

1. [Idea and Use Case](#1-idea-and-use-case) 🤔
2. [Installation](#2-installation) 💻
3. [Usage](#3-usage) ⌨️
4. [Bugs and Features](#4-bugs-and-features) 🐞💡
5. [Buy me a coffee](#5-buy-me-a-coffee) ☕
6. [License](#6-license) 📃

---

## 1. Idea and Use Case

The idea for this plugin came while watching the development stream from a fellow Twitch streamer called [Wheeze202](https://www.twitch.tv/wheeze202). He provides his Twitch subscribers early development builds of his current project [Galdur](https://galdurgame.com/) to playtest and give him feedback on. His infrastructure was simple, he simply whispered the link to the build when a subscriber entered a command in the Twitch chat.

I saw some flaws in this approach, because now you had to rely on the subscriber to keep the link a secret. I though there must be a better way. So, I set out to build this plugin to let any Twitch game developer have the ability to let potential subscriber play testers authenticate in-game. Now only subscribers to your channel can play the game.

> **NOTICE:** There is a huge update planned that utilizes UE4 sub-systems for improved user experience, additional platforms will be added aswell ([Patreon](https://www.patreon.com/5fingergames), [YouTube](https://www.youtube.com/channel/UCzdtkrH5yhKay8JM6D7qMxA)). Let me know what platform you want that I did not already mention.

### The plugin is available for **$4.99** on the **UE4 Marketplace**, please consider purchasing there to support further development of the plugin.

## 2. Installation

To install the plugin simply download it over the [Unreal Engine Marketplace](https://www.unrealengine.com/marketplace). Click [here](https://www.unrealengine.com/marketplace/twitch-auth) to download.

**The alternative:**

Download the plugin via the releases tab, unzip the archive and drop the unzipped folder into either a local plugins folder on your project. If your project is a blueprint only project simply add a dummy C++ class to you project to create the plugins folder.

Or install it engine wide in the ``<folder_to_your_ue4_installation/UE_4.19/Engine/Plugins``. If you didn't change the installation directory of your UE4 installation the folder should be under ``C:\Program Files\Epic Games\UE_4.19\Engine\Plugins``.

> **NOTE:** A detailed guide for both manual installation types can be found in the [wiki](https://github.com/mrbandler/TwitchAuth/wiki).

## 3. Usage

Please check the [docs](https://docs.fivefingergames.com/twitch-auth) for documentation and in depth usage description.

## 4. Bugs and Features

Please open a issue when you encounter any bugs 🐞 or have an idea for a new feature 💡.

## 5. Buy me a coffee

If you like you can buy me a coffee:

[![Support via PayPal](https://cdn.rawgit.com/twolfson/paypal-github-button/1.0.0/dist/button.svg)](https://www.paypal.me/mrbandler/)

[![Donate with Bitcoin](https://en.cryptobadges.io/badge/big/3KGsDx52prxWciBkfNJYBkXaTJ6GUURP2c)](https://en.cryptobadges.io/donate/3KGsDx52prxWciBkfNJYBkXaTJ6GUURP2c)

[![Donate with Litecoin](https://en.cryptobadges.io/badge/big/LcHsJH13A8PmHJQwpbWevGUebZwhWNMXgS)](https://en.cryptobadges.io/donate/LcHsJH13A8PmHJQwpbWevGUebZwhWNMXgS)

[![Donate with Ethereum](https://en.cryptobadges.io/badge/big/0xd6Ffc89Bc87f7dFdf0ef1aefF956634d4B7451c8)](https://en.cryptobadges.io/donate/0xd6Ffc89Bc87f7dFdf0ef1aefF956634d4B7451c8)

---

## 6. License

MIT License

Copyright (c) 2018 fivefingergames

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

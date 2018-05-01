rem Build the plugin
call RunUAT.bat BuildPlugin -Plugin="%CD%\TwitchAuth.uplugin" -Package="%CD%\Build\TwitchAuth" -Rocket

rem Zip the plugin
rd /s /q "./Build/TwitchAuth/Intermediate"
7z a ./Build/TwitchAuth.zip ./Build/TwitchAuth/*
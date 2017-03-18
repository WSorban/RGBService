@ECHO OFF
set PORT=30303
set RULE_NAME="RzrRGBService"

goto check_Permissions

:check_Permissions
    echo Administrative permissions required. Detecting permissions...

    net session >nul 2>&1
    if %errorLevel% == 0 (
        echo Success: Administrative permissions confirmed.
		netsh advfirewall firewall show rule name=%RULE_NAME% >nul
		if not ERRORLEVEL 1 (
			echo Hey, you already got a out rule by that name, you cannot put another one in!
		) else (
			echo Rule %RULE_NAME% does not exist. Creating...
			netsh advfirewall firewall add rule name=%RULE_NAME% dir=in action=allow program="%~dp0RGBService.exe" protocol=UDP localport=30303
			sc create %RULE_NAME% binpath=%~dp0RGBService.exe start=delayed-auto
			sc start %RULE_NAME%
			echo Success... Press any key to exit
		)
    ) else (
        echo Administrator permissions not granted. Please run this as administrator
    )

pause >nul
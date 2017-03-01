@ECHO OFF
set RULE_NAME="RzrRGBService"

goto check_Permissions

:check_Permissions
    echo Administrative permissions required. Detecting permissions...

    net session >nul 2>&1
    if %errorLevel% == 0 (
        echo Success: Administrative permissions confirmed.
		netsh advfirewall firewall show rule name=%RULE_NAME% >nul
		if not ERRORLEVEL 1 (
			netsh advfirewall firewall delete rule name=%RULE_NAME%
			echo Deleted the firewall rule
			net stop %RULE_NAME%
			sc delete %RULE_NAME%
		) else (
			echo Rule %RULE_NAME% does not exist...
		)

		echo Success... Press any key to exit
    ) else (
        echo Administrator permissions not granted. Please run this as administrator
    )

pause >nul
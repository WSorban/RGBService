
ECHO OFF
set PORT=30303
set RULE_NAME="RzrRGBService"

start cmd /c sc create %RULE_NAME% binpath=SampleService.exe start=delayed-auto

netsh advfirewall firewall show rule name=%RULE_NAME% >nul
if not ERRORLEVEL 1 (
    rem Rule %RULE_NAME% already exists.
    echo Hey, you already got a out rule by that name, you cannot put another one in!
) else (
    echo Rule %RULE_NAME% does not exist. Creating...
    start cmd /c netsh advfirewall firewall add rule name=%RULE_NAME% dir=in action=allow program="SampleService.exe" protocol=UDP localport=%PORT%
)
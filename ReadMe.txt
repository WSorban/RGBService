Description:
Service that will listen on UDP port 30303 for UDP packets sent from an Android application.

Should you want to add other controllers to it (like an Arduino), the sent UDP packets contain this:

unsigned chars:
R 0-255 G 0-255 B 0-255

(6 bytes)

The service binary can be downloaded from the Release folder: SampleService.exe

You may also want the install.bat file located in the same folder, which will open the UDP port 30303, and add the binary to the services, for you. (you must run it as administrator)
To uninstall, run uninstall.bat, which will delete the firewall rule, and remove the service.
You will need administrator rights for both.

The Android application was not written by me, and can be found here:

https://play.google.com/store/apps/details?id=com.howesthatapp.mike.razerconfigurator

Also big shout out for iMoD1998, for decoding the ASUS ROG GPU led code.
More info here:
https://www.youtube.com/watch?v=37iOmKfJJPs

This project is using the C++ Razer Chroma SDK, found here:
https://github.com/ChromaSDK/OfficialSample

Enjoy!

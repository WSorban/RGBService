Description:
Service that will listen on UDP port 30303 for UDP packets sent from an Android application.

Should you want to add other controllers to it (like an Arduino), the sent UDP packets contain this:

unsigned chars:
R 0-255 G 0-255 B 0-255

(6 bytes)

The service binary can be downloaded from the Release folder: RGBService.exe

You may also want the install.bat file located in the same folder, which will open the UDP port 30303, and add the binary to the services, for you. (you must run it as administrator)
To uninstall, run uninstall.bat, which will delete the firewall rule, and remove the service.
You will need administrator rights for both.

The Android application, coded by Mike Howes, can be found here:
https://play.google.com/store/apps/details?id=com.howesthatapp.mike.razerconfigurator
Unfortunately, no source code is available.

Also big thanks to iMoD1998, for decoding the ASUS ROG GPU led code.
More info here:
https://www.youtube.com/watch?v=37iOmKfJJPs

This project is using the following code dependencies:
- C++ Razer Chroma SDK: https://github.com/ChromaSDK/OfficialSample
- Windows Service: https://www.codeproject.com/Articles/499465/Simple-Windows-Service-in-Cplusplus
- UDP Server: http://www.java2s.com/Code/Cpp.net/Network/UdpServerwithSocket.htm

A demo video can be found here:
https://www.youtube.com/watch?v=TVKcGGtQR9c

Supported devices
- Static color:
  - All Razer devices
  - Asus ROG GPUs

- Breathing effect:
  - All Razer devices except mice
 
- Spectrum cycling effect:
  - All Razer devices
  
- Wave effect
  - Razer Keyboards
  - Razer Keypads
  - Razer Mousepads
  
- Reactive effect
  - Razer Keyboards
  - Razer Keypads
  
I have personally tested on the following devices:
- Razer Naga Epic Chroma
- Razer Blackwidow Chroma
- Razer Firefly Chroma
- Asus ROG 1080 GPU

Enjoy!

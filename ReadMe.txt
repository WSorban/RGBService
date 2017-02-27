Description:
Service that will listen on UDP port 30303 for UDP packets sent from an Android application.

Should you want to add other controllers to it (like an Arduino), the sent UDP packets contain this:

unsigned chars:
R 0-255 G 0-255 B 0-255

(6 bytes)

The service binary can be downloaded from the Debug folder: SampleService.exe

I am planning to upload the script that will register the binary as a service, and open firewall UDP port 30303.

But in the meantime, you will have to do these yourself.

The Android application was not written by me, and can be found here:

https://play.google.com/store/apps/details?id=com.howesthatapp.mike.razerconfigurator

Enjoy!

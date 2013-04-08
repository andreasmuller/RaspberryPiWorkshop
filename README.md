RaspberryPiWorkshop
===================

Currently uses the OF branch that can be found here:
https://github.com/openFrameworks-RaspberryPi/openFrameworks

A great setup guide for the Pi can be found here:
https://github.com/openFrameworks-RaspberryPi/openFrameworks/wiki/RaspberryPi

###<u>Graphics</u> 

<b>HelloWorld</b>  
Hello world, openFrameworks style!
  
![alt text](https://raw.github.com/andreasmuller/RaspberryPiWorkshop/master/Screenshots/Small/HelloWorld.jpg "Hello World")

<b>GenerativeExample</b>    
Used in our proof of concept demo where we set up the Pi to boot straight into an app, in this case a short program that creates some generative graphics.   
![alt text](https://raw.github.com/andreasmuller/RaspberryPiWorkshop/master/Screenshots/Small/GenerativeExample.jpg "Generative Example")

###<u>Multi-screen apps</u>    

The multi-screen applications need a little bit of setup. In the data/Settings/ folder there are 3 files to know about:  

<b>IsServer.txt</b>  
If this file exists, that computer is going to acts as the server. It will still draw things to screen.

<b>ClientSettings.xml</b>   
This contains the information about which screen you are.   
```
<Settings>  
	<ScreenIndex>0</ScreenIndex>  
</Settings>
```


<b>ServerSettings.xml</b>   
This contains information needed when acting as a server.
The _ServerSendHost_ should be the multicast address of your network. On OSX you can find this out by typing _ifconfig_ into the terminal.   
By specifying the multicast address for the network, the clients will automatically find the server and no IP information is needed by each client.
    
```
<Settings>   
	<ServerSendHost>10.255.255.255</ServerSendHost>   
	<ServerSendPort>7778</ServerSendPort>   
	<ServerReceivePort>7777</ServerReceivePort>   
</Settings>
```


<b>StandaloneTimingServer</b>  
A simple standalone server app that does no drawing of it's own, it only send out the server time to each client, this is used by _MultiScreenSimpleSync_.

<b>MultiScreenSimpleSync</b>   
Takes the synced time from _StandaloneTimingServer_ and displays a rotating circle and pulses the background. If the multicast address is set up correctly, this animation will be the same for any client node that is started.
  
<b>MultiScreenExample2D</b>  
A basic demo of an application that draws things over multiple screens.  
Here we use the same program to act as both the server and the client, as dictated by the existence of _IsServer.txt_ in _data/Settings_. Only one computer on the network should act as the server.   
This program will spawn particles that will travel across the screens. How many screens the server assumes it is creating content for can be set by the variable _screenAmount_ in the _update()_ function.   
The screen a client node is rendering is read from _ClientSettings.xml_, but you can  press the number keys on your keyboard to change this at runtime.   
![alt text](https://raw.github.com/andreasmuller/RaspberryPiWorkshop/master/Screenshots/Small/MultiScreenExample2D.jpg "MultiScreenExample2D")


<b>MultiScreenExample3D</b>   
Similar to the program above, but in this case we create a 3D camera and split this up depending on how many screens we will be drawing for.   
![alt text](https://raw.github.com/andreasmuller/RaspberryPiWorkshop/master/Screenshots/Small/MultiScreenExample3D.jpg "MultiScreenExample3D")  

<b>NetworkedCV</b>  
These network techniques are obviously great for writing programs that create graphics over multiple screens, but there are many other uses as well. In this program each Pi will do some computer vision on a piece of video (a live stream in a real world scenario), process the result into an outline and send it off to a central server.   
Here we are simply displaying the results on the central server, but this technique could be useful for merging the data from a large number of Pis +  cameras to track users in a large space for instance. 

###<u>Shaders</u> 

<b>ShaderLiveCodingExample</b>  
The relatively fast GPU in the Pi makes shaders really important on the Pi. This demo shows off some of this capability.   
![alt text](https://raw.github.com/andreasmuller/RaspberryPiWorkshop/master/Screenshots/Small/ShaderLiveCoding.jpg "ShaderLiveCoding")



<b>VideoShaderLiveCoding</b>   
Indeed, decoding 1080p video and running shaders on the result!

<b>Shader Examples</b> 
The projects whose names begin with ShaderExample are various examples of shaders that work with the new GLES2 Renderer in OF

<b>ShaderExample_AlphaMasking</b> is an OpenGL ES 2 compatible version of the alphaMaskingShaderExample found in openFrameworks/examples/gl/alphaMaskingShaderExample. It is a good project to compare the differences in GLSL syntax as <b>shaders written for the RPi will not work on the desktop (or vice-versa)</b>

<b>ShaderExample_Billboard</b> is a port from openFrameworks/examples/gl/billboardExample and contains both desktop and RPi compatible shaders.

A nice feature of the new OpenGL ES 2 renderer is that the shaders use the same version of GLSL that works with WebGL.
The following projects use shaders that were ported from ShaderToy. https://www.shadertoy.com/

- ShaderExample_MotionBlur  
- ShaderExample_Multitexture  
- ShaderExample_PostProcessing  

<b>ShaderExample_Empty</b>
This example is a good starting point to write your own shader. The project is set up to load an empty vertex and fragment shader from the bin/data folder

###<u>Cameras</u> 

<b>PS3EyeGrabber</b>
The Sony PS3 Eye is a popular USB camera in both the OpenCv and openFrameworks community. It is fairly inexpensive and works well on the RPi at 320x240@60fps.
openFrameworks video capture works with the Pi out out of the box, but with some simple extra commands you can offload some of the colorspace conversion to the hardware.   
  
<b>OpenNI2AppExample</b>
This example allows you to access the depth and RGB streams of a Asus Xtion Pro Live depth camera using OpenNI. It uses the newer OpenNI2 which has a much simpler syntax than the previous version. The project also works on the Mac Desktop which is useful to record .oni files that the project can also playback.
http://www.asus.com/Multimedia/Xtion_PRO/

###<u>GPIO</u> 

<b>wiringPiPotentiometerExample</b>
wiringPi (https://projects.drogon.net/raspberry-pi/wiringpi/)  is a great library that makes working with the RPi's GPIO very Arduino-like. This example shows you how to use it with OF and a analog to digital convertor (ADC) to change the playback speed of a sound.
![alt text](https://raw.github.com/andreasmuller/RaspberryPiWorkshop/master/Screenshots/wiringPiPotentiometerExampleSPI_bb.jpg "Schematics")

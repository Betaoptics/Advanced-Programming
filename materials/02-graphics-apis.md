# Graphics Programming - TTOW0430

&nbsp;
## **Graphics application development**
Usually graphics application development is done with a game/graphics development library. There are dozens of these libraries available, paid and free. Most well known libraries are [Unity](http://www.unity.com) and [Unreal Engine](https://www.unrealengine.com). Both of these are used extensively on game development, but both are advertising their features for animation, cinema, automotive, architecture etc. Most graphics programming libraries also contain a graphical editor to create and edit scenes for the application.

On some projects, Unity or Unreal Engine may seem a overkill. Especially in web-games development, on which there are many lightweight options. Sometimes you may need to work on hardware on which memory and processing power requirements limit the selection of technologies. On those situations, a low level graphics library like [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/) can be a good choice. SDL is also a good choise when full control over the app logics is required, no graphical editor is needed/desired, and programmer wants to avoid the hassle of initializing the underlying *low level graphics APIs*.

&nbsp;
## **Low level graphics APIs**
A Low level graphics API handles the communication between application layer and the graphics drivers that control the actual display adapter. Without these APIs, programmer would need to write the graphics code separately for each hardware architecture, which naturally is not a viable option on anything but very specific embedded systems. There are 3 major low level graphics APIs available:

&nbsp;
### **DirectX**
*DirectX* is a Microsoft specific graphics API available on Windows PC:s and XBox gaming consoles (and now dead Windows Phones). The first version of DirectX was released in September 1995 as the Windows Games SDK. Currently DirectX is at version 12 and version 11 is quaranteed to work on Windows 10 operating systems. DirectX contains many graphics/multimedia components. Its core graphics part is called Direct3D.

Direct3D is modern and powerful API, utilizing programmable pipelines and multithreading support. DirectX12 focused even more on parallel computing by introducing command lists and buffers to execute them in parallel on GPU. Its usage is limited only by the fact that it is available only in Microsoft ecosystem.

&nbsp;
### **OpenGL**
*Open Graphics Library*, OpenGL, is a cross-language, cross-platform application programming interface for rendering 2D and 3D vector graphics. The API is typically used to interact with a graphics processing unit to achieve hardware-accelerated rendering.

Silicon Graphics (SGI) began developing OpenGL in 1991 and released it on June 30, 1992. Applications use it extensively in the fields of computer-aided design (CAD), virtual reality, scientific visualization, information visualization, flight simulation, and video games. Since 2006, OpenGL has been managed by the non-profit technology consortium Khronos Group.

OpenGL has been around for a long time, and in good and bad it has retained its low level, procedural nature. Core of the OpenGL is pretty much as low level as you can go without touching the drivers. The core API itself does not offer any higher level features like loading a texture from file. This has caused a large amount of extensions and helper libraries to be built around OpenGL.

Programming OpenGL is quite easy to get started and learn. Its available virtually in any system, including Windows PC, mobile devices (Android, iOS), Linux operating system, single board computers, embedded devices etc. The noticable handicap of OpenGL is that it is not well suitable for parallel processing on GPU, as all OpenGL API calls must be made from same thread where its *context* was created. Also, its architecture causes some CPU overhead when setting up the data for GPU processing.

&nbsp;
### **OpenGL ES**
OpenGL ES (embedded systems) is a subset of OpenGL API, targeted to run on mobile and embedded systems. On first version, OpenGL ES was very limited, used *fixed point* arithmetics, and generally lacked performance on some chipsets causing developers to write software based graphics engines. Over the time it got better, but still the API was too limiting for serious graphics programming. The OpenGL ES 2.0 fixed all that by introducing the same programmable pipeline as 'parent' OpenGL. This allowed existing OpenGL desktop code to be ported to OpenGL ES quite easily. Currently at version 3.2, the version 2.0 is available virtually on any mobile device, and 3.1 in devices from past half a decade.

&nbsp;
### **Vulcan**
Vulkan is a low-overhead, cross-platform 3D graphics and computing API. Vulkan targets high-performance realtime 3D graphics applications such as video games and interactive media across all platforms. Compared to other APIs like OpenGL, Direct3D and *Metal* (Apples own implementation of Vulkan), Vulkan is intended to offer higher performance and more balanced CPU/GPU usage. Vulkan is a lower-level API and offers parallel tasking. In addition to its lower CPU usage, Vulkan is also able to better distribute work among multiple CPU cores.

Vulkan was first announced by the non-profit Khronos Group at GDC 2015. The Vulkan API was initially referred to as the *next generation OpenGL initiative*, or *OpenGL next* by Khronos, but use of those names was discontinued when Vulkan was announced. Vulkan is derived from and built upon components of AMD's *Mantle API*, which was donated by AMD to Khronos with the intent of giving Khronos a foundation on which to begin developing a low-level API that they could standardize across the industry. [Source](https://en.wikipedia.org/wiki/Vulkan_(API))

In game development, support of Vulcan has grown widespread as many game engines have implemented the support. Any hardware that supports OpenGL ES 3.1, or OpenGL 4.x and up should be adaptable to Vulcan, but Vulcan specific driver must be developed. While writing this (2020), the Vulcan support on mobile and embedded devices vary and drivers are known to be somewhat buggy.

As for the hobbyist programmer, Vulcan can be an armful. The setup requires a lot of code and rendering a single triangle may mean hundreds of lines of setup code. Also, the performance benefits over OpenGL may be small if rest of the application design is not able to handle the powerful parallelism of Vulcan.

&nbsp;
## **Motivation**
Why in earth would anybody like to write their own graphics engine when there is so many ready made libraries available? Curiosity. Desire to understand. Craftmanship. Granted, in real world it does not make much sense to write everything from the scratch, but in same cases it may be benefical; Minimizing dependencies, avoiding license costs or other license related problems, desire to retain IPR of the software, or flexibility to fix problems as they are all your own.

We choose to write our own engine, and to make matters even *worse*, we develop our code to OpenGL ES compatibility for mobile device usage. Our motivation for this is to learn. To understand what happens underneath the hood. Our engine may not be the best one or produce nicest graphics, but it is our own, on which we know and understand every nut and bolt of.

**Happy coding!**

&nbsp;
----
**© 2020 Jani Immonen**


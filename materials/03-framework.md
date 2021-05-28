# Graphics Programming - TTOW0430

&nbsp;
## **The Core**
Lets start coding our graphics engine framework, which we will call **core**. After 6 hours of coding we will have a nice window with gray background. Don't let that put you down, as we learn a lot of structure of graphics application while we get there. It is a journey that counts, not the destination.

We will create an application interface class that is implemented by all *actual* applications. The application interface will implement:

- main loop
    - event and input handling
    - timing
- graphics layer abstraction

&nbsp;
### **Main Loop**
Almost all graphics programming libraries attempt to hide the main loop from the developer. There are many (good) reasons to do so. The main loop may provide multiple execution paths or be prepared for parallel execution. Be it any case, all programs have a main loop. The main loop is responsible for handling the operating system events, providing input handling, calculating timing, and naturally offers the developer means to update application objects.

Operating system events that graphics programs are interested of are typically something related to screen or window (*view*) size or events to minimize application to the taskbar. Application keeps track of available view area and stops updating graphics to the view if application is minimized. On some environments, programmer can reserve an *exclusive* access to graphics hardware when running in full screen. Then, extra steps are required when application is minimized or switched to windowed mode and access to GPU resources must be given to other applications.

Main loop is also responsible of handling the timing of the application updates. Every PC and device runs at the different speed, and to make something on screen move the same amount in same amount of time requires timing. Some game consoles or devices could have *static* timing based on known screen *refresh rate*, like 50 or 60hz, but nowdays all graphics engines use timing to solve this issue.

The timing can be implemented in multiple ways:

- Graphics and objects are updated at display refresh
- Graphics are updated at display refresh, and objects at selected *fixed* rate
- Graphics are updated at display refresh, and different components at selected *fixed* rate on each
    - IE; Graphics are updated 60 times a second, physics can be updated 15 times a second, animation systems 30 times and so on.

When components are separated into their own update loops, it allows to use parallelism to run the components. For example physics can continue running when graphics system is still drawing a previous frame. It is a powerful way to handle main loop, but it also requires synchronization. The physics system must produce its result as a *mutable state*, which is used by the graphics system to draw everything to screen. And while physics is updating the mutable state, graphics system must wait until the update is complete before accessing it.

For the purposes of this course we use simpler approach where everything is updated in single main loop and when loop is complete, the resulting graphics are transfered to the view. Timing is handled by something called *frame time*.

The *frame time* is simply a time that has passed since we last ran thru the main loop. It is easiest to handle as *seconds per frame*. Now, if your main loop is running 100 times per second, the frame time would be 0.01 seconds (10 milliseconds). When we want to move some graphics object to the right at steady speed, we move it by *speed * frame time*, which will make object move *speed* units per second.

Frame time system is simple, but its not without its problems. Imagine that your computer starts doing some background jobs and your program process is halted for a 3 seconds. When it starts running again the frame time would be calculated as 3 seconds and everything in app will move by a huge step and usually this is not desired effect. Also, in collision detection the frame time only cannot be used as objects may 'jump' over each other in single frame. One other thing that is that using frame time is not *deterministic*. It is impossible to replay events as frame time varies depending on how fast computer is currently executing the code, the playback of previous frames will not produce the same result.



## **Happy Coding!**

[Lesson source code](../source/lesson01/)

&nbsp;
----
**© 2020 Jani Immonen**


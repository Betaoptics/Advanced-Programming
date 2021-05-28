# Graphics Programming - TTOW0430
# LAB2

You can work on this assignment in groups of 1-3 people.

When the task is complete

1. Delete the temporary files: '.vs', 'Debug' and 'Release' folders
2. Add the 'readme.txt' file which contains your name(s), student id(s) and any additional information.
3. Compress the project files into a zip file
4. Upload zip file to Moodle's return box


&nbsp;
## Build a Tank

- Take the LAB2 project as a starting point
- Build a tank from three boxes
- Place the tank parts into the scenegraph so that the turret follows the base and the pipe follows the turret
- Perform simple controls:
-- Arrow keys left and right turns tank base
-- Arrow keys up and down move tank base forward and backward
-- 'A' and 'D' rotates turret to the left and right
-- 'W' and 'S' rotates pipe up and down

&nbsp;
### See reference video [here](./lab02-reference.mp4)

Tips:
- You need to declare 3 Geometry objects, one for each piece of Tank.
- You can use one material to all Tank pieces.
- On Windows, use 'GetAsyncKeyState' function to check if certain key is pressed down. For example:

```
if (GetAsyncKeyState(VK_LEFT))
{
	// arrow key left is down
}

if (GetAsyncKeyState('A'))
{
	// 'A' is down
}
```

&nbsp;
----
**© 2020 Jani Immonen**


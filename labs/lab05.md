# Graphics Programming - TTOW0430
# LAB5

You can work on this assignment in groups of 1-3 people.

When the task is complete

1. Delete the temporary files: '.vs', 'Debug' and 'Release' folders
2. Add the 'readme.txt' file which contains your name(s), student id(s) and any additional information.
3. Compress the project files into a zip file
4. Upload zip file to Moodle's return box


&nbsp;
## Texture transparency and blending

- Take the LAB5 project as a starting point
- Load the 2 textures (terrain.jpg and grass.png) provided in LAB5 project
- Build a scene with one terrain quad and several hundred quads with grass texture
- Use Geometry::GetQuad to generate necessary geometry
- Use the provided CTerrain and CGrass classes as a templates
- Modify the fragment shader in a way that output color alpha channel comes from texture and is unaffected by the lighting
- Modify the fragment shader that fully transparent pixels are discarded
- Make the grass 'swing'. The reference video uses shear matrix to achieve the effect. Similar effect can be achieved by rotating around z-axis

&nbsp;
### See reference video [here](./lab05-reference.mp4)

&nbsp;
----
**© 2020 Jani Immonen**

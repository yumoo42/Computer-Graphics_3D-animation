# Computer-Graphics_3D-animation
## 1. Environment
Ubuntu 18.04.6 LTS / Visual Studio Code / C++\
Libraries: glm, glad, stb_image

## 2. Method Description
The overall structure is modified based on https://github.com/Learus/solar-system-opengl.
### (1) Added SceneNode Class:
Organizes scene nodes in a tree structure to construct the model hierarchy.\
**setTransformation / getTransformation:** Used to set and get the transformation matrix of a node.\
**addChild:** Adds child nodes to the current node.\
**draw:** Renders the scene nodes to draw the entire model.

### (2) Added Texture Mapping:
Added texture mapping to the cube.mtl file to apply textures to the model.

### (3) Animation Handling in main.cpp:
**updateWalkAnimation:** Updates the rotation angle of the legs during animation to simulate walking. \
**Linear interpolation(lerpRotation)** is used to compute the rotation angle between keyframes.\
**updateModelPosition:** Updates the position of the entire model during the animation, enabling the model to move within the window while performing the walking animation, simulating real movement.

### In the main program:
A window is created, and callback functions are set.\
Shaders and 3D models are loaded.\
A SceneNode is created to construct the animal shape based on cube.obj. The head, tail, and thighs are set as child nodes of the body, and the lower legs are set as child nodes of the thighs, creating a three-layer tree structure.\
The initial position and orientation of the entire animal model are set.\
During the rendering loop, mouse scroll and keyboard input are used to handle corresponding events. If the animation is playing, the leg rotation angles and the overall position of the model are updated.

## 3. How to Run the Program
Run **make** to compile and execute the program.\
Press the Enter key to play or pause the animation.\
Use the A, D, W, and S keys to adjust the camera angle.\
Press the ESC key to exit the program.\

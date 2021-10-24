# PixelDancer-OpenGL

PixelDancer is a project to create libraries for OpenGL, Vulkan and DirectX graphic programming api s. PixelDancer-OpenGL is a library for OpenGL graphic programming api.

## Before Using 

You have to have this packages to be included

- [x] DearImGui
- [x] Glad
- [x] GLM 
- [x] GLFW
- [x] assimp
- [x] Stb-image

DO NOT FORGET TO INCLUDE glad.c FILE TO YOUR PROJECT

## Usage of Examples

You can use them as main file but for pbr_cerberus.cpp file you have to download cerberus model (You can reach with this http://www.artisaverb.info/PBT.html link.). After you download it create objects file in data file and place model under cerberus filename in objects file.<br/>

Final data file should be like this:

data<br/>
|----->textures<br/>
|----->objects<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|----->cerberus<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|----->Textures<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|----->Cerberus_LP.fbx<br/>

<br/>After the steps you can use pbr_cerberus.cpp file as main file. <br/>

When using pbr_cerberus and shadow examples you have to comment 160, 161 and 162 lines in window.h file. If you are going to use DearImGui in your projects you have to uncomment them.

## Some Output of Examples

### PBR Cerberus
![pbr_model](https://user-images.githubusercontent.com/76731692/137396996-17755117-f1ad-4363-9910-de87a94ad8df.jpg)

### Shockwave
https://user-images.githubusercontent.com/76731692/137397022-b8b27188-2a8c-4060-9ef4-4db291062dd9.mp4

- [x] For more outputs you can look at https://github.com/alpertunga-bile/Showcase repository's OpenGL section.

## References
Vries J.(2020). Learn OpenGL: Learn modern OpenGL graphics programming in a step-by-step fashion(1st ed.). Kendall & Welling.<br/>
Wolff D.(2018). OpenGL 4 Shading Language Cookbook(3rd ed.). Packt Publishing.<br/>
Ben Cloward's Youtube Tutorials. https://www.youtube.com/user/bcloward<br/>
CgShow's Youtube Tutorials. https://www.youtube.com/user/asif786ali1<br/>

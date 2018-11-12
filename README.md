# MyEngine (Provisional name)

## Introduction
MyEngine is a 3D game engine (work-in-progress) developed from scratch by Jordi Tudela Alcacer.

## Features

### Inspector
The inspector shows the configuration parameters for the selected object in the hierarchy tab.

### Models
The engine is capable of **loading multiple 3D models from FBX files**. Drag and drop a FBX file to the working space and it will be automatically loaded. The engine will take into account the position, rotation and scale of any submesh and render it properly (the center and dimensions a bounding box for the model can be seen in "Inspector>Mesh Renderer" with the appropiate model selected). 

**Models can be deleted** by pressing the delete button on the Inspector window.

### Textures

The engine is capable of **converting PNG files to textures**. To asign a loaded texture to any sub-mesh of any model, you can click on the current texture in "Inspector>Mesh Renderer" and a menu will pop up where one can choose another texture to be displayed. 

**Textures can be deleted** by pressing the delete button in the Textures window. Any meshes using a deleted texture will be assigned the default checkers texture.

### Camera
- Change Near and Far render planes
- Change FOV

### Editor Camera Movement

Keyboard:
- **W/S:** Move Forward and Backward (using local axis of the camera)
- **A/D:** Move Left and Right (using local axis of the camera)
- **Q/E:** Move Up and Down (using local axis of the camera)

- **I/K:** Pitch Up & Down (using World axis)
- **J/L:** Yaw Left and Right (using World axis)
- **U/O:** Roll Anticlockwise and Clockwise (using World axis)

- **Left-Shift:** double movement speed

Mouse:
- **Left-Click**: Move Up/Down/Left/Right (using local axis of the camera)
- **Mouse-Wheel:** Move Forward/Backward (using local axis of the camera)
- **Right-Click:** Pitch/Yaw (using World axis)
- **Left-Alt + Left-Click:** Orbit around the selected object in the hierarchy

### Configuration Window

- Change Background Color
- Change Grid Color
- Show/Hide Grid
- Show Geometry in wireframe-mode/solid-mode

### Debug Tools Window
- Performance:
  - FPS histogram
  - Average ms & fps (last 30 samples)
- Renderer
  - Full window size
  - Scene view size
  - Enable/Disable Vsync
- Hardware
  - GPU info
- Libraries


## External Libraries

- [SDL 2.0.8](https://www.libsdl.org)
- [GLEW 2.1.0](http://glew.sourceforge.net)
- [ImGui-Docking 1.66 WIP](https://github.com/ocornut/imgui/tree/docking)
- [MathGeoLib 1.5](https://github.com/juj/MathGeoLib)
- [DevIL 180](http://openil.sourceforge.net/download.php)

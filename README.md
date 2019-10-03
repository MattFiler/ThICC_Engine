# Mario Kart DX12

<img align="right" src="https://img.itch.zone/aW1hZ2UvNDQ5OTcyLzIyNzc1MDQucG5n/original/YNAXtD.png" width="50%">

This project was to create an engine framework capable of running a Mario Kart style game, utilising DirectX12. Our team developed the "ThICC Engine": standing for Toolkit, Physics, Input, Camera, Collision (the main areas of the project we each worked on). The final engine and Mario Kart based game came together quite nicely over a few months of work.

You can download the final Mario Kart game build [via Itch by clicking here](https://mattfiler.itch.io/mario-kart-dx12), or alternatively this repo contains all source code and assets required to build the same project yourself. Full disclaimer, a number of image/model assets are ported from the WiiU build of Mario Kart 8 - if this causes any legal problems, we'll probably have to turn this to a code only repo.

## About the engine

<img align="right" src="https://i.imgur.com/91jrz4M.png" width="40%">

The main focus of this project was building the engine, so a significant focus was placed on producing reuseable systems rather than bespoke functionality for the Mario Kart project. We created a number of systems for physics, audio, asset management, and more - all of which are non-specific to the Mario Kart game and could allow the engine to be easily repurposed for another title.

The core of the engine backend relies on the "ThICC Toolkit" which allows access to most of the engine's core features through a GUI with a range of functionalities for asset importing (models/sounds/images/etc), configuration customisation (fonts/keybinds/cameras/glyphs), and a series of other things. We also embedded some more bespoke features into the engine toolkit, for example, item configurations.

Our asset pipeline was developed around a system I referred to as "asset comps": the idea of importing a series of assets which build up a compilation for a specific use. This "use" was then read-in by the engine itself to provide the assets for a certain entity. For example, a character would have a comp which specified its model, sounds, name, and more. When an asset is assigned to a comp it is tagged - asset tagging became useful to prevent the deletion of in-use assets, and also would be useful for releasing a project and making sure it does not ship with any development (or upcoming) assets.

<img align="right" src="https://i.imgur.com/pDkC8vm.png" width="20%">

Levels are created for the engine through Blender, which utilises our own plugin that I developed for placing points in the world representing collision volumes (for lap checkpoints), item boxes, and more. This is a nice graphical way of adding entities within the map without using magic numbers. A level in the engine is built up by one or two models, one model supporting mesh collision, and the other being effectively background scenery. This was implemented later in development as we reached memory caps with larger levels as just one SDKMESH.

The engine utilises the new SDKMESH2 format for PBR materials. All imported models have a bounding box automatically calculated, unless importing as a map, in which case mesh collision data is produced. Mesh collision data is stored in a proprietary "ThICC file", this is a binary representation of each triangle in the model, which the engine's mesh collision system traverses to calculate collision responses. Another "ThICC file" format is utilised for animated materials: these binary files work by storing a number of frames, and the timing intervals between them. The model renderer then applies this information at runtime when rendering each material.

The engine was built around DirectX12 and utilised a number of open source samples from Microsoft such as the DirectX Model Viewer. This project was useful for understanding the implementation of image-based lighting and resource banks.

## Engine features

This engine was a large project and my first dive into DirectX12 and lower level systems as a whole. If a feature interests you, feel free to get in contact and I'll be happy to discuss the implementation with you - we might both learn some things!

Our engine featured:

 - Full asset pipeline
  - Material editor
  - Asset management
  - Configuration editor
  - Asset previews
 - GUI toolkit
 - AI systems
 - Blender integration
 - Image-based lighting
 - Mesh collision & AABB collision
 - Optimised build target
 - Scene management
 - Audio system
 - Animated materials
 - ... and much more!

The project is fully open source, so code here covers not only the game and engine, but the toolkit as well.

## The team

<img align="right" src="https://img.itch.zone/aW1hZ2UvNDQ5OTcyLzIyNzc0OTkucG5n/original/QqWki%2F.png" width="50%">

This project was put together by:

 - [Toby Jones](https://github.com/Halichoerus) (mesh collision, AI, generalist systems, mapping)
 - [Matt Filer](https://github.com/MattFiler) (toolkit/pipeline, model loader, UI, scene manager, input)
 - [Evan Diamond](https://github.com/EvanDiamond101) (OBB collision, bespoke in-game items, generalist systems)
 - [Lewis Bond](https://github.com/lbondi7) (sounds, cameras, generalist systems)
 
 ## Final mentions
 
 The toolkit utilises a few third party libraries:
 
  - [FFmpeg](https://ffmpeg.org/) (GNU Lesser Public General Public License)
  - [DirectXTex](https://github.com/microsoft/DirectXTex) (MIT License) 
  - [DirectXMesh](https://github.com/microsoft/DirectXMesh) (MIT License) 
  - [Newtonsoft Json.NET](https://www.newtonsoft.com/json) (MIT License) 
  - [Costura Fody](https://github.com/Fody/Costura) (MIT License) 
  - [Helix Toolkit](https://github.com/helix-toolkit/helix-toolkit) (MIT License)

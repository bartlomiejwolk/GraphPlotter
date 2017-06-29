# GraphPlotter
Graph plotter debug module for UE4

Use it to plot debug graphs to the screen.

Licensed under MIT license. See LICENSE file in the project root.

![AnimationPath Tools](/Resources/cover_screenshot.png?raw=true "UE4 GraphPlotter graphs in Unreal Tournament")

Features
--------

- Draw multiple graphs at once
- Specify graph position and size
- Specify graph bg, line and reference line colors
- Specify range of displayed values
- Place reference line at any position on the graph
- Change graph bg color dynamically (through delegate)

[Video](https://youtu.be/AEIqbp3qvms "UE4 GraphPlotter module")<br>

Resources
---
* [Blog Post](https://wordpress.com/post/bartlomiejwolk.wordpress.com/1174)    

Quick Start
------------------

- Clone repository (or extract [zip package](https://github.com/bartlomiejwolk/GraphPlotter/archive/master.zip)) to any location in your project's `Source` folder
- Add _GraphPlotter_ module to `.uproject` file
- Add _GraphPlotter_ module to your game project  `.Target.cs` file
- Add _GraphPlotter_ module to your game module `.build.cs` file
- In a class that you want to debug, create class fields for `FGp_Graph` and `FGp_GraphPlotter`
- In class constructor initialize `FGp_Graph` instance
- In your debug method, eg. `AActor::DisplayDebug()` call `<graph_instance>::AddDataPoint(float)` and `<graphplotter_instance>::Plot(UCanvas, FGp_Graph)`
to plot debug data to the screen.

Check [Blog Post](https://wordpress.com/post/bartlomiejwolk.wordpress.com/1174) to see how _GraphPlotter_ can be added to _Unreal Tournament_.

Help
-----

Just create an issue and I'll do my best to help.

Contributions
------------

Pull requests, ideas, questions and any feedback at all are welcome.

Versioning
----------

Example: `v0.2.3f1`

- `0` Introduces breaking changes.
- `2` Major release. Adds new features.
- `3` Minor release. Bug fixes and refactoring.
- `f1` Quick fix.

# 2D Rendering Engine

This is my attempt to architect a 2D Rendering Engine to serve as a base for future graphics projects (and also to learn C++).

## Features:

- GPU instanced rendering
- Virtual classes that follows pipeline Init->Update->Render.
- Basic physics system with Spatial Hash Grid to speed things up

## How to build:

Using Premake:

- Edit GenerateProject.bat to suit your IDE and run it.

## How to use:

### To start:

1. Create class that inherits from Engine class
2. Override Init and Update methods

### To create Objects (entities who possess behavior):

1. Create classes that inherits from Object
2. Override Init and Update methods

The inherited classes get automatically added into the pipeline and have their methods called on respective events.

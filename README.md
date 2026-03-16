# miniRT
42 miniRT (slightly extended)


| ![eflectionglasssphere](https://github.com/antolp/miniRT/blob/main/docs/gallery/reflectionglasssphere.png) | ![roughreflectioncloudy](docs/gallery/roughreflectioncloudy.png) |
|---|---|

See the *.rt* file format spec :  [scene initialization guide](docs/sceneinit_guide.md)

See the render-loop control flow graph : [render-loop diagram](docs/renderloop_ctrlflow.md).

See the data structure diagram : [data-struct diagram](docs/data_struct.md).


---
### Build :
`--recurse-submodules` required :

```
git clone --recurse-submodules git@github.com:antolp/miniRT.git
```
Then  :
```
make
```
In order to have minilibx compiled properly, you may need to install `xorg`, `libxext-dev` and `libbsd-dev`. Those are the name of the required packages on apt, the name may differ depending on your package manager.

Linux only because of minilibx, but you can make it work on other Unix OSs (like MacOS !) (tested on macOS 15).

---
### Usage :
```
./miniRT file.rt
```
Then, press `h` to see the different commands :
 - Press `e` to switch to edit mode, press `r` to start rendering from edit mode
 - On camera mode, can move the camera with `z`,`q`,`s`,`d`,`t`,`g`. 
 - You can edit the scene by pressing `1` to edit objects, or `2` to edit lights, and `0` to go back to camera mode.
 - `m` lets you bypass steps of shading pipeline, e.g fresnel, reflections, etc.
 - `u` lets toggles supersampling, which works great but is painfully slow

(the edits cannot be saved to a .rt file unless you write them down, the image cannot be saved unless you take a screenshot or something)

There's a bunch scenes in the `./scenes` directory, you can also write your own by following the format specification.

| ![transparency](https://github.com/antolp/miniRT/blob/main/docs/gallery/transp.png) | ![reflectionglasssphere](docs/gallery/transluscent_roughrefractive_spheres.png) |
|---|---|

# miniRT
42 miniRT (slightly extended)

| ![roughreflectioncloudy](docs/gallery/roughreflectioncloudy.png) | ![reflectionglasssphere](docs/gallery/reflectionglasssphere.png) |
|---|---|

See the *.rt* format spec :  [scene initialization guide](docs/sceneinit_guide.md)

See the render-loop control flow graph : [render-loop diagram](docs/renderloop_ctrlflow.md).

See the data structure diagram : [data-struct diagram](docs/data_struct.md).


---

### Build :
--recurse-submodules required

```git clone --recurse-submodules git@github.com:antolp/miniRT.git ```

In order for minilibx to compile, you may need to install `xorg`, `libxext-dev` and `libbsd-dev`. Those are the name of the required packages on apt, the name may differ depending on your package manager.

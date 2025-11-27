# miniRT
42 miniRT (slightly extended)

some textures (temporary link) : https://we.tl/t-sZhykf5hoa

See the *.rt* file format spec :  [scene initialization guide](docs/sceneinit_guide.md)

See the render-loop control flow graph : [render-loop diagram](docs/renderloop_ctrlflow.md).

See the data structure diagram : [data-struct diagram](docs/data_struct.md).


---
### Build :
--recurse-submodules required :

```
git clone --recurse-submodules git@github.com:antolp/miniRT.git
```
Then  :
```
make
```
In order to have minilibx compiled properly, you may need to install `xorg`, `libxext-dev` and `libbsd-dev`. Those are the name of the required packages on apt, the name may differ depending on your package manager.

## Scene Data Structure Diagram

```mermaid
---
config:
  theme: dark
  look: classic
  class:
    hideEmptyMembersBox: true
---

%%{init: {
  "theme": "dark",
  "flowchart": { "curve": "basis", "useMaxWidth": true, "nodeSpacing": 40, "rankSpacing": 60 },
  "themeVariables": {
    "background": "#0b1221",
    "primaryColor": "#111827",
    "primaryTextColor": "#e5e7eb",
    "primaryBorderColor": "#60a5fa",
    "secondaryColor": "#0f172a",
    "tertiaryColor": "#1f2937",
    "lineColor": "#60a5fa",
    "edgeLabelBackground": "#0f172a",
    "clusterBkg": "#0f172a",
    "clusterBorder": "#334155",
    "fontSize": "14px"
  }
}}%%

classDiagram
direction TB

class t_renderer {
	mlx;
	win;
	t_img			img;
	t_render_mode	mode;
	int				resolution_scale;
	bool			is_rendering;
	bool			render_done;
	bool			has_drawn_realtime;
	bool			supersampled;
	int				shading_flag;
}

class t_scene {
  camera: t_camera
  objects: t_list*
  clights: t_list*
  ambient_color: t_color
  ambient_ratio: float
  is_rendering: bool
  background_color: t_color
  skybox: t_texture
}

class t_light {
  position: t_vec3
  intensity: double
  color: t_color
}

class t_camera {
  position: t_vec3
  direction: t_vec3
  fov: double
}

class t_object {
  type: t_object_type
  shape: void \*
  material: t_material
  intersect(s_object*, t_ray*, double*): bool
  get_normal(s_object*, t_vec3*, t_vec3*): void
  get_uv(s_object*, t_vec3*, t_vec2*): bool
}

%%material prop
class t_material {
  +base_color: t_color
  +shininess: double
  +specular_strength: double
  +reflectivity: double
  +ior: double
  +refractivity: double
  +texture: t_texture
  +bump_maps: t_texture
  +bump_strength: double
}

class t_texture {
  +type: t_texture_type
  +data: void\*
}

class t_texture_image {
    void   *mlx_img;
    char   *addr;
    int     bpp;
    int     line_len;
    int     endian;
    int     width;
    int     height;
    int     bgra;
}

class t_checkerboard {
    t_color color1;
    t_color color2;
    double  scale_u;
    double  scale_v;
}

namespace Material {
  class t_material
  class t_texture
  class t_texture_image
  class t_checkerboard 
  class data
}

%%object shape

class t_sphere {
	t_vec3		center;
	double		radius;
}

class t_plane {
	t_vec3		point;
	t_vec3		normal;
}

class t_cylinder {
	t_vec3		center;
	t_vec3		axis;
	double		radius;
	double		height;
}

class t_cone {
	t_vec3		apex;
	t_vec3		axis;
	double		angle;
	double		height;
}

class t_triangle {
	t_vec3			p0;
	t_vec3			p1;
	t_vec3			p2;
	t_tri_uv_mode	uv_mode;
}	

namespace Objects {
  class t_object
  class t_sphere
  class t_plane
  class t_cylinder
  class t_cone
  class t_triangle
  class shape
}

%% Notes
note for t_texture "data pointer is set to the texture data payload"
note for t_object "function pointers are set per shape"

%%core containers
shape "1" ..* "1" t_object : shape

t_plane  "1" .. "1" shape
t_sphere  "1" .. "1" shape
t_cylinder  "1" .. "1" shape
t_cone  "1" .. "1" shape
t_triangle"1" .. "1" shape


t_object "1" *-- "1" t_material : material
t_material "1" *-- "1" t_texture : texture
t_material "1" *-- "1" t_texture : bump_maps
t_texture "1" *-- "1" data : data
data "1" .. "1" t_texture_image
data "1" .. "1" t_checkerboard

t_scene "1" *-- "1" t_camera : camera
t_scene "1" *-- "0..N" t_object : objects
t_scene "1" *-- "0..N" t_light : lights
```

---

### Some explanations

The ***t_scene*** struct stores informations about the scene the program should render. It contains :
- a ***t_camera*** : the point, direction and angle from which the ray should be shot
- a list of ***t_lights***
- a list of ***t_objects***
- some infos about ambiant light
- some infos about the background of the scene : a solid color or a sky texture

The ***t_object*** struct stores information about an object in the scene. It uses a (*sort of*) polymorphic design, which allows it to describe any type of object. It contains : 
- a type : spheres, planes, cylinders, cones or triangles
- a *`void *shape`* pointer to a struct containing informations on the shape of the object it describes (***t_sphere***, ***t_plane***, ***t_cylinder***, ***t_cone*** or ***t_triangle***)
- three pointers to three functions :
  - a function pointer *`intersect()`*, initialized to *`intersect_sphere()`*, *`intersect_plane`*, etc.
  - a function pointer *`get_normal()`*, initialized to *`get_normal_cone()`*, *`get_normal_cylinder`*, etc.
  - a function pointer `get_uv()`, initialized to *`get_uv_sphere()`*, *`get_uv_triangle`*, etc.
- a ***t_material*** : informations on how to shade the object.

The ***t_material*** contains information on how to  shade a pixel resulting from the interesection of a ray and an object. It contains :
- a ***t_color*** base_color, the color of the surface of the object before texturing.
- a shininess and specular_strength, these number are used to compute the Blinn-Phong equation for specular highlight.
- an ior, (indice of refraction), determines how much the rays are bent when entering a medieum (the object), used to compute the fresnel equation and refractive materials.
- a reflectivity ratio, determines if and how much a surface reflects rays.
- a refractive ratio, determines if and how much a surface is translucide.
- a ***t_texture*** that defines the texture of the surface.
- a ***t_texture*** that defines a bump map.

Finally, the ***t_texture*** contains informations on textures. The *`void *data`* points to either a ***t_checkboard*** or a ***t_texture_image*** allocated in heap. 

***t_checkboard*** describes the characteristics of a checkboard texture : the color of each tile and their scale on each axis.

***t_texture_image*** stores a pointer to the texture data in heap, and infos used to parse the right bytes during the uv-wrapping process (may add some more details here).

For infos on the data primitives (t_color, t_vec3, etc), see the section 2) of this document : [*`.rt`* Scene Format](../docs/sceneinit_guide.md).


---
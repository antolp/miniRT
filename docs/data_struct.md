## Scene Data Structure Diagram

(Missing the shape structs and the texture payloads, because it looked bad and i have no idea how to format this mermaid mess).

```mermaid
classDiagram
direction LR

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
  shape: void*
  material: t_material
  intersect(s_object*, t_ray*, double*): bool
  get_normal(s_object*, t_vec3*, t_vec3*): void
  get_uv(s_object*, t_vec3*, t_vec2*): bool
}

class t_material {
  base_color: t_color
  shininess: double
  specular_strength: double
  reflectivity: double
  ior: double
  refractivity: double
  texture: t_texture
}

class t_texture {
  type: t_texture_type
  data: void*
}

%%core containers
t_scene "1" *-- "1" t_camera : camera
t_scene "1" *-- "0..N" t_object : objects
t_scene "1" *-- "0..N" t_light : lights
t_object "1" *-- "1" t_material : material
t_material "1" *-- "1" t_texture : texture

%% Notes
note for t_object "function pointers are set per shape"
note for t_texture "data pointer is set to the texture data payload"
```

---

### Some explanations

The ***t_scene*** struct stores informations about the scene the program should render. It contains :
- a camera : the point, direction and angle from which the ray should be shot
- a list of ***t_lights***
- a list of ***t_objects***
- some infos about ambiant light
- some infos about the background of the scene : a solid color or a sky texture

The ***t_object*** struct stores information about an object in the scene. It uses a (*sort of*) polymorphic design, which allows it to describe any type of object. It contains : 
- a type : spheres, planes, cylinders, cones or triangles
- a pointer to a struct containing informations on the shape of the object it describes (***t_sphere***, ***t_plane***, ***t_cylinder***, ***t_cone*** or ***t_triangle***)
- three pointers to three functions :
  - a function pointer *intersect()*, initialized to *intersect_sphere()*, *intersect_plane*, etc.
  - a function pointer *get_normal()*, initialized to *get_normal_cone()*, *get_normal_cylinder*, etc.
  - a function pointer *get_uv()*, initialized to *get_uv_sphere()*, *get_uv_triangle*, etc.
- a ***t_material*** : informations on how to shade the object.

Finally, the ***t_material*** contains information on how to  shade a pixel resulting from the interesection of a ray and an object. It contains :
- a ***t_color*** base_color, the color of the surface of the object.
- a shininess and specular_strength, these number are used to compute the Blinn-Phong equation for specular highlight.
- an ior, (indice of refraction), determines how much the rays are bent when entering a medieum (the object), used to compute the fresnel equation and refractive materials.
- a reflectivity ratio, determines if and how much a surface reflects rays.
- a refractive ratio, determines if and how much a surface is translucide.

The ***t_texture*** contains informations on textures, whether they are images or procedural checkerboards

See the render-loop control flow graph : [render-loop diagram](../docs/renderloop_ctrlflow.md).

---
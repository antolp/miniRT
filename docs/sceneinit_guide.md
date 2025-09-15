# miniRT `.rt` Scene Format

> **Purpose**  
> This document defines an **extended** `.rt` scene format used by this miniRT.  
> It is a *superset* of the 42 subject’s format: any valid subject file should load here unchanged.  

---

## 1) Quick Overview

- One **ASCII** file with extension `.rt`.
- **One element per line**
- **Comments**: any line starting with `#` is ignored.
- **Whitespace**: one or more spaces between tokens.
- **Commas** inside vectors or colors spaces around commas are allowed.
- Number of time an element can be present in a scene :
  - `A` (Ambient): **0..1**
  - `C` (Camera): **exactly 1**
  - `L` (Light): **0..N**
  - Shapes (`sp`, `pl`, `cy`, `co`, `tr`): **0..N**
  - `BG` (Background color): **0..1**
  - `SB` (Skybox): **0..1**

If optional material/texture keys are not present, **defaults** are applied (See 7.).

Here is a quick overview of all the Identifiers, each with their expected associated values :

- **A**: `ratio` `[0..1]`, `R,G,B` `[0..255]`

- **C**: `pos`, `dir`, `fovDeg` optional (default `60`)

- **L**: `pos`, `intensity` `[0..1]`, `R,G,B` `[0..255]`

- **BG**: `R,G,B` `[0..255]`

- **SB**: `path=<.xpm>`

- **sp**: `center`, `diameter>0`, `R,G,B` + optional material/texture keys

- **pl**: `point`, `normal`, `R,G,B` + optional keys

- **cy**: `center`, `axis`, `diameter>0`, `height>0`, `R,G,B` + optional keys

- **co**: `apex`, `axis`, `angleDeg`, `height>0`, `R,G,B` + optional keys

- **tr**: `p0`, `p1`, `p2` , `R,G,B`, optional `uv=0` uv mode and keys

More precise description below.

---

## 2) Some data primitives

- **Color (RGB)**: integers **0–255**  
  Example: `255,0,21`
- **Vector (position/direction/axis)**: floats (e.g., `-12.5,0,3`)  
  - Directions/axes must be **non-zero**.
- **Angle** : degrees e.g. `60` (Camera FOV), `45.0` (Cone half-angle).
- **Ratios**: floats in **[0.0, 1.0]** (ambient ratio, light intensity).
- **Sizes**: strictly **> 0** (diameter, height).

---

## 3) In-depth Elements Initialization & Examples

This is pretty long, it's not necessary to read all of this to undersand how the file work. This only serves as a way to document how things should work.

Each section shows:
- a textbook format explaining each field
- a real example that would be valid in this format.

## 3.1 Scene Environnemt :

### 3.1.1 Ambient Light : `A`

    A <ratio> <R,G,B>

- `ratio` in `[0.0, 1.0]`
- `R,G,B` are integers `[0..255]`
- **At most one per file**

**Example**

    A 0.15 255,255,255

---

### 3.1.2 Camera : `C`

    C <x,y,z> <dx,dy,dz> [fov_deg]

- Position `<x,y,z>` floats
- Direction `<dx,dy,dz>` **non-zero**
- Optional `fov_deg` in `(10, 180)`; **defaults to 60** if omitted
- **Exactly 1 per file**

**Examples**

	#Camera looking towards the Z axis, slightly upward
    C 0,10,0 0,0.2,1

---

### 3.1.3 Light : `L`

    L <x,y,z> <intensity> <R,G,B>

- Position `<x,y,z>` floats
- `intensity` in `[0.0, 1.0]`
- `R,G,B` integers `[0..255]`

**Examples**

	#Strong purple light
    L -12,18,10 0.99 255,180,220

---

### 3.1.4 Background Color : `BG`

    BG <R,G,B>

- Background used when no skybox is present
- **Multiplicity**: `0..1`

**Example**

	#Dark blue background
    BG 10,10,40

---

### 3.1.5 Skybox : `SB`

    SB path=<assets/skyboxes/your_skybox.xpm>

- Sets the scene’s skybox (XPM image)
- Path should not contain spaces
- **Multiplicity**: `0..1`

**Example**

    SB path=assets/skyboxes/clear_sky_2k.xpm

---
## 3.2 Objects :

### 3.2.1 Sphere : `sp`

    sp <x,y,z> <diameter> <R,G,B> [material/texture keys...]

- Center `<x,y,z>` floats
- `diameter > 0`  (internal radius = diameter/2)
- Base color `<R,G,B>` integers `[0..255]`
- Optional material/texture keys appended

**Examples**

	#Earth textured sphere with a radius of &
    sp 0,8,0 8.0 255,255,255 tex=image(path=assets/2k_earth_daymap.xpm) spec=0.6 shine=64

---

### 3.2.2 Plane : `pl`

    pl <x,y,z> <nx,ny,nz> <R,G,B> [material/texture keys...]

- Point on plane `<x,y,z>` floats
- Normal `<nx,ny,nz>` **non-zero**
- Base color `<R,G,B>` integers `[0..255]`
- Optional material/texture keys appended

**Examples**

	#Checkerboard floor
    pl 0,-2,0 0,1,0 255,255,255 tex=checker(c1=220,220,220;c2=30,30,30;su=2;sv=2) spec=0.2 shine=16

---

### 3.2.3 Cylinder : `cy`

    cy <x,y,z> <ax,ay,az> <diameter> <height> <R,G,B> [material/texture keys...]

- Center `<x,y,z>` floats
- Axis `<ax,ay,az>` **non-zero**
- `diameter > 0`, `height > 0`
- Base color `<R,G,B>` integers `[0..255]`

**Example**

	# Metallic cylinder (sort of) 
    cy 8,-2,-3 0,1,0 2.0 6.0 200,180,160 refl=1.0 ior=0.18 spec=1.0 shine=256

---

### 3.2.4 Cone : `co`  *(bonus shape)*

    co <x_apex,y_apex,z_apex> <ax,ay,az> <angleDeg> <height> <R,G,B> [material/texture keys...]

- Apex position floats
- Axis `<ax,ay,az>` **non-zero**
- `angleDeg` = **half-angle** in degrees, `(0, 89.9]` recommended
- `height > 0`
- Base color `<R,G,B>` integers `[0..255]`

**Example**

	# Flat, shiny cone
    co -10,-2,-10 0,1,0 25.0 8.0 255,150,60 spec=0.9 shine=64

---

### 3.2.5 Triangle — `tr`  *(bonus shape)*

    tr <x0,y0,z0> <x1,y1,z1> <x2,y2,z2> <R,G,B> [uv=<mode>] [material/texture keys...]

- Three vertices
- Base color `<R,G,B>` integers `[0..255]`
- Optional `uv=<mode>`; default `uv=0` = `TRI_UV_ORTHONORMAL`
- A triangle is facing the camera when its vertices are arranged **anti-clockwise**, only useful when dealing with textures.

**Example**

    tr 0,0,-5 3,0,-5 0,3,-5 255,255,255 uv=0 tex=checker(c1=240,240,240;c2=40,40,40;su=4;sv=4) spec=0.3 shine=32

---


## 4) Materials & Textures (Per-Object, Optional)

Append **key=value** tokens after the object’s base fields, separated by spaces, in any order.  
If you omit them, **defaults** apply (see 7. Defaults Table).

### Supported keys

- `refl=<float>` — **reflectivity** in `[0,1]` (ignored if `refr>0`)
- `refr=<float>` — **refractivity** in `[0,1]`; if `refr>0` the surface is transmissive
- `ior=<float>` — **index of refraction** (used by Fresnel/refraction if `refr>0`)
- `spec=<float>` — **specular strength** `[0,1(+)]` (Phong/Blinn-Phong amplitude)
- `shine=<float>` — **shininess** (specular exponent, e.g., 16–256)
- `tex=checker(c1=R,G,B;c2=R,G,B;su=S;sv=S)` — procedural checkerboard, c1 and c2 are the colors of the tiles, s1 and s2 are the scale of each tile in u, v 2d coordinates
- `tex=image(path=assets/image.xpm)` — XPM image texture
- For triangles only: `uv=0` — UV mode (`0` = `TRI_UV_ORTHONORMAL`, default), 1 and 2 allows mapping a whole image, triangles mapped to each opposite squared triangles of the image. If the triangles aren't square or arent set-up properly, this will fuck up the image proportion.

> **Fresnel rule**: 
If `refl = 0` and `refr = 0` , the Fresnel equation will not be visible.

>The IOR values are **not** physically accurate, especially with reflections... Coherent values will differ greatly whether you try to initialize a `reflective` or `refractive` object !! Refer to the recommended values in (7.1).

---

## 5) Exemple Scene (Feature Showcase)

This scene demonstrates:
- **Skybox**
- **Multiple lights** with colors
- **Procedural texture** (checker)
- **Image texture** (XPM images)
- **Reflection** (mirror)
- **Refraction** (glass with Fresnel)
- **Transparent shadows** (from refractive objects)
- **Phong specular highlights** (spec/shine)
- **Multiple shapes**: plane, sphere, cylinder, cone, triangle
	```Scene
	# --- Environment ---
	A 0.12 255,255,255
    C 0,2,15 0,0,-1 60
    BG 10,10,20
    SB path=assets/skyboxes/sky_2.xpm

    # --- Lights ---
	L -12,18,10 0.9 255,240,220
    L 15,8,-5 0.6 180,200,255
    L 0,5,0 0.3 255,120,120

    # --- Geometric Objects ---
    # Ground: soft checker, low specular :
    pl 0,-2,0 0,1,0 255,255,255 tex=checker(c1=220,220,220;c2=30,30,30;su=2;sv=2) spec=0.2 shine=16

    # Mirror wall at the back :
    pl 0,0,-30 0,0,1 255,255,255 refl=1.0 ior=0.01 spec=1.0 shine=256

    # Textured earth sphere :
    sp -6,0,-8 8.0 255,255,255 tex=image(path=assets/2k_earth_daymap.xpm) spec=0.6 shine=64

    # Refractive glass sphere (Fresnel reflection; reflectivity ignored) :
    sp 4,0,-10 6.0 200,180,255 refr=0.0005 ior=1.02 spec=1.0 shine=128

    # Metallic cylinder :
    cy 8,-2,-3 0,1,0 2.0 6.0 200,180,160 refl=1.0 ior=0.18 spec=1.0 shine=256

    # Colored cone :
    co -10,-2,-10 0,1,0 25.0 8.0 255,150,60 spec=0.8 shine=64

    # Two triangles displaying an image :
    tr 25,0,0 0,0,0 25,25,0 255,255,255 uv=0 tex=image(path=assets/image.xpm) uv=1 reflectivity=1 ior=1
    tr 0,25,0 25,25,0 0,0,0 255,255,255 uv=0 tex=image(path=assets/image.xpm) uv=2 reflectivity=1 ior=1
	```


### Notes :
- If the skybox fails to load, the renderer falls back to `BG`, if non defined, defaults to `0, 0, 0`.
- If an object omits all material/texture keys, it renders with default material (Lambertian, no texture).
- If `refractivity > 0`, the renderer computes transmission + Fresnel reflection; `reflectivity` is ignored by design.

---

## 6) Error Handling

> Juste quelque idées, à ettendre 

- Unknown identifier
- Wrong field count or order for a known identifier
- Non-numeric / malformed numbers
- Vectors that are zero or cannot be normalized where required
- Out-of-range values (e.g., negative diameter, `intensity` outside `[0,1]`, `RGB` outside `[0..255]`)
- Duplicate elements where only one is allowed in a scene (e.g., multiple `SB`)

---

## 7) Defaults Table (applied when optional keys are omitted)

> Should add preset for when there's only SOME missing values. Example refr > 0 but no IOR

- **Scene**
  - Ambient (if missing): `ratio=0.0`, `color=0,0,0`
  - Background (if missing): `BG=0,0,0`
  - Skybox (if missing): none

- **Camera**
  - `fov=60` (if omitted)

- **Material (per object)**
  - `base_color`: required by the object
  - `spec=0.0` (Lambertian by default)
  - `shine=32`
  - `refl=0.0`
  - `refr=0.0`
  - `ior=1.0` (here ior = 1.3 if refl > 0, ior = 1.02 if refr > 0.0)
  - `tex=none`
  - Triangle `uv=0` (`TRI_UV_ORTHONORMAL`)

## 7.1 Some decent looking preset

When `refl > 0` :

-	plastic	: shine =  16; spec = 0.3; refl = 0.3; ior = 1.00;
-	glossy	: shine =  32; spec = 1.0; refl = 1.0; ior = 1.52;
-	shiny	: shine = 128; spec = 1.0; refl = 1.0; ior = 2.42;
-	mirror	: shine = 256; spec = 1.0; refl = 1.0; ior = 0.01;
-	metallic: shine = 256; spec = 1.0; refl = 1.0; ior = 0.18;

When `refr > 0` :
- slightly disruptive glass	: ior = 1.001 to 1,03;
- completely solid glass ball : ior = 1.3 to 1.52;

---

Voila voila
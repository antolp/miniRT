## Top-Level Render Loop Control Flow Diagram

Following the logic of the *render_loop* function and subsequent function calls and branches (not perfectly accurate).

First, we set up the ray-tracing conditions based on rendering modes. If the mode is RENDERING_REALTIME, a frame will be drawn without any shading whatsoever, and in a low resolution format. If the mode is RENDERING_HQ, a real shaded image will be rendered. If supersampled is activated, for one pixel, multiple rays will be shot and blended in (supersampling anti-aliasing, see SSAA on wikipedia).

Then, for each pixel, we trace a ray, and if the ray intersects an object, we follow the shading pipeline. Each operation in the shading pipeline starting with *compute_comp* return a componment that is either added or blended into the color of the pixel, using the material properties of the surface of the object the ray has hit.

See the data structure diagram : [data-struct diagram](../docs/data_struct.md).

---

```mermaid
%%---
%%config:
%%  theme: redux-dark
%%  look: neo
%%---

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

flowchart

subgraph shading[ **Shading pipeline** : ]
    direction LR
    ShadePixel["**Shade Pixel**
        *shade_pixel()* 
        Entry to pixel shading logic."]
    %%shade pipeline
    ShadePixel --> FixNormal["Fix Normal Side"]
    FixNormal --> UseTexture{Textured ?}

    %%get color of surface at hit point
    GetHitColor["**Get Hit Color**
        *get_hit_color()* 
        Returns color of the object 
        surface at hit point."]
    UseTexture -->|No| UseBaseColor["Use Base Color"]
    UseTexture -->|Yes| GetHitColor

    %%Bumpmap
    ApplyBump["**Apply Bump From Image**
        *apply_bump_from_image()* 
        Modifies the passed normal 
        vector according to a bump 
        map texture."]
    BumpOn{Bump Maps ?}
    BumpOn --> |Yes| ApplyBump
    BumpOn --> |No| ComputeDiffuse
    ApplyBump --> ComputeDiffuse

    %%routing hit color to diffuse
    GetHitColor --> HitNode@{ shape: sm-circ, label: "" }
    HitNode -.-> CheckerColor["Checker Color"]
    HitNode -.-> ImageColor["Image Color"]
    CheckerColor -->BumpOn
    ImageColor -->BumpOn
    UseBaseColor ---->BumpOn

    %%Diffuse lighting
    ComputeDiffuse["**Compute Diffuse**
        *compute_diffuse_lighting()* 
        Computes the effect of Lambertian lighting, and
        full/transparent shadows at hit point."]
    %%Specular
    ComputeSpecular["**Compute Specular**
        *compute_specular_component()* 
        Computes the specular 
        highlight from single light using 
        the material properties of the 
        object the Blinn-Phong model."]
    ComputeDiffuse --> SpecOn{Specular On?}
    SpecOn -->|Yes| ComputeSpecular
    SpecOn -->|No| RefractOn

    ComputeSpecular --> RefractOn

    %%refraction or reflection (exclusive by your rule)
    ComputeRefract["**Compute Refraction**
        *compute_refraction()* 
        Compute refraction at hit
        point, traces a secondary 
        ray using the material
        properties of object, 
        computes a custom fresnel 
        equation."]
    RefractOn{Refractive?}
    RefractOn -->|Yes| ComputeRefract
    RefractOn -->|No| ReflectOn{Reflective?}

    ComputeRefract ----> ReturnHit([Return Color])

    ComputeReflect["**Compute Reflection**
        *compute_reflection()* 
        Compute reflection at hit
        point, traces a secondary 
        ray and computes the 
        fresnel equationusing the 
        material properties of 
        object."]
    ReflectOn -->|Yes| ComputeReflect
    ReflectOn -->|No| ReturnHit

    ComputeReflect --> ReturnHit
end

subgraph tracing[ **Ray-tracing** : ]
    PipelineInfo@{ shape: rounded, label: "This routine runs for each
        pixel on an image, 480 000 times for an image
        of 600*800 resolution."}
    PipelineInfo ~~~ HitCheck
    TraceRay["**Trace Ray**
        *trace_ray()* 
        Trace a ray and tests 
        intersection with every 
        object in the scene
        (pure brute force)."]
    TraceRay --> HitCheck{Hit Anything?}

    %%miss path
    GetBackground --> ReturnMiss(["Return Color"])
    HitCheck -->|No| GetBackground["Get Background"]
    %%hit path
    HitCheck -->|Yes| ShadePixel
end

%%top-level render loop and mode switch
RenderLoop@{ shape: circle, label: "**Render Loop**"} --> ModeChoice{Rendering Mode ?}
ModeChoice -->|RENDER_REALTIME| RenderRealtime["Render Realtime Frame"]
ModeChoice -->|RENDER_HIGH_QUALITY| RenderHQ["Render High Quality Frame"]
ModeChoice -->|RENDER_TEST| RenderTest["Render Test Frame"]

%%raltime branch
RenderRealtime --> FirstRT?{First Realtime?}
FirstRT? -->|no| ReturnRT([Return])
FirstRT? --->|yes| RenderDown@{ shape: rounded, label: "Render Downsampled
Calls trace ray N times for one pixel, skips shading path." }

%%HQ branch (simple frame and supersampled)
RenderHQ --> BusyOrDone?{Busy Or Done?}
BusyOrDone? -->|no| Supersampled?{Supersampled?}
BusyOrDone? -->|yes| ReturnHQ([Return])
Supersampled? -->|no| RenderFull@{ shape: rounded, label: "Render Classic
    Each pixel gets one ray, full shading path" } 
Supersampled? -->|yes| RenderSuper@{ shape: rounded, label: "Render Supersampled 
    Repeat render_full_frame path N times, accumulate into one pixel, then put said pixel." }



RenderDown --> CreateRay["Create Ray For Pixel"]
RenderFull --> CreateRay
RenderSuper --> CreateRay

CreateRay --> TraceRay

shading --> PutPixel["Put pixel color in image buffer"] --> RenderLoop
```
---
minecraft rendering algorithm

https://www.minecraftforum.net/forums/minecraft-java-edition/suggestions/15266-rendering-engine-speed-up-with-theorized-algorithm

https://www.reddit.com/r/Minecraft/comments/kfe8y8/java_performance_vs_bedrock_performance_in_a/

https://stackoverflow.com/questions/3693407/culling-techniques-for-rendering-lots-of-cubes

https://www.reddit.com/r/gamedev/comments/eez33/how_does_minecraft_render_the_world/

https://gamedev.stackexchange.com/questions/103779/how-does-minecraft-render-chunks

https://gamedev.stackexchange.com/questions/12938/how-can-i-optimise-a-minecraft-esque-voxel-world

https://gamedev.stackexchange.com/questions/11976/is-a-voxel-engine-appropriate-for-a-minecraft-like-game

https://gamedev.stackexchange.com/questions/12669/working-with-lots-of-cubes-improving-performance

https://gamedev.stackexchange.com/questions/8096/how-to-get-the-blocks-seen-by-the-player

https://forum.thegamecreators.com/thread/175623

marching cubes algorithm
https://pl.wikipedia.org/wiki/Algorytm_maszeruj%C4%85cych_sze%C5%9Bcian%C3%B3w
https://pl.wikipedia.org/wiki/Woksel

Historia:

https://www.wired.com/2013/11/minecraft-book/

https://interactive-stag.wired.com/www-wired-com__2013__11__minecraft-book/Infiniminer-660x495.png

clone infiminer


===============================================================================================================

1. krótka historia 
    3. dzielenie świata (obliczenia chunków - ile bloków, trójkątów)
    2. rotowanie bloków
2. ogólne informacje o grafice komputerowej
    1. mnożenie macierzy i wektory (+ hitbox)
    5. z-buffer i alternatywy
    2. pipeline
    3. shadery, w tym geometry shader (GPU szybsze od CPU, przekazywanie wielu punktów wolniejsze niż przekazanie jednego i klonowanie pkt na samej karcie graficznej)
    4. nakładanie tekstury, zamiast wyliczania kształtu na podstawie pkt (prykład z kulą ziemską, albo że każdy pixel tekstury to były by 2 trójkąty)
3. zarządzanie wyświetlaniem
    1. nie wyświetlanie ścian kiedy bloki się stykają
    2. wyświetlanie tylko chunk w zasięgu
    3. podział świata na fragmenty, używanie oct/quad Tree
    4. zmienny LOD (level of detail) + mipmapy
4. zarządzanie pamięcią
    1. garbage collector jest wolny, lepiej ręcznie (java vs bedrock)
    2. lokalność cache przy ładowaniu chunk
    3. wielowątkowość

===============================================================================================================


* rotowanie bloków trawy, żeby nie powtarzał się ten sam układ [link1](https://user-images.githubusercontent.com/61069740/88026810-af6c3c00-cb36-11ea-8da0-d6e929318aa2.jpg), [link2](https://user-images.githubusercontent.com/61069740/87943012-ba26c280-ca9d-11ea-8f52-055c76372ad8.jpg)

* przedstawienie pipeline i shaderów
[pipeline1](https://www.researchgate.net/profile/Christoph-Guetter/publication/235696712/figure/fig1/AS:299742132228097@1448475501091/The-graphics-pipeline-in-OpenGL-consists-of-these-5-steps-in-the-new-generation-of-cards.png)
[pipeline2](https://vulkan-tutorial.com/images/vulkan_simplified_pipeline.svg)
[pipeline3](https://media.geeksforgeeks.org/wp-content/uploads/openGL_pipeline.png)
[geeks](https://www.geeksforgeeks.org/opengl-rendering-pipeline-overview/)

* przekształcanie obrazu świata do monitora (mnożenie macierzy)
[openGL](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)
[link?](https://learnopengl.com/Getting-started/Coordinate-Systems)

* hitbox i vector wzroku ^
[picture](https://planetsmarts.com/wp-content/uploads/2021/05/Minecraft-How-to-show-hitboxes-or-collision-zones-and-what.jpg)

* depth buffer / z-buffer
In a 3d-rendering pipeline, when an object is projected on the screen, the depth (z-value) of a generated fragment in the projected screen image is compared to the value already stored in the buffer (depth test), and replaces it if the new value is closer. It works in tandem with the rasterizer, which computes the colored values. The fragment outputted by the rasterizer is saved if it is not overlapped by another fragment. 
The Z-buffer is a technology used in almost all contemporary computers, laptops, and mobile phones for performing 3D computer graphics. The primary use now is for video games, which require fast and accurate processing of 3d scenes. The Z-buffer is implemented in hardware within consumer graphics cards.
[wiki](https://en.wikipedia.org/wiki/Z-buffering)

* pozycja -> punkt -> kopia punktu na sześcian -> nałożenie tekstur (duplikowanie punktu w shader)
[geometryShader](https://github.com/keijiro/StandardGeometryShader)

* nie wyświetlanie ścian kiedy bloki się stykają (łączenie bloków w większe figury)
[link](https://www.minecraftforum.net/forums/minecraft-java-edition/suggestions/15266-rendering-engine-speed-up-with-theorized-algorithm)

* drzewiasta struktura do przetrzymywania zestawów bloków (chunków) pozwalajaca szybko określić, które chunki w ogóle warto rozpatrywać przy wyświetlaniu

* podział świata na chunki 16\*16\*16, pozwala na jeszcze większą optymalizację 

* wysokość chunk 256/384, rozmiar 16*16, to oznacza że na jeden chunk mamy 65536/98304 bloków 

* mówiąc o chunkach ogólna idea [octTree](https://en.wikipedia.org/wiki/Octree), [quadTree](https://en.wikipedia.org/wiki/Quadtree) i [BSP](https://en.wikipedia.org/wiki/Binary_space_partitioning) [K-D tree](https://en.wikipedia.org/wiki/K-d_tree)
najważniejsze są obrazki i animacja przy quadTree, wykorzystać tą animacje żeby przejść do tematu wyświetlania mniejszej ilości szczegółów dalej od gracza

* dzielenie świata na fragmenty
miasto -> osiedle -> ulica -> dom -> pokój
wyświetlamy TYLKO to co możemy zobaczyć

* im dalej obiekt od gracza tym mniejsza precyzja przy jego wyświetlaniu (np zamiast wielu kolorów bloku dirt, wystarczy duży brązowy kwadrat analogicznie z animacją z quadTree)
zmienny LOD (level of detail)
przy mc można zrobić pikseloze jak w quadTree, ale normalnie używa się [mipmaps](https://en.wikipedia.org/wiki/Mipmap)
[picture](https://vulkan-tutorial.com/images/mipmaps_example.jpg)

* język ma wpływ na wydajność (zwłaszcza zarządzanie pamięcia, garbage colector jest wolny!): mc w Java, mc bedrock w C++ [link](https://www.reddit.com/r/Minecraft/comments/kfe8y8/java_performance_vs_bedrock_performance_in_a/)


* wielowątkowość i lokalność cache (link linia 13)

===============================================================================================================

https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview

z buffer należy do ostatniego kroku pipeline



jedną z optymalizacji jest wyliczenie całego chunku a następnie zapamiętanie w cache tego co należy wyświetlić
i ten zapisany stan pozostanie niezmieniony dopóki my czegoś nie zrobimy (zniszczymy/zbudujemy blok)
w ten sposób zmniejszamy ilość obliczeń - podejście leniwe (wyliczamy tylko kiedy potrzeba)


https://en.wikipedia.org/wiki/Polygonal_modeling
https://en.wikipedia.org/wiki/Z-buffering               1974
https://en.wikipedia.org/wiki/Ray_casting               1978–1980
https://en.wikipedia.org/wiki/Ray_tracing_(graphics)    ~1982


https://en.wikipedia.org/wiki/Graphics_pipeline

In computer graphics, a computer graphics pipeline, rendering pipeline or simply graphics pipeline, is a conceptual model that describes what steps a graphics system needs to perform to render a 3D scene to a 2D screen.[1] Once a 3D model has been created, for instance in a video game or any other 3D computer animation, the graphics pipeline is the process of turning that 3D model into what the computer displays.[2]   Because the steps required for this operation depend on the software and hardware used and the desired display characteristics, there is no universal graphics pipeline suitable for all cases. However, graphics application programming interfaces (APIs) such as Direct3D and OpenGL were created to unify similar steps and to control the graphics pipeline of a given hardware accelerator. These APIs abstract the underlying hardware and keep the programmer away from writing code to manipulate the graphics hardware accelerators (AMD/Intel/NVIDIA etc.).

The model of the graphics pipeline is usually used in real-time rendering. Often, most of the pipeline steps are implemented in hardware, which allows for special optimizations. The term "pipeline" is used in a similar sense to the pipeline in processors: the individual steps of the pipeline run in parallel as long as any given step has what it needs.

The 3D pipeline usually refers to the most common form of computer 3D rendering called 3D polygon rendering[citation needed], distinct from raytracing and raycasting. In raycasting, a ray originates at the point where the camera resides, and if that ray hits a surface, the color and lighting of the point on the surface where the ray hit is calculated. In 3D polygon rendering the reverse happens - the area that is in view of the camera is calculated, and then rays are created from every part of every surface in view of the camera and traced back to the camera.[3]


https://www.quora.com/What-is-the-difference-between-ray-tracing-and-ray-casting

Ray casting, ray tracing and ray marching all tend to be used interchangeably as a generalists catch-all term for a rendering process which relies on computed lines of intersection, but there are subtle distinctions implied when speaking precisely.

Ray casting is a process where points of intersection with a line are computed analytically, using formulas of intersection.

Ray marching is a specific algorithm, a variant on ray casting where samples are taken down a line to test for intersections or other criteria. This is easier to implement and allows for speed optimizations via number of samples, but is not as precise even when large numbers of samples are used.

Ray tracing is a more complex series of tasks which uses ray casting and/or ray marching to compute not only the point of intersection between origin and object surface (or voxel cell etc) but which iteratively computes secondary and tertiary rays, which can be used to collect data used typically (but not exclusively) for calculation of reflected or refracted light.
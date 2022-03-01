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

===============================================================================================================

1. krótka historia, 'taka prosta gra - genialne algorytmy'
2. ogólne informacje o grafice komputerowej
3. zarządzanie wyświetlaniem
4. zarządzanie pamięcią


===============================================================================================================


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

* Zbufor/ray tracing ???
depth buffer / z-buffer
In a 3d-rendering pipeline, when an object is projected on the screen, the depth (z-value) of a generated fragment in the projected screen image is compared to the value already stored in the buffer (depth test), and replaces it if the new value is closer. It works in tandem with the rasterizer, which computes the colored values. The fragment outputted by the rasterizer is saved if it is not overlapped by another fragment. 
The Z-buffer is a technology used in almost all contemporary computers, laptops, and mobile phones for performing 3D computer graphics. The primary use now is for video games, which require fast and accurate processing of 3d scenes. The Z-buffer is implemented in hardware within consumer graphics cards.


* pozycja -> punkt -> kopia punktu na sześcian -> nałożenie tekstur (duplikowanie punktu w shader)

* rotowanie bloków trawy, żeby nie powtarzał się ten sam układ [link1](https://user-images.githubusercontent.com/61069740/88026810-af6c3c00-cb36-11ea-8da0-d6e929318aa2.jpg), [link2](https://user-images.githubusercontent.com/61069740/87943012-ba26c280-ca9d-11ea-8f52-055c76372ad8.jpg)

* nie wyświetlanie ścian kiedy bloki się stykają (łączenie bloków w większe figury)
[link](https://www.minecraftforum.net/forums/minecraft-java-edition/suggestions/15266-rendering-engine-speed-up-with-theorized-algorithm)

* język ma wpływ na wydajność (zwłaszcza zarządzanie pamięcia, garbage colector jest wolny!): mc w Java, mc bedrock w C++ [link](https://www.reddit.com/r/Minecraft/comments/kfe8y8/java_performance_vs_bedrock_performance_in_a/)

* drzewiasta struktura do przetrzymywania zestawów bloków (chunków) pozwalajaca szybko określić, które chunki w ogóle warto rozpatrywać przy wyświetlaniu

* podział świata na chunki 16*16*16

* wysokość chunk 256/384, rozmiar 16*16, to oznacza że na jeden chunk mamy 65536/98304 bloków 

* mówiąc o chunkach ogólna idea [octTree](https://en.wikipedia.org/wiki/Octree), [quadTree](https://en.wikipedia.org/wiki/Quadtree) i [BSP](https://en.wikipedia.org/wiki/Binary_space_partitioning)
najważniejsze są obrazki i animacja przy quadTree, wykorzystać tą animacje żeby przejść do tematu wyświetlania mniejszej ilości szczegółów dalej od gracza

* dzielenie świata na fragmenty
miasto -> osiedle -> ulica -> dom -> pokój
wyświetlamy TYLKO to co możemy zobaczyć

im dalej obiekt od gracza tym mniejsza precyzja przy jego wyświetlaniu (np zamiast wielu kolorów bloku dirt, wystarczy duży brązowy kwadrat analogicznie z animacją z quadTree)


===============================================================================================================




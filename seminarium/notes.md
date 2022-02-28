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

* przekształcanie obrazu świata do monitora (mnożenie macierzy)

* hitbox i vector wzroku ^

* podział świata na chunki 16*16*16

* Zbufor/ray tracing ???

* pozycja -> punkt -> kopia punktu na sześcian -> nałożenie tekstur

* rotowanie bloków trawy, żeby nie powtarzał się ten sam układ

* nie wyświetlanie ścian kiedy bloki się stykają (łączenie bloków w większe figury)
[link](https://www.minecraftforum.net/forums/minecraft-java-edition/suggestions/15266-rendering-engine-speed-up-with-theorized-algorithm)

* drzewiasta struktura do przetrzymywania zestawów bloków (chunków) pozwalajaca szybko określić, które chunki w ogóle warto rozpatrywać przy wyświetlaniu

* język ma różnice na wydajność (zwłaszcza zarządzanie pamięcia, garbage colector jest wolny!): mc w Java, mc bedrock w C++ (link)[https://www.reddit.com/r/Minecraft/comments/kfe8y8/java_performance_vs_bedrock_performance_in_a/]



===============================================================================================================




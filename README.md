# CS50 final
### Video demo
https://youtu.be/-kNVX47qgfc
### Description
This is mainly a testing ground for Web Assembly. How can you use it? How can you use it with React? What if you want to import a function using Wab assembly. Can you use sdl and c++ for a frontend app? This was a test for how far I can push the browser with some inclusions of some fun simulations.
This uses sdl and for the converge animation it can handle nearly 50000 particles depending on size. Using emsdk and make for the compiling and sdl for window control. There was a lot of little things to overcome window sizing, imports and some random meta knowledge
that was documented nowhere and that was the main challenge of this project getting the actual screen to pop up like html canvas but c++ and sdl. Of course the animations themselfs had challenges but that was the main one.
### Files
The project starts from the Ball.jsx folder where we call, import and control Web Assembly. The main code starts in WebAsm which contains sdl, AniSrc, main.cpp and make. Make is all the build commands sdl contains the imported sdl code and main is the main render loop.
Main.cpp also contains a setArguments function we export to update screen size and its global components. Something to note is the role that emscripten plays in the main render loop its a little odd compared to using sdl normally more similar to 
request animation frame then sdl.
 In AniSrc there is Shapes which contains all the related code for drawing shapes which is just a pixel. Converge contains all the code for the converging pixels.
Which is exported to main. The same with SpaceS 'no relation to spaceX :)' This contains the logic for the orbiting thing I made. Both of these follow a general layout. Define a class which contains specific meta info such as x,y rgb, speed, etc. Initialize a certain
amount of these classes. Then use math to do something with them. Moving the x and y in a certain direction or the speed or rgb. And that's generally it there is some nuance like looping over every frame with the updated data but that's the general idea.
### Largest challenges 
The hardest part was again the integration of Web Asm. There was constant unhelpful errors, linking issues, trying to figure out how to call a function from Web Assembly, getting main to work. There was also no proper documentation or videos I could find for the specific issues
that I was encountering trying to link Web Assembly and React I was able to finally solve it by reading doc's and some hints in videos here and there. But the reward was worth it sdl and c++ offers way more control and general speed seeing as its compiled compared to html canvas. Proof being that 
when checking performance the only thing that really uses the cpu is the scripting. There was some small issues as well
like canvas sizing and the like but compared to simply getting it to work everything else was trivial.

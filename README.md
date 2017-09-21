# Tetrees
A piece of my work as a game developer and a computer scientist, but yet just another puzzle game involving blocks.

Inside the Documentation/html directory you will find the code documented using doxygen. It's a work in progress so it is
incomplete and there are several flaws...

Basically, there is a TetreesEngine main object which creates a TetreesUI object providing an interface (using GTK and cairo) 
for the user to interact with, while the TetreesEngine do the background work by controlling the gameplay.

So, if you're interested in analyzing the code, I suggest you start by reading the TetreesEngine class source code (which you
can find inside Source folder) while I continue documenting everything...

There is a 1.0 version ("Tetrees 1.0" executable) right off the bat in the main folder that you can execute and play. 

***DON'T MOVE THE EXECUTABLE ALONE TO ANOTHER DIRECTORY OR IT WON'T RUN THE GAME INTERFACE PROPERLY DUE TO SOME .CSS, .TTF, 
AND OTHER FILES PRESENT INSIDE "Misc" DIRECTORY***

For now, it'll only compile in Linux systems since IT'S THE BEST AND UNIQUE REAL O.S.!!!... Just kidding... I love to develop
under Linux O.S. because it's free, beautiful, and it also allows to put REAL computer science in action! I'll work to develop
a version for Windows (sooner) and MacOS (only when I get one...).

This project uses: 
  - cairo library for drawing on screen;
  - GTK and GDK for UI components (widgets).

Thank you for your interest in my project!

Best,

Igor.

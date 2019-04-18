# Tetrees
A small part of my work as a game developer and a computer scientist, but yet just another puzzle game involving blocks.

### **_Understanding the code:_**

You will find the Doxygen generated documentation within ["Documentation/html"](https://github.com/ibutta/Tetrees/tree/master/Documentation/html) directory.

Start by downloading the project and then opening the "Documentation/html/index.html" file in your browser - preferably, Google Chrome - (if you try opening the file directly from GitHub you'll end up with the raw version of the HTML file and it won't work). Once you've opened index.html, you will be presented with a web interface where you'll find all the documentation with definitions and explanations for all thw classes, objects, member functions, and variables present in the source code.

Simply put, a TetreesEngine main object is created which then creates a TetreesUI object providing a UI (using GTK and cairo) 
for the user to interact with. During the gameplay, the TetreesEngine object is responsible for controlling the game mechanics while the TetreesUI object controls animations, drawings, and user inputs.

So, if you're interested in analyzing the code, I suggest you start by reading the TetreesEngine class source code (which you
can find inside the [Source folder](https://github.com/ibutta/Tetrees/tree/master/Source)) alongside with its documentation.

There is a 1.0 version ("Tetrees 1.0" executable) right off the bat in the main folder that you can execute and play. 

***DON'T MOVE THE EXECUTABLE ALONE TO ANOTHER DIRECTORY OR IT WON'T RUN THE GAME INTERFACE PROPERLY DUE TO SOME .CSS, .TTF, 
AND OTHER FILES PRESENT INSIDE "Misc" DIRECTORY***

### **_Linux_**:
For now, it'll only compile in **Linux** systems since **_it's the best OS ever!!!_** Just kidding... I love to develop
using Linux because it's free, beautiful, and it also demands some computer science abilities. 

### **_Windows_**: 
I'll work to come up with a version for Windows as soon as possible.

### **_MacOS_**: 
There is a MacOS version almost finished! I'll upload it very soon!


**This project uses:**
  - **_cairo library_** for drawings;
  - **_GTK_** and **_GDK_** for widgets.

Thank you for your interest in my project!

Best,

Igor.

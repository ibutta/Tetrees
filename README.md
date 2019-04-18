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

***DO NOT MOVE THE BINARY FILE/EXECUTABLE ALONE TO ANOTHER DIRECTORY OR IT WON'T RUN THE GAME INTERFACE PROPERLY DUE TO SOME .CSS, .TTF, 
AND OTHER FILES LOCATED INSIDE "Misc" DIRECTORY***

### **_Linux_**:
For now, it'll only compile in **Linux** systems since **_it's the best OS ever!!!_** Just kidding... I love to develop
using Linux because it's free, beautiful, and it also demands some computer science abilities.

**->Instructions to build (Linux):** The Tetrees repository is a complete Eclipse (IDE) project itself. Thus, you only need to have Eclipse (4.7-Oxygen or later version) installed in your Linux (it can be a Virtual Machine) and then import an existing project by selecting the Tetrees folder.

**HOWEVER**, in order to successfully build the project, you **HAVE** to have the following libraries installed:

    pthread             cairo-gobject 
    fontconfig          gtk-3 
    gdk-3               cairo 
    pangocairo-1.0      gdk_pixbuf-2.0 
    pango-1.0           gio-2.0 
    atk-1.0             gobject-2.0
    glib-2.0

### **_Windows_**: 
I'll work to come up with a version for Windows as soon as possible. Meanwhile, you can: 
* use [Virtual Box](https://www.virtualbox.org/) for instance; 
* [create a VM](https://www.virtualbox.org/manual/UserManual.html#gui-createvm) with [Ubuntu](https://www.ubuntu.com/) or [Mint](https://linuxmint.com/) (or whatever [Linux distro](https://distrowatch.com/dwres.php?resource=major) that fits you best); 
* [git clone](https://git-scm.com/docs/git-clone) the [project's repository](https://github.com/ibutta/Tetrees); 
* execute the compiled binary ("Tetrees 1.0") found in the project's root directory by opening a terminal (Ctrl+Alt+t) changing to project's directory and then running the command `./Tetrees\ 1.0`. 

### **_MacOS_**: 
There is a MacOS version almost finished! I'll upload it very soon! You can also use a virtual machine following exactly the same steps I mentioned above, under the "Windows" section.

### **Credits**
**This project uses:**
  - [**_cairo library_**](https://www.cairographics.org/) for drawings;
  - [**_GTK_**](https://www.gtk.org/) for widgets.

Thank you for your interest in my work!

Please, do not hesitate to contact me at igor.buttarello@gmail.com.

Best,

Igor.

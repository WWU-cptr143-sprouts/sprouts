sprouts
=======
Every spring quarter the CPTR 143 programming class at Walla Walla University
codes Conway's Game of Sprouts for a final project. The students are supposed
to create a new version from previous code with the final goal of creating an
unbeatable AI. However, except for one three-year streak, most years people
recode the program basically from scratch due to differing design approaches,
not wanting to read other people's messy code, etc. The CPTR 143 class of 2013
chose to take the 2010v2 code, which looked like a single person's side project
to check for line crossings graphically, as the basis for the new version.
However, like other years, after spending much time in designing a foundation,
a simple and unified data structure for storing nodes and lines, we chose to
recode most of the basic design. We have provided the A-Checker as a working,
well-tested foundation for future years and provided an experimental,
partially-working AI and GUI that respectively inherit from each other and
from Game, the A-Checker.

Using
-----
Because of people's differing preferred platforms, we have made this
cross-platform. 

### Windows ###
Throughout the year we used CodeBlocks to code in class. We chose to provide
project files for Windows in ``codeblocks/Sprouts_windows.cbp``. This will
link to the included SDL libraries. If you want to run the A-Checker tests,
build the project ``codeblocks/AChecker.cbp``.

**Note:** On Windows SDL will not output to the command prompt, so you'll
have to open ``stdout.txt`` to see the output.

### Mac ###
If you have MacPorts, you can run ``port install libsdl libsdl-framework
libsdl_gfx libsdl_image libsdl_ttf`` to install the necessary SDL libraries.
After this, you can either setup a CodeBlocks file or run ``make`` to compile
and ``./sprouts`` to run. To run the tests, ``make tests; ./tests/tests``.

### Linux ###
Install whatever provides the following. Dependencies:  

 * SDL
 * SDL\_gfx
 * SDL\_image
 * SDL\_ttf

Then, run ``make`` to compile and ``./sprouts`` to run. You can also run the
tests with ``make tests; ./tests/tests``.

Project Files
-------------
[Github Code](https://github.com/floft/sprouts)  
[Sprouts Wiki](http://sprouts.kingscastle.co/index.php/Main_Page)

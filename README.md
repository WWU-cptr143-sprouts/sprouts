sprouts
<<<<<<< HEAD
======

Okay, here's the deal. I created an automated script generate a doxygen comment for every function. Unfortunately, scripts aren't perfect. It still saves us hours of work, but it does mean that we'll have to have a list of files that need to be checked, and then go through each file and delete/fix any small errors. Some files have more things wrong than others. Most of the files should be "correct" out of the box.

The only important files in this branch are the .h files that were modified, along with this readme and a list "checklist.md". Please edit the checklist.md with your name/date next to whichever .h file you went through and checked and then push the changes to this repo when you're done. When all the files have been changed, we'll merge this with the master repo and work on actually filling in the boilerplate. I will be looking at the script in the meantime to see if I can modify it to "proof" it for more things. With that in mind, feel free to work "easiest to hardest" on the files and leave the really long and messy ones for me.
=======
=======

Okay, here's the deal. I created an automated script to find every instance of "{" and generate a doxygen comment for the function above that line. Unfortunately, that means the script added an extraneous doxygen comment for every for/while/if/etc loop as well. And it sometimes broke on weird and funky lambda syntax. It still saves us hours of work, but it does mean that we'll have to have a list of files that need to be checked, and then go through each file and delete/fix any small errors. Some files have more things wrong than others. Most of the files should be "correct" out of the box.

The only important files in thsi branch are the .h files that were modified, along with this readme and a list "checklist.md". Please edit the checklist.md with your name/date next to whichever .h file you went through and checked and then push the changes to this repo when you're done. When all the files have been changed, we'll merge this with the master repo and work on actually filling in the boilerplate. I will be looking at the script in the meantime to see if I can modify it to "proof" it for more things (e.g. get it to ignore if/for/while/etc loops). With that in mind, feel free to work "easiest to hardest" on the files and leave the really long and messy ones for me.
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

-Jared

Note to self:

<<<<<<< HEAD
Script currently breaks on:

1.     functions with no {
1.     if statements | FIXED
1.     switch statements | FIXED
1.     else statement | FIXED
1.     while loop | FIXED
1.     Anything starting with { that's not a function, really. | FIXED (should be)
1.     Breaks if "{" isn't one line below function name
1.     Breaks if entire function is commented out "//" style (don't do this! We have version control for a reason)
1.     Breaks if function is void w/no parameters
=======
Old script currently breaks on:

1. functions with no {
2. if statements
3. switch statements
4. else statement
5. while loop
6. Anything starting with { that's not a function, really.
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

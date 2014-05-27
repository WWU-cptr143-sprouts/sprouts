sprouts
=======

Okay, here's the deal. I created an automated script to find every instance of "{" and generate a doxygen comment for the function above that line. Unfortunately, that means the script added an extraneous doxygen comment for every for/while/if/etc loop as well. And it sometimes broke on weird and funky lambda syntax. It still saves us hours of work, but it does mean that we'll have to have a list of files that need to be checked, and then go through each file and delete/fix any small errors. Some files have more things wrong than others. Most of the files should be "correct" out of the box.

The only important files in thsi branch are the .h files that were modified, along with this readme and a list "checklist.md". Please edit the checklist.md with your name/date next to whichever .h file you went through and checked and then push the changes to this repo when you're done. When all the files have been changed, we'll merge this with the master repo and work on actually filling in the boilerplate. I will be looking at the script in the meantime to see if I can modify it to "proof" it for more things (e.g. get it to ignore if/for/while/etc loops). With that in mind, feel free to work "easiest to hardest" on the files and leave the really long and messy ones for me.

-Jared

Note to self:

Old script currently breaks on:

1. functions with no {
2. if statements
3. switch statements
4. else statement
5. while loop
6. Anything starting with { that's not a function, really.

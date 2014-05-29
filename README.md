sprouts
======
Okay, here's the deal. I created an automated script generate a doxygen comment for every function. Unfortunately, scripts aren't perfect. It still saves us hours of work, but it does mean that we'll have to have a list of files that need to be checked, and then go through each file and delete/fix any small errors. Some files have more things wrong than others. Most of the files should be "correct" out of the box.

The only important files in this branch are the .h files that were modified, along with this readme and a list "checklist.md". Please edit the checklist.md with your name/date next to whichever .h file you went through and checked and then push the changes to this repo when you're done. When all the files have been changed, we'll merge this with the master repo and work on actually filling in the boilerplate. I will be looking at the script in the meantime to see if I can modify it to "proof" it for more things. With that in mind, feel free to work "easiest to hardest" on the files and leave the really long and messy ones for me.

In the doxygen code, there should be a @retval for every returned variable (or none for no returned variable). Please go ahead and clone the @retval line if there's multiple returns. Change (variable) to the name of the variable and (description of variable) to a description.

All the doxygen code can be
In @brief. Please change the "Write  what..." to an actual description of the code

-Jared

Note to self:

Script currently breaks on:

1.     functions with no {
1.     Breaks if "{" isn't one line below function name
1.     Breaks if entire function is commented out "//" style (don't do this! We have version control for a reason)
1.     Breaks if function is void w/no parameters
1.     Cannot handle lambda functions, either adding proper spacing above function or subsequent :Dox running.

Todo:

1.  Add logic to find void w/no parameters
1.  Add logic to properly handle lambda functions | Script now will create lines above lambdas correctly without breaking other things.
1.  Edit plugin to parse for "returns"?
1.  Edit plugin to work with void + no parameters
1.  :Dox works just fine with lambda... So the reason the script was breaking wasn't "lambda" but rather improper handling of adding spaces above function

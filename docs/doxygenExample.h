/**
* \class ExampleClass
*
* \brief Provide an example
*
* This class is meant as an example.  It is not useful by itself
* rather its usefulness is only a function of how much it helps
* the reader.  It is in a sense defined by the person who reads it
* and otherwise does not exist in any real form.
*
* \note Attempts at zen rarely work.
*
*
*/

#ifndef EXAMPLECLASS_H
#define EXAMPLECLASS_H

class ExampleClass
{

public:

   /// Create an ExampleClass
   ExampleClass();

   /// Create an ExampleClass with lot's of intial values
   ExampleClass(int a, float b);

   /// This method does something
   void DoSomething();

   /** This is a method that does so
     * much that I must write an epic
     * novel just to describe how much
     * it truly does. */
   void DoNothing();

   /** \brief A useful method.
     * \param level does this and that and those things
     * \return Output that is extra useful
     *
     * This method does unbelievably useful things.
     * And returns exceptionally useful results.
     * Use it everyday with good health.
     */
   void* VeryUsefulMethod(bool level);

private:

   const char* fQuestion; ///< the question
   int fAnswer;           ///< the answer

};                              // end of class ExampleClass

#endif  // EXAMPLECLASS_H

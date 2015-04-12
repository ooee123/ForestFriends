package letter;

class Dimensions 
{
   /**
    * The maximum height each letter will occupy.
    * All letters will reach this height. Symbols may or may not reach.
    */
   static int HEIGHT = Letter.INCH;

   /**
    * The width that all alphabetic characters will have.
    */
   static int WIDTH = HEIGHT * 3 / 5;

   /**
    * The lower height bounds of where the center of the bit it allowed to go
    */
   static int MIN_HEIGHT = 0;

   /**
    * The upper height bounds of where the center of the bit it allowed to go
    */
   static int MAX_HEIGHT = HEIGHT;

   /**
    * The lower width bounds of where the center of the bit it allowed to go
    */
   static int MIN_WIDTH = 0;

   /**
    * The upper width bounds of where the center of the bit it allowed to go
    */
   static int MAX_WIDTH = WIDTH;

   static void setDimensions(int fontHeight, int strokeWidth)
   {
      HEIGHT = fontHeight * Letter.INCH;
      WIDTH = HEIGHT * 3 / 5;
      //MIN_HEIGHT = strokeWidth / 2;
      MAX_HEIGHT = HEIGHT - strokeWidth;
      //MIN_WIDTH = strokeWidth / 2;
      MAX_WIDTH = WIDTH - strokeWidth;
   }
}

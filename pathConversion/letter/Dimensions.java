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
   static int WIDTH_RATIO_N = 8;
   static int WIDTH_RATIO_D = 10;
   static int WIDTH = HEIGHT * WIDTH_RATIO_N / WIDTH_RATIO_D;

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

   static int strokeWeight = Letter.INCH / 8;

   static void setDimensions(int fontHeight, int _strokeWeight)
   {
      strokeWeight = _strokeWeight;
      HEIGHT = fontHeight * Letter.INCH;
      WIDTH = HEIGHT * WIDTH_RATIO_N / WIDTH_RATIO_D;
      //MIN_HEIGHT = strokeWeight / 2;
      MAX_HEIGHT = HEIGHT - strokeWeight;
      //MIN_WIDTH = strokeWeight / 2;
      MAX_WIDTH = WIDTH - strokeWeight;
   }

   static int getRelativeX(int num, int den)
   {
      return (Dimensions.MAX_WIDTH - Dimensions.MIN_WIDTH) * num / den + Dimensions.MIN_WIDTH;
   }

   static int getRelativeY(int num, int den)
   {
      return (Dimensions.MAX_HEIGHT - Dimensions.MIN_HEIGHT) * num / den + Dimensions.MIN_HEIGHT;
   }
}

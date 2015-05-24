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

   /**
    * Width of a 4/5 ratio letter
    */
   static int WIDTH_80 = (WIDTH * 4 / 5);
   /**
    * Width of a 4/5 ratio letter
    */
   static int WIDTH_90 = (WIDTH * 9 / 10);
   /**
    * Width of a 3/10 ratio letter
    */
   static int WIDTH_30 = (WIDTH * 3 / 10);
   /**
    * Width of a 1/5 ratio letter
    */
   static int WIDTH_20 = (WIDTH * 1 / 5);

   /**
    * Width of a 4/5 ratio letter
    */
   static int MAX_WIDTH_80 = WIDTH_80;
   /**
    * Width of a 4/5 ratio letter
    */
   static int MAX_WIDTH_90 = WIDTH_90;
   /**
    * Width of a 3/10 ratio letter
    */
   static int MAX_WIDTH_30 = WIDTH_30;
   /**
    * Width of a 1/5 ratio letter
    */
   static int MAX_WIDTH_20 = WIDTH_20;

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
      WIDTH_80 = (WIDTH * 4 / 5);
      WIDTH_90 = (WIDTH * 9 / 10);
      WIDTH_30 = (WIDTH * 3 / 10);
      WIDTH_20 = (WIDTH * 1 / 5);
      WIDTH_20 = strokeWeight * 1;
      MAX_WIDTH_30 = WIDTH_30 - strokeWeight;
      MAX_WIDTH_80 = WIDTH_80 - strokeWeight;
      MAX_WIDTH_90 = WIDTH_90 - strokeWeight;
      MAX_WIDTH_20 = strokeWeight;
   }

   static int getRelativeX(int num, int den)
   {
      return (int)Math.round((double)(Dimensions.MAX_WIDTH - Dimensions.MIN_WIDTH) * num / den + Dimensions.MIN_WIDTH);
   }

   static int getRelativeX(int maxWidth, int num, int den)
   {
      return (int)Math.round((double)(maxWidth - Dimensions.MIN_WIDTH) * num / den + Dimensions.MIN_WIDTH);
   }

   static int getRelativeY(int num, int den)
   {
      return (int)Math.round((double)(MAX_HEIGHT - Dimensions.MIN_HEIGHT) * num / den + Dimensions.MIN_HEIGHT);
   }
}

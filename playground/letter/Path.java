package letter;

/**
 * This is a vector that the marking tool will follow.
 * It will follow a motion from its current position to a new x and y on
 * the board. The motion can either be a marking motion, or a non-marking
 * motion, or could be a mark to begin the cut. The type of movement is
 * determined by the MovementType enum.
 */
public class Path {
   /**
    * Specifies the action of the marking tool.
    * Can either note the beginning of a serires of marks, or to activate
    * the marking tool to scribe along its motion, or to move without
    * scribing.
    */
   public enum MovementType {
      START,
      LINE,
      MOVE
   }

   /**
    * The absolute x component of the motion to take.
    */
   public int x;

   /**
    * The absolute y component of the motion to take.
    */
   public int y;

   /**
    * The scribing or non-scribing activity the marking tool.
    */
   public MovementType type;

   /**
    * Constructor for a path.
    *
    * @param x The absolute x coordinate.
    * @param y The absolute y coordinate.
    * @param type The movement type this path will take.
    */
   public Path(int x, int y, MovementType type)
   {
      this.x = x;
      this.y = y;
      this.type = type;
   }
}

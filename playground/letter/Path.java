package letter;

public class Path {
   public enum MovementType {
      START,
      LINE,
      MOVE
   }

   public int x;
   public int y;
   public MovementType type;

   public Path(int x, int y, MovementType type)
   {
      this.x = x;
      this.y = y;
      this.type = type;
   }
}

public class Path {
   public enum MovementType {
      START,
      LINE,
      MOVE
   }

   int x;
   int y;
   MovementType type;

   public Path(int x, int y, MovementType type)
   {
      this.x = x;
      this.y = y;
      this.type = type;
   }
}

package letter;

import java.util.*;

public class T implements Letter
{
   public static final int WIDTH =  Letter.MAX_HEIGHT * 6 / 10;

   public Paths getPaths()
   {
      Paths paths = new Paths(WIDTH);
      paths.add(new Path(0, Letter.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(WIDTH, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(WIDTH / 2, 0, Path.MovementType.MOVE));
      paths.add(new Path(WIDTH / 2, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}

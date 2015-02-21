package letter;

import java.util.*;

public class I implements Letter
{
   public static final int WIDTH = Letter.MAX_HEIGHT * 2 / 10;

   public Paths getPaths()
   {
      Paths paths = new Paths(WIDTH);
      paths.add(new Path(WIDTH / 2, 0, Path.MovementType.START));
      paths.add(new Path(WIDTH / 2, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}

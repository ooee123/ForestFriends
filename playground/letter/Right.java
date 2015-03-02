package letter;

import java.util.*;

public class Right implements Letter
{
   private static final int height = Letter.HEIGHT;
   private static final int width = height / 2 * 3;

   public Paths getPaths()
   {
      Paths paths = new Paths(width, height);
      paths.add(new Path(0, height * 9 / 16, Path.MovementType.START));
      paths.add(new Path(width * 1 / 2, height * 9 / 16, Path.MovementType.LINE));
      paths.add(new Path(width * 14 / 30, height, Path.MovementType.LINE));
      paths.add(new Path(width, height * 1 / 2, Path.MovementType.LINE));
      paths.add(new Path(width * 14 / 30, 0, Path.MovementType.LINE));
      paths.add(new Path(width * 1 / 2, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(0, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(0, height * 9 / 16, Path.MovementType.LINE));
      return paths;
   }
}

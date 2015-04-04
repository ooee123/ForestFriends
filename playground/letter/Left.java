package letter;

import java.util.*;

public class Left implements Letter
{
   private static final int height = Height.HEIGHT;
   private static final int width = height * 3 / 2;

   public Paths getPaths()
   {
      Paths paths = new Paths(width, height);
      paths.add(new Path(width, height * 9 / 16, Path.MovementType.START));
      paths.add(new Path(width * 1 / 2, height * 9 / 16, Path.MovementType.LINE));
      paths.add(new Path(width * 16 / 30, height, Path.MovementType.LINE));
      paths.add(new Path(0, height / 2, Path.MovementType.LINE));
      paths.add(new Path(width * 16 / 30, 0, Path.MovementType.LINE));
      paths.add(new Path(width * 1 / 2, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(width, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(width, height * 9 / 16, Path.MovementType.LINE));
      return paths;
   }
}

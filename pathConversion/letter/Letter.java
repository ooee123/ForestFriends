package letter;

import java.util.*;

public interface Letter
{
   /**
    * The number of encoder pulses per full revolution.
    */
   public static final int PULSES_PER_REVOLUTION = 32;

   /**
    * The number of interrupts per encoder Pulse
    */
   public static final int INTERRUPTS_PER_PULSE = 4;

   /**
    * The number of revolutions it takes to travel an inch.
    */
   public static final int REVOLUTION_PER_INCH = 8;

   /**
    * The number of discrete encoder units per physical inch.
    */
   public static final int INCH = PULSES_PER_REVOLUTION * REVOLUTION_PER_INCH * INTERRUPTS_PER_PULSE;

   /**
    * The resolution of points it takes to draw a complete circle.
    * Arbitrarily set as some double of 4.
    */
   public static final int DISCRETE_POINTS = 32;

   /**
    * A collection of Path that when followed will produce the Letter.
    * @return A collection of paths
    */
   public Paths getPaths();
} 

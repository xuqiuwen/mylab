package boolgame.Logic.PocketedStrategy;

import boolgame.Entity.Ball;

public class PocketedContext {
   private PocketedStrategy strategy;

   public PocketedContext(PocketedStrategy strategy) {
      this.strategy = strategy;
   }

   public void executeStrategy(Ball ball) {
      strategy.pocketed(ball);
   }
}

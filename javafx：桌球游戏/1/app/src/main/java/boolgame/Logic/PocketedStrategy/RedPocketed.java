package boolgame.Logic.PocketedStrategy;

import boolgame.Entity.Ball;

public class RedPocketed implements PocketedStrategy {
    public void pocketed(Ball ball) {
        ball.setIsPocketed(true);
        ball.disappear();
    }
}

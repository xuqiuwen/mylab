package boolgame.Logic.PocketedStrategy;

import boolgame.Entity.Ball;

public class WhitePocketed implements PocketedStrategy {
    @Override
    public void pocketed(Ball ball) {
        ball.reset();
    }

}

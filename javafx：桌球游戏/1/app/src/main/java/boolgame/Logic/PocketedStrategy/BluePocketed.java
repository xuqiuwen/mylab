package boolgame.Logic.PocketedStrategy;

import boolgame.Entity.Ball;
import javafx.scene.paint.Color;

public class BluePocketed implements PocketedStrategy {
    public void pocketed(Ball ball) {// 蓝球进袋子一次变成红球
        ball.setColor(Color.RED);
        ball.reset();
    }
}

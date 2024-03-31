package PoolGame;

import java.util.ArrayList;
import java.util.List;
import javafx.geometry.Point2D;
import PoolGame.Items.Ball;

public class GameMemento {
    private List<Point2D> ballPositions; // 用于存储球的位置信息

    public GameMemento(Game game) {
        this.ballPositions = new ArrayList<>();
        for (Ball ball : game.getBalls()) {
            // 存储每个球的位置信息
            ballPositions.add(new Point2D(ball.getXPos(), ball.getYPos()));
        }
    }

    // 获取球的位置信息列表
    public List<Point2D> getBallPositions() {
        return ballPositions;
    }
}

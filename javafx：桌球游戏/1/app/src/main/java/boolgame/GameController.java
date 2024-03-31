package boolgame;

import java.util.ArrayList;
import java.util.List;

import boolgame.Config.ConfigReader;
import boolgame.Config.ConfigParse.BallConfig;
import boolgame.Config.ConfigParse.PocketConfig;
import boolgame.Entity.Ball;
import boolgame.Entity.Pocket;
import boolgame.Entity.Table;
import boolgame.Logic.BallMoveLogic;
import javafx.scene.paint.Color;

public class GameController {
    Table table;
    private List<Pocket> pockets = new ArrayList<Pocket>();// 袋子
    private List<Ball> balls = new ArrayList<Ball>();// 红蓝球
    private Ball whiteBall;// 白球

    public void initData() {

        ConfigReader config = null;
        try {
            config = new ConfigReader("/config.json", true);
        } catch (Exception e) {
            System.err.println(e.getMessage());
            System.exit(1);
        }

        table = new Table(config.getConfig().getTableConfig().getSizeConfig().getX(),
                config.getConfig().getTableConfig().getSizeConfig().getY(),
                config.getConfig().getTableConfig().getFriction(),
                Color.web(config.getConfig().getTableConfig().getColor()));

        for (PocketConfig pocketConfig : config.getConfig().getPocketsConfig().getPocketConfigs()) {
            pockets.add(new Pocket(pocketConfig.getPositionConfig().getX(), pocketConfig.getPositionConfig().getY(),
                    pocketConfig.getRadius()));

        }

        for (BallConfig ballConfig : config.getConfig().getBallsConfig().getBallConfigs()) {
            balls.add(Ball.builder().setColor(Color.web(ballConfig.getColor()))
                    .setPosition(ballConfig.getPositionConfig().getX(), ballConfig.getPositionConfig().getY())
                    .setMass(ballConfig.getMass()).setRadius(ballConfig.getRadius())
                    .setVelocity(ballConfig.getVelocityConfig().getX(), ballConfig.getVelocityConfig().getY())
                    .setPocketed(false).build());
        }

        whiteBall = balls.get(0);

        // 初始化游戏设置，如重置球的位置等
    }

    public void startGame() {
        whiteBall.reset();// 刷新UI
        // 添加其他球的 UI
        for (Ball ball : balls) {
            ball.reset();
        }
    }

    public void next() {// 球的运动逻辑
        // 更新球的位置
        for (Ball ball : balls) {
            BallMoveLogic.update(ball, balls, pockets, table);
        }
    }

    public List<Ball> getBalls() {
        return balls;
    }

    public Ball getWhiteBall() {
        return whiteBall;
    }

    public List<Pocket> getPockets() {
        return pockets;
    }

    public Table getTable() {
        return table;
    }
}

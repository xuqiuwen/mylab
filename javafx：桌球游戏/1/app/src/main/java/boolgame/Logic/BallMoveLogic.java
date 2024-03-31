package boolgame.Logic;

import java.util.List;

import boolgame.Entity.Ball;
import boolgame.Entity.Pocket;
import boolgame.Entity.Table;
import boolgame.Logic.PocketedStrategy.BluePocketed;
import boolgame.Logic.PocketedStrategy.RedPocketed;
import boolgame.Logic.PocketedStrategy.WhitePocketed;
import boolgame.unit.Vector;
import javafx.scene.paint.Color;

public class BallMoveLogic {

    public static void update(Ball ball, List<Ball> balls, List<Pocket> pockets, Table table) {
        // if (ball.getColor().equals(Color.WHITE))
        // System.out.println(ball.getY());

        collideWithEdge(ball, table.getWidth(), table.getHeight());

        for (Ball otherBall : balls) {
            if (ball != otherBall) {
                collideWithAnotherBall(ball, otherBall);
            }
        }

        checkForPocketed(ball, pockets);

        applyFriction(ball, table.getFriction());

        move(ball);

    }

    public static void applyFriction(Ball ball, double fic) {
        if (Math.abs(ball.getVelocity().getX()) * Math.abs(ball.getVelocity().getY()) < 0.2) {
            ball.getVelocity().setX(0);
            ball.getVelocity().setY(0);
        }

        ball.getVelocity().setX(ball.getVelocity().getX() * (1 - fic));
        ball.getVelocity().setY(ball.getVelocity().getY() * (1 - fic));
    }

    public static void move(Ball ball) {

        // 更新位置基于速度
        ball.setX(ball.getX() + ball.getVelocity().getX());
        ball.setY(ball.getY() + ball.getVelocity().getY());
    }

    public static void collideWithEdge(Ball ball, double width, double height) {
        double nextX = ball.getX() + ball.getVelocity().getX();
        double nextY = ball.getY() + ball.getVelocity().getY();

        if (nextX - ball.getRadius() <= 0 || nextX + ball.getRadius() >= width) {
            ball.getVelocity().setX(-ball.getVelocity().getX());
        }
        if (nextY - ball.getRadius() <= 0 || nextY + ball.getRadius() >= height) {
            ball.getVelocity().setY(-ball.getVelocity().getY());
        }

    }

    public static void collideWithAnotherBall(Ball ball, Ball otherBall) {
        double dx = ball.getX() - otherBall.getX();
        double dy = ball.getY() - otherBall.getY();
        double distance = Math.sqrt(dx * dx + dy * dy);

        if (distance < ball.getRadius() + otherBall.getRadius()) {
            adjustVelocityAfterCollision(ball, otherBall);// 计算碰撞后速度

            adjustPositionsAfterCollision(ball, otherBall);// 防止粘在一起
        }
    }

    public static void adjustVelocityAfterCollision(Ball ball1, Ball ball2) {

        double dx = ball1.getX() - ball2.getX();
        double dy = ball1.getY() - ball2.getY();
        double distance = Math.sqrt(dx * dx + dy * dy);
        // 计算相对速度
        Vector relativeVelocity = new Vector(ball1.getVelocity().getX() - ball2.getVelocity().getX(),
                ball1.getVelocity().getY() - ball2.getVelocity().getY());

        // 计算法线方向
        Vector normal = new Vector(dx / distance, dy / distance);

        // 计算相对速度在法线方向上的投影
        double relativeSpeed = relativeVelocity.dotProduct(normal);

        // 计算质量比
        double massRatio = ball2.getMass() / ball1.getMass();

        // 计算碰撞后的相对速度
        double newRelativeSpeed = (relativeSpeed * (1 - massRatio) + 2 * massRatio * relativeSpeed)
                / (1 + massRatio);

        // 计算碰撞后的速度变化
        Vector velocityChange = new Vector(newRelativeSpeed * normal.getX(), newRelativeSpeed * normal.getY());

        // 更新球的速度
        ball1.getVelocity().setX(ball1.getVelocity().getX() - velocityChange.getX());
        ball1.getVelocity().setY(ball1.getVelocity().getY() - velocityChange.getY());

        ball2.getVelocity().setX(ball2.getVelocity().getX() + velocityChange.getX());
        ball2.getVelocity().setY(ball2.getVelocity().getY() + velocityChange.getY());
    }

    public static void adjustPositionsAfterCollision(Ball ball1, Ball ball2) {
        double dx = ball1.getX() - ball2.getX();
        double dy = ball1.getY() - ball2.getY();
        double distance = Math.sqrt(dx * dx + dy * dy);

        // 需要移动的距离，使得两球刚好接触，不重叠
        double overlap = (ball1.getRadius() + ball2.getRadius() - distance) / 2 + 0.0001;

        // 计算单位方向向量
        double unitX = dx / distance;
        double unitY = dy / distance;

        // 调整球的位置
        ball1.setX(ball1.getX() + unitX * overlap);
        ball1.setY(ball1.getY() + unitY * overlap);

        ball2.setX(ball2.getX() - unitX * overlap);
        ball2.setY(ball2.getY() - unitY * overlap);
    }

    public static void checkForPocketed(Ball ball, List<Pocket> pockets) {
        for (Pocket pocket : pockets) {
            double dx = ball.getX() - pocket.getX();
            double dy = ball.getY() - pocket.getY();
            double distance = Math.sqrt(dx * dx + dy * dy);

            if (distance < pocket.getRadius()) {
                // 球进入球袋
                if (ball.getColor().equals(Color.WHITE)) {
                    new WhitePocketed().pocketed(ball);
                } else if (ball.getColor().equals(Color.BLUE)) {
                    new BluePocketed().pocketed(ball);
                } else {
                    new RedPocketed().pocketed(ball);
                }
                break; // 如果球进入任一球袋，不需要检查其他球袋
            }
        }
    }

}

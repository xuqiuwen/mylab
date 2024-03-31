package boolgame.UI;

import boolgame.Entity.Ball;
import boolgame.unit.Vector;
import javafx.geometry.Point2D;
import javafx.scene.shape.Line;

public class WhiteBallUI extends BallUI {
    Line dragLine = new Line();// 拖拽线
    Line targetLine = new Line();// 瞄准线

    public WhiteBallUI(Ball ball) {
        super(ball);

        dragLine.setVisible(false);
        dragLine.setStrokeWidth(4);
        targetLine.setVisible(false);

        setOnMousePressed(event -> {
            if (ball.getVelocity().getX() == 0
                    && ball.getVelocity().getY() == 0) {
                Point2D pressPoint = new Point2D(event.getSceneX(), event.getSceneY());
                setUserData(pressPoint);
                // 准备拖拽线
                dragLine.setVisible(true);
                dragLine.setStartX(getCenterX());
                dragLine.setStartY(getCenterY());
                dragLine.setEndX(getCenterX());
                dragLine.setEndY(getCenterY());
                // 瞄准线
                targetLine.setVisible(true);
                targetLine.setStartX(getCenterX());
                targetLine.setStartY(getCenterY());
                targetLine.setEndX(getCenterX());
                targetLine.setEndY(getCenterY());
            }

        });

        setOnMouseDragged(event -> {
            if (ball.getVelocity().getX() == 0
                    && ball.getVelocity().getY() == 0) {
                // 更新拖拽线
                dragLine.setEndX(event.getSceneX());
                dragLine.setEndY(event.getSceneY());
                targetLine.setEndX(2 * getCenterX() - event.getSceneX());
                targetLine.setEndY(2 * getCenterY() - event.getSceneY());
            }
        });
        setOnMouseReleased(event -> {
            if (ball.getVelocity().getX() == 0
                    && ball.getVelocity().getY() == 0) {
                Point2D pressPoint = (Point2D) getUserData();
                Point2D releasePoint = new Point2D(event.getSceneX(), event.getSceneY());

                // 隐藏拖拽线
                dragLine.setVisible(false);
                targetLine.setVisible(false);

                // 计算速度
                double dx = releasePoint.getX() - pressPoint.getX();
                double dy = releasePoint.getY() - pressPoint.getY();

                // 设置小球的速度
                ball.setVelocity(new Vector(-dx / 10, -dy / 10));
            }
        });
    }

    public Line getDragLine() {
        return dragLine;
    }

    public Line getTargetLine() {
        return targetLine;
    }
}

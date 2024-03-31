package boolgame.UI;//球的UI模型

import boolgame.Entity.Ball;
import javafx.scene.shape.Circle;

public class BallUI extends Circle {// BallUI用来观察ball

    public BallUI(Ball ball) {
        super(ball.getRadius());
        // 设置初始位置
        setCenterX(ball.getX());
        setCenterY(ball.getX());
        setFill(ball.getColor());

        // 该为手动绘制UI，不监听了
        // // 监听这两个属性
        // ball.xProperty().addListener((obs, oldVal, newVal) -> {
        // setCenterX(newVal.doubleValue());

        // });
        // ball.yProperty().addListener((obs, oldVal, newVal) -> {
        // setCenterY(newVal.doubleValue());
        // });
    }

}

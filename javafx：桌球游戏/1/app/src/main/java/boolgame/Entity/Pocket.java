package boolgame.Entity;

public class Pocket {
    double x, y;
    private double radius;

    // 构造函数
    public Pocket(double x, double y) {
        this.x = x;
        this.y = y;
        this.radius = 80;
    }

    public Pocket(double x, double y, double radius) {
        this.x = x;
        this.y = y;
        this.radius = radius;
    }

    // 检测球是否进袋
    public boolean detectBall(Ball ball) {
        // 确定球的位置是否在球袋范围内
        // double distance = position.distance(ball.getPosition());
        // return distance <= radius;
        return false;
    }

    // Getter 和 Setter 方法
    public double getX() {
        return x;
    }

    public void setX(double x) {
        this.x = x;
    }

    public double getY() {
        return y;
    }

    public void setY(double y) {
        this.y = y;
    }

    public double getRadius() {
        return radius;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }
}

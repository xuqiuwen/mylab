package boolgame.Entity;//球的数据模型,建造者模式

import boolgame.unit.Vector;
import javafx.scene.paint.Color;

public class Ball {
    private Color color;// 颜色
    private Vector velocity;// 速度
    private double mass;// 质量
    private double radius;// 半径
    private boolean isPocketed;// 是否进袋子
    private Vector originPos;// 初始位置
    private Vector position;// 位置

    public Ball(Color color, Vector velocity, double mass, double radius, boolean isPocketed, double x, double y) {
        this.color = color;
        this.velocity = velocity;
        this.mass = mass;
        this.radius = radius;
        this.isPocketed = false;
        this.originPos = new Vector(x, y);
        this.position = new Vector(-1000, -1000);
    }

    // Getter 和 Setter 方法
    public Color getColor() {
        return color;
    }

    public void setColor(Color color) {
        this.color = color;
    }

    public Vector getVelocity() {
        return velocity;
    }

    public void setVelocity(Vector velocity) {
        this.velocity = velocity;
    }

    public void setVelocity(double vx, double vy) {
        setVelocity(new Vector(vx, vy));
    }

    public double getMass() {
        return mass;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }

    public double getRadius() {
        return radius;
    }

    public void setMass(double mass) {
        this.mass = mass;
    }

    public boolean getIsPocketed() {
        return isPocketed;
    }

    public void setIsPocketed(boolean isPocketed) {
        this.isPocketed = isPocketed;
    }

    public void setPos(double x, double y) {
        position.setX(x);
        position.setY(y);
    }

    public double getX() {
        return position.getX();
    }

    public void setX(double value) {
        position.setX(value);
    }

    public double getY() {
        return position.getY();
    }

    public void setY(double value) {
        position.setY(value);
    }

    public void reset() {// 重置位置
        setVelocity(0, 0);
        setX(originPos.getX());
        setY(originPos.getY());
    }

    public void disappear() {// 消失
        setVelocity(0, 0);
        setX(-1000);
        setY(-1000);
    }

    public static Ball.Builder builder() {
        return new Ball.Builder();
    }

    public static class Builder {
        private Color color = Color.WHITE; // 默认颜色为白色
        private Vector velocity = new Vector(0, 0); // 默认速度为 0
        private double mass = 1.0; // 默认质量
        private double radius = 15.0; // 默认半径
        private boolean isPocketed = false; // 默认不在袋中
        private double x = 0.0; // 默认位置 x
        private double y = 0.0; // 默认位置 y

        public Builder setColor(Color color) {
            this.color = color;
            return this;
        }

        public Builder setVelocity(double vx, double vy) {
            this.velocity = new Vector(vx, vy);
            return this;
        }

        public Builder setMass(double mass) {
            this.mass = mass;
            return this;
        }

        public Builder setRadius(double radius) {
            this.radius = radius;
            return this;
        }

        public Builder setPocketed(boolean isPocketed) {
            this.isPocketed = isPocketed;
            return this;
        }

        public Builder setPosition(double x, double y) {
            this.x = x;
            this.y = y;
            return this;
        }

        public Ball build() {
            return new Ball(color, velocity, mass, radius, isPocketed, x, y);
        }
    }

}
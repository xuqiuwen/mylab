package boolgame.Entity;

import javafx.scene.paint.Color;

public class Table {
    private double width;// 宽度
    private double height;// 高度
    private Color color;// 颜色
    private double friction;// 摩擦力

    // 构造函数
    public Table(double width, double height) {
        this.width = width;
        this.height = height;
        this.color = Color.GREEN;
        this.friction = 0.99;
    }

    public Table(double width, double height, double friction) {
        this.width = width;
        this.height = height;
        this.color = Color.GREEN;
        this.friction = friction;
    }

    public Table(double width, double height, double friction, Color color) {
        this.width = width;
        this.height = height;
        this.color = color;
        this.friction = friction;
    }

    // Getter 和 Setter 方法
    public double getWidth() {
        return width;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double height) {
        this.height = height;
    }

    public Color getColor() {
        return color;
    }

    public void setColor(Color color) {
        this.color = color;
    }

    public double getFriction() {
        return friction;
    }

    public void setFriction(double friction) {
        this.friction = friction;
    }
}

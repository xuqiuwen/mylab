package boolgame.unit;

public class Vector {
    private double x;
    private double y;

    // 构造函数
    public Vector(double x, double y) {
        this.x = x;
        this.y = y;
    }

    // 向量加法
    public void add(Vector other) {
        this.x += other.x;
        this.y += other.y;
    }

    // 向量乘法（标量）
    public void multiply(double scalar) {
        this.x *= scalar;
        this.y *= scalar;
    }

    // 获取向量的大小
    public double magnitude() {
        return Math.sqrt(x * x + y * y);
    }

    // 向量单位化
    public void normalize() {
        double magnitude = magnitude();
        if (magnitude != 0) {
            this.x /= magnitude;
            this.y /= magnitude;
        }
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

    public double dotProduct(Vector otherVector) {
        return this.x * otherVector.getX() + this.y * otherVector.getY();
    }
}

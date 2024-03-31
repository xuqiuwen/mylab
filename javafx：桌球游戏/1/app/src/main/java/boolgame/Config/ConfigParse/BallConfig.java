package boolgame.Config.ConfigParse;

import org.json.simple.JSONObject;

public class BallConfig implements Configurable {
    private String color;
    private VectorConfig position;
    private VectorConfig velocity;
    private double mass;
    private double radius;

    public BallConfig(Object obj) {
        this.parseJSON(obj);
    }

    private void init(String color, VectorConfig posConf, VectorConfig velConf, double mass, double radius) {

        this.color = color;
        this.position = posConf;
        this.velocity = velConf;
        this.mass = mass;
        this.radius = radius;
    }

    @Override
    public Configurable parseJSON(Object obj) {

        JSONObject json = (JSONObject) obj;
        String color = (String) json.get("color");
        VectorConfig posConf = new VectorConfig(json.get("position"));
        VectorConfig velConf = new VectorConfig(json.get("velocity"));
        double mass = (double) json.get("mass");
        double radius = (double) json.get("radius");
        init(color, posConf, velConf, mass, radius);
        return null;
    }

    public String getColor() {
        return this.color;
    }

    public VectorConfig getPositionConfig() {
        return this.position;
    }

    public VectorConfig getVelocityConfig() {
        return this.velocity;
    }

    public double getMass() {
        return this.mass;
    }

    public double getRadius() {
        return this.radius;
    }
}

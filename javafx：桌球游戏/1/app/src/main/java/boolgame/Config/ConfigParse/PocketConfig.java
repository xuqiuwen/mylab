package boolgame.Config.ConfigParse;

import org.json.simple.JSONObject;

public class PocketConfig implements Configurable {

    private VectorConfig position;

    private double radius;

    public PocketConfig(Object obj) {
        this.parseJSON(obj);
    }

    public PocketConfig(VectorConfig posConf, double radius) {
        this.init(posConf, radius);
    }

    private void init(VectorConfig posConf, double radius) {
        this.position = posConf;
        this.radius = radius;
    }

    public Configurable parseJSON(Object obj) {
        JSONObject json = (JSONObject) obj;
        VectorConfig posConf = new VectorConfig(json.get("position"));
        double radius = (double) json.get("radius");
        this.init(posConf, radius);
        return null;
    }

    public VectorConfig getPositionConfig() {
        return this.position;
    }

    public double getRadius() {
        return this.radius;
    }
}

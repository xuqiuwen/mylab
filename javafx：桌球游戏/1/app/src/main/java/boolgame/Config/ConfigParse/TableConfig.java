package boolgame.Config.ConfigParse;

import org.json.simple.JSONObject;

public class TableConfig implements Configurable {
    private String color;
    private double friction;
    private VectorConfig size;

    public TableConfig(Object obj) {
        this.parseJSON(obj);
    }

    public TableConfig(String colour, double friction, VectorConfig sizeConf) {
        this.init(color, friction, sizeConf);
    }

    private void init(String colour, double friction, VectorConfig sizeConf) {
        this.color = colour;
        this.friction = friction;
        this.size = sizeConf;
    }

    public Configurable parseJSON(Object obj) {
        JSONObject json = (JSONObject) obj;
        String color = (String) json.get("color");
        double friction = (double) json.get("friction");
        VectorConfig szConf = new VectorConfig(json.get("size"));
        this.init(color, friction, szConf);
        return this;
    }

    public String getColor() {
        return this.color;
    }

    public double getFriction() {
        return this.friction;
    }

    public VectorConfig getSizeConfig() {
        return this.size;
    }
}

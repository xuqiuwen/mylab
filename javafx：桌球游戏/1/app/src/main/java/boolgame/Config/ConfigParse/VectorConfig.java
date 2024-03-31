package boolgame.Config.ConfigParse;

import org.json.simple.JSONObject;

public class VectorConfig implements Configurable {
    private double x;
    private double y;

    private void init(double xPos, double yPos) {
        this.x = xPos;
        this.y = yPos;
    }

    public VectorConfig(Object obj) {
        this.parseJSON(obj);
    }

    public Configurable parseJSON(Object obj) {
        JSONObject json = (JSONObject) obj;
        this.init((double) json.get("x"), (double) json.get("y"));
        return this;
    }

    public double getX() {
        return this.x;
    }

    public double getY() {
        return this.y;
    }

}

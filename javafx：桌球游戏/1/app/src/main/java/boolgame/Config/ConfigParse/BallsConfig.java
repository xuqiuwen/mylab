package boolgame.Config.ConfigParse;

import java.util.ArrayList;
import java.util.List;

import org.json.simple.JSONArray;

public class BallsConfig implements Configurable {
    private List<BallConfig> ballList;

    public BallsConfig(Object obj) {
        this.parseJSON(obj);
    }

    private void init(List<BallConfig> ballList) {
        this.ballList = ballList;
    }

    public Configurable parseJSON(Object obj) {
        List<BallConfig> list = new ArrayList<>();
        JSONArray json = (JSONArray) obj;

        for (Object b : json) {
            list.add(new BallConfig(b));
        }
        this.init(list);
        return this;
    }

    public List<BallConfig> getBallConfigs() {
        return this.ballList;
    }
}

package boolgame.Config.ConfigParse;

import java.util.ArrayList;
import java.util.List;

import org.json.simple.JSONArray;

public class PocketsConfig implements Configurable {
    private List<PocketConfig> pocketList;

    public PocketsConfig(Object obj) {
        this.parseJSON(obj);
    }

    public PocketsConfig(List<PocketConfig> pocketList) {
        this.init(pocketList);
    }

    private void init(List<PocketConfig> pocketList) {
        this.pocketList = pocketList;
    }

    public Configurable parseJSON(Object obj) {
        List<PocketConfig> list = new ArrayList<>();
        JSONArray json = (JSONArray) obj;
        for (Object b : json) {
            list.add(new PocketConfig(b));
        }
        this.init(list);
        return this;
    }

    public List<PocketConfig> getPocketConfigs() {
        return this.pocketList;
    }
}

package boolgame.Config.ConfigFactory;

import boolgame.Config.ConfigParse.Configurable;
import boolgame.Config.ConfigParse.PocketsConfig;

public class PocketsConfigFactory implements ConfigFactory {
    public Configurable make(Object jSONObject) {
        return new PocketsConfig(jSONObject);
    }
}

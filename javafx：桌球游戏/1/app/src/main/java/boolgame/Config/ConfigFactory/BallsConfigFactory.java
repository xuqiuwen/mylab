package boolgame.Config.ConfigFactory;

import boolgame.Config.ConfigParse.BallsConfig;
import boolgame.Config.ConfigParse.Configurable;

public class BallsConfigFactory implements ConfigFactory {
    public Configurable make(Object jSONObject) {
        return new BallsConfig(jSONObject);
    }
}

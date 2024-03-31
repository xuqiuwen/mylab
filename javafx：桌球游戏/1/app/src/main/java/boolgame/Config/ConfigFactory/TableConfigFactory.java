package boolgame.Config.ConfigFactory;

import boolgame.Config.ConfigParse.Configurable;
import boolgame.Config.ConfigParse.TableConfig;

public class TableConfigFactory implements ConfigFactory {
    public Configurable make(Object jSONObject) {
        return new TableConfig(jSONObject);
    }
}

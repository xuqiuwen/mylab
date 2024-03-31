package boolgame.Config.ConfigFactory;

import boolgame.Config.ConfigParse.Configurable;

public interface ConfigFactory {
    public Configurable make(Object jSONObject);

}

package boolgame.Config.ConfigFactory;

import java.util.*;

import boolgame.Config.ConfigParse.Configurable;

public class ConfigFactoryRegistry {

    private Map<String, ConfigFactory> registry = new HashMap<>();

    public void registerDefault() {
        this.register("table", new TableConfigFactory());
        this.register("balls", new BallsConfigFactory());
        this.register("pockets", new PocketsConfigFactory());
    }

    public void register(String key, ConfigFactory factory) {
        if (!this.registry.containsKey(key)) {
            this.registry.put(key, factory);
        } else {
            this.registry.replace(key, factory);
        }
    }

    public ConfigFactory unregister(String key) {
        return this.registry.remove(key);
    }

    public ConfigFactory getFactory(String key) {
        return this.registry.get(key);
    }

    public Configurable create(String key, Object obj) {
        ConfigFactory factory = this.getFactory(key);
        if (factory != null) {

            return factory.make(obj);
        }
        return null;
    }
}

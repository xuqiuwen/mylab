package PoolGame.Factory;

import org.json.simple.JSONObject;

import PoolGame.Config.PocketsConfig;
import PoolGame.Config.Configurable;

/** A config factory that will handle the `pocket` section of the config */
public class PocketsConfigFactory implements ConfigFactory {
    public Configurable make(Object jSONObject) {
        Object obj = ((JSONObject) jSONObject).get("pocket");
        return new PocketsConfig(obj);
    }
}

package boolgame.Config;//读配置文件

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import boolgame.Config.ConfigFactory.ConfigFactoryRegistry;
import boolgame.Config.ConfigParse.BallsConfig;
import boolgame.Config.ConfigParse.GameConfig;
import boolgame.Config.ConfigParse.PocketsConfig;
import boolgame.Config.ConfigParse.TableConfig;

public class ConfigReader {

    private String path;
    private boolean isResourcesDir;
    private GameConfig config;

    public ConfigReader(String path, boolean isResourcesDir)
            throws FileNotFoundException, IOException, ParseException, IllegalArgumentException {
        this.path = path;
        this.isResourcesDir = isResourcesDir;
        this.config = null;
        this.parse();
    }

    private JSONObject readFile() throws FileNotFoundException, IOException,
            ParseException {
        JSONParser parser = new JSONParser();
        Object obj;
        if (this.isResourcesDir) {
            obj = parser.parse(new InputStreamReader(getClass().getResourceAsStream(path)));
        } else {
            obj = parser.parse(new FileReader(path));
        }
        return (JSONObject) obj;
    }

    public void parse() throws FileNotFoundException, IOException,
            ParseException,
            IllegalArgumentException {
        JSONObject jsonConfig = this.readFile();
        ConfigFactoryRegistry registry = new ConfigFactoryRegistry();
        registry.registerDefault();
        TableConfig table = (TableConfig) registry.create("table",
                jsonConfig.get("table"));
        BallsConfig pockets = (BallsConfig) registry.create("balls",
                jsonConfig.get("balls"));
        PocketsConfig balls = (PocketsConfig) registry.create("pockets",
                jsonConfig.get("pockets"));
        this.config = new GameConfig(table, pockets, balls);

    }

    public String getPath() {
        return this.path;
    }

    public GameConfig getConfig() {
        return this.config;
    }

    public void reload() throws FileNotFoundException, IOException,
            ParseException,
            IllegalArgumentException {
        this.parse();
    }
}

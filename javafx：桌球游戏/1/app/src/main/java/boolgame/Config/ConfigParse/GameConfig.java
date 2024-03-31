package boolgame.Config.ConfigParse;

import org.json.simple.JSONObject;

public class GameConfig implements Configurable {
    private TableConfig table;
    private BallsConfig balls;
    private PocketsConfig pockets;

    public GameConfig(TableConfig table, BallsConfig balls, PocketsConfig pockets) {
        this.init(table, balls, pockets);
    }

    private void init(TableConfig table, BallsConfig balls, PocketsConfig pockets) {
        this.table = table;
        this.balls = balls;
        this.pockets = pockets;
    }

    public Configurable parseJSON(Object obj) {
        JSONObject json = (JSONObject) obj;
        this.table = new TableConfig(json.get("table"));
        this.balls = new BallsConfig(json.get("balls"));
        this.pockets = new PocketsConfig(json.get("pockets"));

        this.init(table, balls, pockets);
        return this;
    }

    public TableConfig getTableConfig() {
        return this.table;
    }

    public BallsConfig getBallsConfig() {
        return this.balls;
    }

    public PocketsConfig getPocketsConfig() {
        return this.pockets;
    }

}
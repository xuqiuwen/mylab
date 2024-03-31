package PoolGame.Config;

import org.json.simple.JSONObject;

/** A config class for the game configuration */
public class GameConfig implements Configurable {
    private TableConfig table;
    private BallsConfig balls;
    private PocketsConfig pockets;

    /**
     * Initialise the game config with the provided value
     * 
     * @param table An instance of the table config
     * @param balls An instance of the balls config containing all the balls
     */
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
        this.table = new TableConfig(json.get("Table"));
        this.balls = new BallsConfig(json.get("Balls"));
        this.init(table, balls, pockets);
        return this;
    }

    /**
     * Get the table config instance as defined in the config
     * 
     * @return The table config instance
     */
    public TableConfig getTableConfig() {
        return this.table;
    }

    /**
     * Get the balls config instance as defined in the config
     * 
     * @return The balls config instance
     */
    public BallsConfig getBallsConfig() {
        return this.balls;
    }

    public PocketsConfig getPocketsConfig() {
        return this.pockets;
    }
}

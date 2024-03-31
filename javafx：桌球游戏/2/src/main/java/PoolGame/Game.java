package PoolGame;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

import PoolGame.Builder.BallBuilderDirector;
import PoolGame.Config.BallConfig;
import PoolGame.Config.PocketConfig;
import PoolGame.Items.Ball;
import PoolGame.Items.Pocket;
import PoolGame.Items.PoolTable;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.geometry.Point2D;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.input.MouseEvent;
import javafx.scene.text.Text;

/** The game class that runs the game */
public class Game {
    private PoolTable table;
    List<Ball> balls;
    List<Pocket> pockets;
    private boolean shownWonText = false;
    private final Text winText = new Text(100, 100, "Win and Bye");

    Stack<GameMemento> history;

    private Label Time;
    private Label Score;
    private TimeScore timeScore;
    private Button Undo;

    /**
     * Initialise the game with the provided config
     * 
     * @param config The config parser to load the config from
     */
    public Game(ConfigReader config) {
        this.setup(config);
    }

    public void reload(ConfigReader config) {
        this.setup(config);
    }

    private void setup(ConfigReader config) {
        this.table = new PoolTable(config.getConfig().getTableConfig());
        List<BallConfig> ballsConf = config.getConfig().getBallsConfig().getBallConfigs();
        List<PocketConfig> pocketsConf = config.getConfig().getPocketsConfig().getPocketConfigs();
        history = new Stack<GameMemento>();
        balls = new ArrayList<>();
        pockets = new ArrayList<>();
        BallBuilderDirector builder = new BallBuilderDirector();
        timeScore = new TimeScore(balls);

        builder.registerDefault();
        for (BallConfig ballConf : ballsConf) {
            Ball ball = builder.construct(ballConf);
            if (ball == null) {
                System.err.println("WARNING: Unknown ball, skipping...");
            } else {
                balls.add(ball);
            }
        }
        this.table.setupBalls(balls);
        setReleasedCueBallWithSave();

        for (PocketConfig pocketConf : pocketsConf) {
            pockets.add(new Pocket(pocketConf.getPositionConfig().getX(),
                    pocketConf.getPositionConfig().getY(),
                    pocketConf.getRadius(),
                    pocketConf.getColour()));
        }
        this.table.setupPockets(pockets);

        this.winText.setVisible(false);
        this.winText.setX(table.getDimX() / 2);
        this.winText.setY(table.getDimY() / 2);
    }

    public boolean getshownWonText() {
        return this.shownWonText;
    }

    /**
     * Get the window dimension in the x-axis
     * 
     * @return The x-axis size of the window dimension
     */
    public double getWindowDimX() {
        return this.table.getDimX();
    }

    /**
     * Get the window dimension in the y-axis
     * 
     * @return The y-axis size of the window dimension
     */
    public double getWindowDimY() {
        return this.table.getDimY();
    }

    /**
     * Get the pool table associated with the game
     * 
     * @return The pool table instance of the game
     */
    public PoolTable getPoolTable() {
        return this.table;
    }

    public List<Ball> getBalls() {
        return this.balls;
    }

    /**
     * Add all drawable object to the JavaFX group
     * 
     * @param root The JavaFX `Group` instance
     */
    public void addDrawables(Group root) {
        ObservableList<Node> groupChildren = root.getChildren();
        table.addToGroup(groupChildren);
        groupChildren.add(this.winText);

        Undo = new Button("Undo");
        Undo.setLayoutX(720);
        Undo.setLayoutY(0);
        groupChildren.add(Undo);
        Undo.setOnMouseClicked(new EventHandler<MouseEvent>() { // Creation of the undo button
            @Override
            public void handle(MouseEvent event) {
                getThis().undo();
            }
        });

        this.Time = new Label("Score: " + String.valueOf(this.timeScore.getTime()));
        this.Time.setLayoutX(700);
        this.Time.setLayoutY(130);
        groupChildren.add(this.Time);

        this.Score = new Label("Score: " + String.valueOf(this.timeScore.getScore()));
        this.Score.setLayoutX(700);
        this.Score.setLayoutY(150);
        groupChildren.add(this.Score);

    }

    /** Reset the game */
    public void reset() {
        this.winText.setVisible(false);
        this.shownWonText = false;
        this.table.reset();
    }

    /** Code to execute every tick. */
    public void tick() {
        if (table.hasWon() && !this.shownWonText) {
            System.out.println(this.winText.getText());
            this.winText.setVisible(true);
            this.shownWonText = true;
        }
        table.checkPocket(this);
        table.handleCollision();
        this.table.applyFrictionToBalls();
        for (Ball ball : this.table.getBalls()) {
            ball.move();
        }

        this.Score.setText("Score: " + String.valueOf(this.timeScore.getAndUpdateScore()));
        this.Time.setText(("Time: " + String.valueOf(Math.floorDiv((this.timeScore.getAndUpdatedTime()), 60))));
    }

    public void setReleasedCueBallWithSave() {// 注册击球保存，白球的扩展
        for (Ball ball : table.getBalls()) {
            if (ball.getBallType() == Ball.BallType.CUEBALL)
                ball.setReleasedWithSave(this);
        }
    }

    public Game getThis() {
        return this;
    }

    public void saveCurrent() {
        history.push(createMemento());
    }

    public void undo() {
        if (!history.empty()) {
            restoreFromMemento(history.pop());
        }
    }

    public GameMemento createMemento() {
        // 创建并返回当前状态的快照
        return new GameMemento(this);
    }

    public void restoreFromMemento(GameMemento memento) {
        List<Point2D> savedPositions = memento.getBallPositions();
        List<Ball> balls = this.balls;// 获取当前的球列表

        for (int i = 0; i < balls.size(); i++) {
            Ball ball = balls.get(i);
            Point2D position = savedPositions.get(i);
            ball.setXPos(position.getX());
            ball.setYPos(position.getY());
            ball.setXVel(0);
            ball.setYVel(0);
        }

    }

}

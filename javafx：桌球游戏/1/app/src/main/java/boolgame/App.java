package boolgame;

import boolgame.UI.GameUI;
import javafx.animation.AnimationTimer;
import javafx.application.Application;

import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class App extends Application {

    @Override
    public void start(Stage primaryStage) {

        // 创建游戏逻辑控制
        GameController gameController = new GameController();
        gameController.initData();

        // 创建根组件
        Group root = new Group();

        // 初始化UI
        GameUI.initUI(root, gameController);

        // 游戏开始
        gameController.startGame();

        // 播放游戏帧
        new AnimationTimer() {
            @Override
            public void handle(long now) {
                // 更新球的位置
                gameController.next();
                GameUI.next(gameController);
            }
        }.start();

        // 设置场景和舞台
        Scene scene = new Scene(root, gameController.getTable().getWidth() + 100,
                gameController.getTable().getHeight());
        primaryStage.setTitle("BallGame");
        primaryStage.setScene(scene);
        primaryStage.show();

    }

    public static void main(String[] args) {
        launch(args);
    }
}

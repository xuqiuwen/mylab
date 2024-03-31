package boolgame.UI;

import java.util.ArrayList;
import java.util.List;

import boolgame.GameController;
import boolgame.Entity.Pocket;
import javafx.scene.Group;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.scene.shape.Shape;

public class GameUI {
    private static WhiteBallUI whiteBallUI;
    private static List<BallUI> ballUIs = new ArrayList<BallUI>();
    private static List<Shape> pocketUIs = new ArrayList<Shape>();
    private static VBox buttonBox;
    private static Label label1;
    private static Label label2;
    private static Group tableUI;

    public static void initUI(Group root, GameController gameController) {
        // 创建一个 VBox 用于放置标签
        buttonBox = new VBox(10); // 10 是按钮之间的间距
        buttonBox.setLayoutX(gameController.getTable().getWidth() + 10);
        label1 = new Label();
        label2 = new Label("You Win!");
        label2.setVisible(false);
        buttonBox.getChildren().addAll(label1, label2);

        // 创建代表台球桌的 tableUI
        Rectangle Rec = new Rectangle(gameController.getTable().getWidth(), gameController.getTable().getHeight(),
                gameController.getTable().getColor());
        tableUI = new Group(Rec);

        // 创建代表袋子的 UI
        for (Pocket pocket : gameController.getPockets()) {
            Shape pocketShape = Shape.intersect(
                    new Circle(pocket.getX(), pocket.getY(), pocket.getRadius(), Color.BLACK),
                    Rec);
            pocketUIs.add(pocketShape);
            tableUI.getChildren().add(pocketShape);
        }

        // 添加白球的 UI
        whiteBallUI = new WhiteBallUI(gameController.getWhiteBall());
        tableUI.getChildren().add(whiteBallUI);
        tableUI.getChildren().add(whiteBallUI.getDragLine());
        tableUI.getChildren().add(whiteBallUI.getTargetLine());
        ballUIs.add(whiteBallUI);

        // 添加其他球的 UI
        for (int i = 1; i < gameController.getBalls().size(); i++) {
            BallUI ballui = new BallUI(gameController.getBalls().get(i));
            ballUIs.add(ballui);
            tableUI.getChildren().add(ballui);
        }

        // 将台球桌和按钮添加到 BorderPane 的相应区域
        root.getChildren().add(tableUI);
        root.getChildren().add(buttonBox);
    }

    public static void next(GameController gameController) {
        int score = 0;
        for (int i = 0; i < gameController.getBalls().size(); i++) {
            ballUIs.get(i).setCenterX(gameController.getBalls().get(i).getX());
            ballUIs.get(i).setCenterY(gameController.getBalls().get(i).getY());
            if (gameController.getBalls().get(i).getIsPocketed()) {
                score++;
            }
        }
        if (score == gameController.getBalls().size() - 1) {
            label2.setVisible(true);
        }
        label1.setText("Score: " + String.valueOf(score));

    }

}

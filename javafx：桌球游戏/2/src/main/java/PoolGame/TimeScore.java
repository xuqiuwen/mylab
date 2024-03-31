package PoolGame;

import java.util.List;
import PoolGame.Items.Ball;

/** A pool TimeScore to keep track of score and time */
public class TimeScore {
    private int Time;
    private int Score;
    List<Ball> balls;

    public TimeScore() {
        this.Score = 0;
        this.Time = 0;
    }

    /**
     * Build the TimeScore with the provided values
     * 
     * 
     */

    TimeScore(List<Ball> balls) {
        this.Score = 0;
        this.Time = 0;
        this.balls = balls;
    }

    /**
     * Update the time
     */

    public void UpdateTime() {
        this.Time = this.Time + 1;
    }

    public int getTime() {

        // System.out.println(this.Time);
        return this.Time;
    }

    public int getAndUpdatedTime() {

        // System.out.println(this.Time);
        return this.Time++;
    }

    /**
     * Calculates the score by adding up the scores of all the observers
     * 
     * @return the score of the TimeScore
     */
    public int getScore() {
        this.Score = 0;
        return this.Score;
    }

    public int getAndUpdateScore() {
        this.Score = 0;
        for (Ball ball : balls) {
            Score += ball.getScore();
        }
        return this.Score;
    }

}

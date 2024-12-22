public class Agent {
    public double velocity;
    public double position;    // domain: [0, 1] [top, bottom]
    public boolean isJumping = false;

    private double jumpStrength = 0.15; // How powerfully the bird jumps
    private double gravity = 0.05;      // Gravity effect
    private double maxVelocity = 0.05;  // max downward velocity

    public Agent() {
        velocity = 0;
        position = 0.5;
    }

    public void handleInput(char input) {
        if (input == '\n') {
            isJumping = true;
        }
    }

    // returns new position
    public double update() {
        if (isJumping) {
            velocity = -jumpStrength;
            isJumping = false;
        }

        velocity += gravity;    // Apply gravity
        if (velocity > maxVelocity)
            velocity = maxVelocity;
        position += velocity;

        return position;
    }
}


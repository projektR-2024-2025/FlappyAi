import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Simulator {
    public static final int groundLevel = 20;
    public static final int viewWidth = 35;
    public boolean[][] obstacleMap = new boolean[groundLevel][viewWidth];

    private List<Integer> mapEmpty = new ArrayList<>();
    private List<Integer> mapCeiling = new ArrayList<>();
    private List<Integer> mapGround = new ArrayList<>();

    private int mapReadIndex = 0;
    private boolean pillarFlag = false;

    private boolean running;
    private boolean scoreScreen;
    private int birdPosition;

    public Simulator() {
        this.running = true;
        this.scoreScreen = true;
        for (int i = 0; i < groundLevel-1; i++) {
            for (int j = 0; j < viewWidth; j++) {
                obstacleMap[i][j] = false;
            }
        }

        initializeMap();
    }

    public Simulator(String map) {
        this.running = true;
        this.scoreScreen = true;
        for (int i = 0; i < groundLevel; i++) {
            for (int j = 0; j < viewWidth; j++) {
                obstacleMap[i][j] = false;
            }
        }

        initializeMap(map);
    }

    public boolean isRunning() {
        return running;
    }

    public boolean isScoreScreenActive() {
        return scoreScreen;
    }

    // Attempt a non-blocking check for input
    public boolean handleEvents(Agent agent) {
        char ch = readKeyNonBlocking();
        if (ch != '\0') {
            if (ch == 'q') {
                running = false;
            } else {
                agent.handleInput(ch);
            }
            return true;
        }
        return false;
    }

    public void handleScoreEvents() {
        char ch = readKeyNonBlocking();
        if (ch != '\0') {
            if (ch == 'q') {
                System.out.println("The END!");
                scoreScreen = false;
            }
        }
    }

    public void update(Agent agent) {
        // Map movement.
        for (int i = 0; i < groundLevel; i++) {
            for (int j = 0; j < viewWidth - 1; j++) {
                obstacleMap[i][j] = obstacleMap[i][j + 1];
            }
        }

        loadNextColumn();

        double y = agent.update();
        birdPosition = (int) (y * groundLevel);

        // Ground and ceiling collision.
        if (birdPosition >= groundLevel || birdPosition <= 0) {
            running = false; // Game over if bird hits ground or ceiling
        }

        // Pillar collision.
        if(birdPosition != groundLevel) {
            if (obstacleMap[birdPosition][0]) {
                running = false;
            }
        }

    }

    public void render() {
        clearScreen();

        // Ceiling
        for (int j = 0; j < viewWidth; j++) System.out.print("=");
        System.out.println();
        for (int i = 0; i < groundLevel; i++) {
            System.out.print("  ");
            for (int j = 0; j < viewWidth; j++) {
                if (i == birdPosition && j == 0) System.out.print("O");
                else if (!obstacleMap[i][j]) System.out.print(" ");
                else System.out.print("#");
            }
            System.out.println();
        }
        // Ground
        for (int j = 0; j < viewWidth; j++) System.out.print("=");
        System.out.println();
    }

    public void initializeMap(String map) {
        // read lines from file
        try (BufferedReader br = new BufferedReader(new FileReader(map))) {
            String line;
            while ((line = br.readLine()) != null) {
                // Parse the line
                String[] tokens = line.split("\\s+"); // split by whitespace
                if (tokens.length < 3) {
                    continue;
                }
                int emptyVal    = Integer.parseInt(tokens[0]);
                int ceilingVal  = Integer.parseInt(tokens[1]);
                int groundVal   = Integer.parseInt(tokens[2]);

                mapEmpty.add(emptyVal);
                mapCeiling.add(ceilingVal);
                mapGround.add(groundVal);
            }
        } catch (IOException e) {
            System.err.println("Error opening map file: " + map);
            e.printStackTrace();
            return;
        }

        // Fill the initial view
        for (int j = 0; j < viewWidth; j++) {
            loadNextColumn(j);
        }
    }


    public void initializeMap() {
        // placeholder pillars
        // pillar 1
        mapEmpty.add(20);
        mapCeiling.add(3);
        mapGround.add(5);

        // pillar 2
        mapEmpty.add(30);
        mapCeiling.add(7);
        mapGround.add(1);

        // pillar 3
        mapEmpty.add(25);
        mapCeiling.add(8);
        mapGround.add(6);

        // Fill the initial view
        for (int j = 0; j < viewWidth; j++) {
            loadNextColumn(j);
        }
    }

    public void loadNextColumn() {
        loadNextColumn(viewWidth - 1);
    }

    public void loadNextColumn(int pos) {
        if (mapReadIndex >= mapEmpty.size()) {
            for (int i = 0; i < groundLevel; i++) {
                obstacleMap[i][pos] = false;
            }
        } else if (pillarFlag) {
            pillarFlag = false;
            for (int i = 0; i < groundLevel; i++) {
                obstacleMap[i][pos] = false;
                if (i <= mapCeiling.get(mapReadIndex) - 1 || i >= groundLevel - mapGround.get(mapReadIndex))
                obstacleMap[i][pos] = true;
            }
            mapReadIndex += 1;
        } else if (mapEmpty.get(mapReadIndex) == 0) {
            for (int i = 0; i < groundLevel; i++) {
                obstacleMap[i][pos] = false;
                if (i <= mapCeiling.get(mapReadIndex) - 1 || i >= groundLevel - mapGround.get(mapReadIndex))
                obstacleMap[i][pos] = true;
            }
            pillarFlag = true;
        } else {
            for (int i = 0; i < groundLevel; i++) {
                obstacleMap[i][pos] = false;
            }
            mapEmpty.set(mapReadIndex, mapEmpty.get(mapReadIndex) - 1);
        }
    }

    // Attempts to read a character from System.in without blocking.
    // If no input is available, returns '\0'.
    private char readKeyNonBlocking() {
        try {
            if (System.in.available() > 0) {
                int ch = System.in.read();
                if (ch != -1) return (char) ch;
            }
        } catch (IOException e) {
            // Ignore errors in reading
        }
        return '\0';
    }

    private void clearScreen() {
        // ANSI clear screen
        System.out.print("\u001b[H\u001b[2J");
        System.out.flush();
    }
}


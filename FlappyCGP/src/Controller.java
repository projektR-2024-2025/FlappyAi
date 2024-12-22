public class Controller {
    public boolean action(Agent agent, Simulator simulator) {
        return false;
    }
}

class MyController extends Controller {

    public boolean action(Agent agent, Simulator simulator, Entity entity) {
        double[] input = new double[Constants.AMOUNT_OF_CGP_INPUTS];//input za cgp ,ovisi o kolicini inputa

        double yPos = agent.position * simulator.groundLevel;
        double obstacleAhead = 0;
        //simulator.obstacleMap;

        for(int i = 0; i < Constants.VISIBILTY_DISTANCE; i++){
            if(simulator.obstacleMap[(int)yPos][i] == true){
                obstacleAhead = 1;
                break;
            }
        }

        //ovo nekako pametnije napisat
        input[0] = obstacleAhead;
        input[1] = agent.position;
        input[2] = agent.velocity;
        input[3] = simulator.groundLevel;

        //double output = 1;
        double output  = entity.entityFunction(input);

        if(output > Constants.DO_I_JUMP){
            agent.isJumping = true;
            return true;
        }
        return false;
    }









//    @Override
//    public boolean action(Agent agent, Simulator simulator) {
//        int yPos = (int) (agent.position * simulator.groundLevel);
//        boolean obstacleAhead = false;
//
//        // Check if obstacle ahead
//        for (int i = 0; i < simulator.viewWidth; i++) {
//            if (simulator.obstacleMap[yPos][i]) {
//                obstacleAhead = true;
//                break;
//            }
//        }
//
//        // jump if obstacle ahead or too low, but not if too high
//        if ((yPos > simulator.groundLevel * 0.35) && (obstacleAhead || yPos > simulator.groundLevel * 0.85)) {
//            agent.isJumping = true;
//            return true;
//        }
//        return false;
//    }
}


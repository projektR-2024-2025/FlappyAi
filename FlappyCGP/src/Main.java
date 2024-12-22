
//52: NAPRAVI OVO PAMETNIJE NESTO SA SIMULATOROROM NEVALJA PROBA DOHVATIT JEDAN ENTITY LIST(POPULACIJU) VISE NEGO STA IHBIM
public class Main {

    public static void main(String[] args) {


//        Population population1 = new Population();
//        int newPopulationFullness;
//
//        //punjenje populacije
//        //filling and evaluating initial population ==============================
//
//        for (int i = 0; i < Population.size; i++) {
//
//            Entity entity = new Entity();
//            entity.createGenome();
////            entity.fitnessEval(lenthTraversed);//lenthTraversed nez odkud ovo dobit
//            population1.entityList.add(i, entity);
//        }
//        population1.entityFitnessEval();
//        population1.entityFitnessSort();//sorting the initial entities in the population by fitness
        //String map = "src/Map1.txt";
        String map = "/Users/nikson/Documents/FER/ProjektC/FlappyCGP/out/production/FlappyCGP/Map1.txt";//jer je out folder pokrenut
        Population population1 = new Population();

        System.out.printf("runs starting...");

        for (int i = 0; i < Constants.NUMBER_OF_GENERATIONS; i++) { //vrtimo generacije
            int newPopulationFullness;
//            Population population1 = new Population();

            if(i == 0){
                //punjenje populacije
                //filling and evaluating initial population ==============================
                for (int j = 0; j < Constants.POPULATION_SIZE; j++) {
                    Entity entity = new Entity();
                    entity.createGenome();
//            entity.fitnessEval(lenthTraversed);//lenthTraversed nez odkud ovo dobit
                    population1.entityList.add(j, entity);
                }
                //population1.entityFitnessEval();
                //population1.entityFitnessSort();
            }

            for (int j = 0; j < Constants.POPULATION_SIZE; j++) {
                int lengthTraversed = 0;
                Simulator simulator = new Simulator(map);
                Agent agent = new Agent();
                MyController controller = new MyController();

                while (simulator.isRunning() && population1.entityList.size() == Constants.POPULATION_SIZE) {

//                    int len = population1.entityList.size();
//                    System.out.println(len);
//                    float ent = population1.entityList.get(1).fitness;

                    controller.action(agent, simulator, population1.entityList.get(j));
                    simulator.update(agent);
                    lengthTraversed++;
                    if (lengthTraversed >= Constants.MAX_MAP_SIZE) {
                        break;
                    }
                    population1.entityList.get(j).lengthTraversed = lengthTraversed;
                }
//                if(population1.entityList.size() == Constants.POPULATION_SIZE) {
//                    population1.entityList.get(j).lengthTraversed = lengthTraversed;
//                }
            }

            Population population2 = new Population();
            //System.out.printf("Best fitness so far is %f\n", population1.getBestEntity().fitness);//+++++++++++++
            //izrada i punjenje nove populacije
            newPopulationFullness = 0;

            while (newPopulationFullness < Constants.POPULATION_SIZE && population1.entityList.size() == Constants.POPULATION_SIZE){
                population1.chooseParents();
                newPopulationFullness = population1.crossingOverAndMutation(population2, newPopulationFullness);
                newPopulationFullness++;
            }
            population2.entityFitnessEval();
            population1 = population2;
            population1.entityFitnessSort();

        }

        Entity bestEntityFromLastPop = population1.getBestEntity();

        Simulator simulator = new Simulator(map);
        Agent agent = new Agent();
        MyController controller = new MyController();


        while (simulator.isRunning()) {
            if (simulator.handleEvents(agent) == false)
                controller.action(agent, simulator, bestEntityFromLastPop);
            simulator.update(agent);
            simulator.render();
            try {
                Thread.sleep(60);  // sleep for 60 milliseconds
            } catch (InterruptedException e) {
                e.printStackTrace();
            } // Control game speed
        }

        //system("cls");
        System.out.println("press q to quit");
        while (simulator.isScoreScreenActive()) {
            simulator.handleScoreEvents();
        }

    }
}

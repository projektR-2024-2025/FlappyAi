import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Random;

public class Population {

    static int size = Constants.POPULATION_SIZE;
    ArrayList<Entity> entityList = new ArrayList<Entity>(Constants.POPULATION_SIZE);
    Entity parent1;
    Entity parent2;



    public void entityFitnessSort() {
        // Sorting the list using an anonymous Comparator class
        //mozda tu prije sortiranja bolje stavit fitness evaluation koji je u mainu trenutno???
        this.entityList.sort(new Comparator<Entity>() {
            @Override
            public int compare(Entity e1, Entity e2) {
                return Float.compare(e1.getFitness(), e2.getFitness());
            }
        });
    }

    public Entity getBestEntity() {
        return this.entityList.get(0);
    }//ovo je za ne izgubit najbolji entity, vrati entity


    public void chooseParents(){

        Random r = new Random();
        float random = r.nextFloat();
        int halfwayPointy = Constants.POPULATION_SIZE/2;

        if(random <= Constants.BETTER_PARENT_ADVANTAGE){
            //ako bolji roditelji imaju prednost
            Random x = new Random();
            Random y = new Random();
            int random1 = x.nextInt((halfwayPointy - 0)) + 0;
            int random2 = y.nextInt((halfwayPointy - 0)) + 0;

            this.parent1 = this.entityList.get(random1);
            this.parent2 = this.entityList.get(random2);

        }else if(random > Constants.BETTER_PARENT_ADVANTAGE){
            // ako losiji roditelji imaju prednost
            Random x = new Random();
            Random y = new Random();
            int random1 = x.nextInt(Constants.POPULATION_SIZE - (halfwayPointy + 1)) + (halfwayPointy + 1);
            int random2 = y.nextInt(Constants.POPULATION_SIZE - (halfwayPointy + 1)) + (halfwayPointy + 1);

            this.parent1 = this.entityList.get(random1);
            this.parent2 = this.entityList.get(random2);
        }
    }

    void entityFitnessEval(){
        for (Entity e: this.entityList){
            e.fitness = 1 - (e.lengthTraversed/Constants.MAX_MAP_SIZE);
        }
    }


    public int crossingOverAndMutation(Population NewPop, int index){
        //vraca zadnji idex na koji je nesto stavljeno u novu populaciju ??


        if(index == 0){
            NewPop.entityList.add(0,this.getBestEntity());//dodamo najbolju jedinku u novu populaciju
            // ovo tu bi moglo van ic myb da ovaj idex krece od 1??
            return index;
        }

        int breakPoint = Constants.ENTITY_SIZE/2;
        Entity child1 = new Entity();
        Entity child2 = new Entity();


        for(int i = 0; i < breakPoint; i++){
            child1.genome.add(i, this.parent1.genome.get(i));
            child2.genome.add(i, this.parent2.genome.get(i));
        }
        for(int i = breakPoint; i < Constants.ENTITY_SIZE; i++){
            child1.genome.add(i, this.parent2.genome.get(i));
            child2.genome.add(i, this.parent1.genome.get(i));
        }

        Random r = new Random();
        float random = 0 + r.nextFloat() * (Constants.MUTATION_PROBABILITY - 0);//mislim da tu nije dobar random

        if(random <= Constants.MUTATION_PROBABILITY){
            //ako se desi mutacija

            Random z = new Random();
            Random w = new Random();

            int mutatingIndex1 = z.nextInt(0,8);
            int mutatingIndex2 = w.nextInt(0,8);
            Random a = new Random();

//TU SAM SAD STALA ################################
            if(mutatingIndex1 == 0 || mutatingIndex1 == 1){//prva dva gena, bira x ili y
                child1.genome.set(mutatingIndex1, new Gene(a.nextInt(0,2), a.nextInt(0,2)));

            }else if(mutatingIndex1 == 2 || mutatingIndex1 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica
                child1.genome.set(mutatingIndex1, new Gene(a.nextInt(0,4), a.nextInt(0,4)));

            }else if(mutatingIndex1 == 4 || mutatingIndex1 == 5){//treca dva gena, bira iz prva dva stupca kucica
                child1.genome.set(mutatingIndex1, new Gene(a.nextInt(2,6), a.nextInt(2,6)));

            }else if (mutatingIndex1 == 6){//output, bira iz druga dva stupca kucica
                int outPut = a.nextInt(4,7);
                child1.genome.set(mutatingIndex1, new Gene(outPut,outPut));
            }

            if(mutatingIndex2 == 0 || mutatingIndex2 == 1){//prva dva gena, bira x ili y
                child2.genome.set(mutatingIndex2, new Gene(a.nextInt(0,2), a.nextInt(0,2)));

            }else if(mutatingIndex2 == 2 || mutatingIndex2 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica
                child2.genome.set(mutatingIndex2, new Gene(a.nextInt(0,4), a.nextInt(0,4)));

            }else if(mutatingIndex2 == 4 || mutatingIndex2 == 5){//treca dva gena, bira iz prva dva stupca kucica
                child2.genome.set(mutatingIndex2, new Gene(a.nextInt(2,6), a.nextInt(2,6)));

            }else if (mutatingIndex2 == 6){//output, bira iz druga dva stupca kucica
                int outPut = a.nextInt(4,7);
                child2.genome.set(mutatingIndex2, new Gene(outPut,outPut));
            }


        }



        //child1.fitnessEval(lengthTraversed);
        //child2.fitnessEval(lengthTraversed);

        NewPop.entityList.add(index,child1);
        index++;

        if(index != Constants.POPULATION_SIZE){
            NewPop.entityList.add(index,child2);
        }


        return index;
    }



}
//treba jos osdabrat dva roditelja, damo prednost onim boljina



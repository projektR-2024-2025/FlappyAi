
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import static java.lang.Math.sqrt;
import static java.lang.StrictMath.pow;
import static java.lang.StrictMath.sin;


public class Entity {

    List<Gene> genome = new ArrayList<>();
    float fitness;
    float lengthTraversed;
    //List<Step> steps = new ArrayList<>();

    //(x,y), 6 kucica, output


    public List<Gene> genome() {
        return genome;
    }

    public float getFitness() {
        return fitness;
    }

    public void createGenome(){//creating a genome for the entity
        for (int i = 0; i < Constants.ENTITY_SIZE; i++) { //ide od 0 do 6 tolko razlicitih gena ima

            int funcIndex;
            Random a = new Random();

            if(i == 0 || i == 1){//prva dva gena, bira iz inputa(array of inputs)
                Gene geneSet= new Gene(a.nextInt(0,Constants.AMOUNT_OF_CGP_INPUTS), a.nextInt(0,Constants.AMOUNT_OF_CGP_INPUTS));
                this.genome.add(geneSet);

            }else if(i == 2 || i == 3){//druga dva gena, bira iz inputa(array of inputs) ili iz prvog stupca kucica
                Gene geneSet= new Gene(a.nextInt(0,Constants.AMOUNT_OF_CGP_INPUTS+2), a.nextInt(0,Constants.AMOUNT_OF_CGP_INPUTS+2));
                this.genome.add(geneSet);

            }else if(i == 4 || i == 5){//treca dva gena, bira iz prvog i drugog  stupca kucica
                Gene geneSet= new Gene(a.nextInt(Constants.AMOUNT_OF_CGP_INPUTS,Constants.AMOUNT_OF_CGP_INPUTS+4), a.nextInt(Constants.AMOUNT_OF_CGP_INPUTS,Constants.AMOUNT_OF_CGP_INPUTS+4));
                this.genome.add(geneSet);

            }else if (i == 6){//output, bira iz drugog i treceg stupca kucica
                int outPut = a.nextInt(Constants.AMOUNT_OF_CGP_INPUTS+2,Constants.AMOUNT_OF_CGP_INPUTS+6);
                Gene geneSet= new Gene(outPut, outPut);
                this.genome.add(geneSet);

            }

        }

    }

//    float realFunction(float x, float y, String functionIndex){
//
//        if(functionIndex.equals("1")){
//            //using first function
//            return (float) (Math.pow(x, 2) + 2*x*y + y);
//
//        }else if(functionIndex.equals("2")){
//            //using second function
//            return (float) (Math.pow(y,2) + Math.cos(x*y));
//        }
//
//        return 0;//nez dal je ovo pametno
//    }

    //prepravit ovu funkciju da prima array doubleova, variable length of arraya



    double entityFunction(double[] cgpInputs) {

        //kako sad funkciju iz genoma izvuc

        double[] temp = new double[Constants.ENTITY_SIZE + Constants.AMOUNT_OF_CGP_INPUTS];

        for(int i = 0; i < cgpInputs.length; i++){
            temp[i] = cgpInputs[i];
        }


        for (int i = cgpInputs.length; i < Constants.ENTITY_SIZE + cgpInputs.length; i++) {

            int in1 = this.genome.get(i - cgpInputs.length).in1;
            int in2 = this.genome.get(i - cgpInputs.length).in2;


            String func = this.genome.get(i - cgpInputs.length).function.toString();
            double result = 0;

            double a = temp[in1];
            double b = temp[in2];

// PLUS, MINUS, MUL, DIV, MOD, COS, SIN, POW;
            if (func.equals("PLUS")) {
                result = a + b;

            } else if (func.equals("MINUS")) {
                result = a - b;

            } else if (func.equals("MUL")) {
                result = a * b;

            } else if (func.equals("DIV")) {
                result = a / b;

            } else if (func.equals("MOD")) {
                result = a % b;

            } else if (func.equals("POW")) {
                result = (float) Math.pow(a, b);

            } else if (func.equals("SIN")) {
                result = (float) Math.sin(a + b);

            } else if (func.equals("COS")) {
                result = (float) Math.cos(a + b);
            }

            temp[i] = result;
        }

        int outputIndex = this.genome.get(Constants.ENTITY_SIZE - 1).in1;

        return temp[outputIndex];
    }




//    public void fitnessEval(float lengthTraversed) {
//        //fitness calculation
//        this.fitness = 1 - (lengthTraversed/Constants.MAX_MAP_SIZE);
//    }

    public String toString(){
        String output = "[ ";

        for(Gene gene: genome){
            output = output + gene.in1 + gene.in2 + gene.function.toString() + " ";
        }
        output = output + " ] " + fitness;

        return output;
    }
//    float entityFunction(float x, float y, Entity e){
//
//        //kako sad funkciju iz genoma izvuc
//
//        float[] temp = new float[Constants.ENTITY_SIZE + 1];
//        temp[0] = x;
//        temp[1] = y;
//
//        for(int i = 2; i < Constants.ENTITY_SIZE + 1; i++){
//
//            int in1 = e.genome.get(i - 2).in1;
//            int in2 = e.genome.get(i - 2).in2;
//            String func = e.genome.get(i - 2).function.toString();
//            float result = 0;
//
//            float a = temp[in1];
//            float b = temp[in2];
//
//// PLUS, MINUS, MUL, DIV, MOD, COS, SIN, POW;
//            if(func.equals("PLUS")){
//                result = a + b;
//
//            }else if(func.equals("MINUS")){
//                result = a - b;
//
//            }else if(func.equals("MUL")){
//                result = a * b;
//
//            }else if(func.equals("DIV")){
//                result = a / b;
//
//            }else if(func.equals("MOD")){
//                result = a % b;
//
//            }else if(func.equals("POW")){
//                result = (float) Math.pow(a,b);
//
//            }else if(func.equals("SIN")){
//                result = (float) Math.sin(a+b);
//
//            }else if(func.equals("COS")){
//                result = (float) Math.cos(a+b);
//            }
//
//            temp[i] = result;
//        }
//
//        int outputIndex = e.genome.get(Constants.ENTITY_SIZE - 1).in1;
//
//        return temp[outputIndex];
//    }




//    public void fitnessEval(float[] x, float[] y, String realFunctionIndex) {
//        //fitness calculation
//        //tu moze bit da za dva broja izracunamo pravu funkciju i entity funkciju itd.
//        float sum = 0;
//
//        for (int i = 0; i < x.length && i < y.length; i++) {
//
//            float fReal = realFunction(x[i], y[i], realFunctionIndex);
//            float fEntity = entityFunction(x[i], y[i], this);
//
//            float squared = (float) Math.pow(fReal - fEntity,2);
//
//            sum += squared;
//        }
//
//        float avg = sum / x.length;
//
//        this.fitness = (float) Math.sqrt(avg);
//    }

    //the smaller the fitness the better the entity is

}









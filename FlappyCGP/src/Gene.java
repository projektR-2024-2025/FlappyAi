import java.util.Random;

enum Functions{
    PLUS, MINUS, MUL, DIV, MOD, COS, SIN, POW;
}

public class Gene {

    int in1;
    int in2;
    Functions function;



    public Gene(int in1, int in2) {
        Random a = new Random();
        int index = a.nextInt(0, 8);

        this.in1 = in1;
        this.in2 = in2;
        this.function = Functions.values()[index];
    }

//    public void getFunction() {
//
//            Random a = new Random();
//
//            int index = a.nextInt(0, 8);
//
//            switch (index) {
//                case 0:
//                    this.function = Functions.PLUS;
//
//                case 1:
//                    this.function = Functions.MINUS;
//
//                case 2:
//                    this.function = Functions.MUL;
//
//                case 3:
//                    this.function = Functions.DIV;
//
//                case 4:
//                    this.function = Functions.MOD;
//
//                case 5:
//                    this.function = Functions.COS;
//
//                case 6:
//                    this.function = Functions.SIN;
//
//                case 7:
//                    this.function = Functions.POW;
//
//            }
//
//        }

}





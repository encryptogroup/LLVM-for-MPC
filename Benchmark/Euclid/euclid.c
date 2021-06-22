#include <stdio.h>
int mpc_main(int INPUT_A, int INPUT_B) {
      int result1= INPUT_A*INPUT_A;
      int result2= INPUT_B*INPUT_B;
      int result3= result1+result2;
      return result3;
}

int main(){
printf("%u\n",mpc_main(4,3));
}

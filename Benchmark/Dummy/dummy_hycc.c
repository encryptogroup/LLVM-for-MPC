#include <stdio.h>

struct I_A{
	int a;
	int b;
};

int mpc_main(struct I_A INPUT_A, int INPUT_B) {
    int array[8];
    for(int i=0;i<8;i++){
        array[i]=INPUT_A.a+INPUT_A.b*i;
    }
    int ret=0;
    if(INPUT_B<array[INPUT_B]){
        ret = array[2]+array[3];
    }
    else{
        ret = array[0]*array[1];
    }
    return ret;
}


int main(){
	int a;
	int b;
        int c;
	scanf("%d",&a);
	scanf("%d",&b);
        scanf("%d",&c);
	struct I_A inp;
	inp.a=a;
	inp.b=b;
	printf("dummy: %d\n",mpc_main(inp,c));
}



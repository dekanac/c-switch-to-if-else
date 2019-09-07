int x_sqared(int x){
    x*x;
}

int func() {
    int c,i;
    c = 2;
    i = 0;
    switch (5) {

        case 1:
            c = 1;
        case 5:
            while(i < 3){
                i = i+1;
                if(i > 1){
                    c = x_sqared(c);
                }
                else {
                    c = c + 8;
                }
            }
        case 7:
            c = 3;
        case 8:
            c = 5;
            break;
    }
    c;
}

int main() {
    int c;
    c = 0;
    switch (5) {
        
        case 1:
            c = 2;
        case 5:
            c = 2;
        case 3:
            c = 3;
        case 5:
            c = 99;
    }
    c;
}

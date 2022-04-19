#define ORANGE_R 235
#define ORANGE_G 133
#define ORANGE_B 9
#define ORANGE_RAND_R 219
#define ORANGE_RAND_G 196
#define ORANGE_RAND_B 167

#define HEIGHT 100
#define WIDTH 10
#define HEIGHT_CYL 5

#define PARTS 9


void specialToetsPress(int toets, int x_muis, int y_muis);
void toetsPress(unsigned char toets, int x_muis, int y_muis);
void drawBalk();
void drawAssen();
void drawVakwerk(char vlak);
void drawTopVlak();
void drawCylinderVast();
void drawCylinderLos();
void drawTopPart();
void drawCabine();
void drawArm();
void drawVakwerkArm();
void drawBalDing();
void drawKabine();
void drawKraan();
void drawGewichten();
void animFunc(int delta);
void letThereBeLight();

/*

    up: camera in z +
    down: camera in z -
    right: camera in x +
    left: camera in x -

    p: draai bovenkant met klok
    P: draai bovenkant tegen klok
    o: verplaats bolplatform naar buiten
    O: verplaats bolplatform naar binnen
    i: verleng de kabel met bal
    I: verkort de kabel met bal

*/
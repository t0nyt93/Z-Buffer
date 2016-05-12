/*

Line and matrix header.

*/

void draw_line(double, double, double, double);
int near_far_clip(double,double,double*,double*,double*,double*,double*,double*);
void osuInitialize();
void osuPushMatrix();
void osuPopMatrix();
void osuLoadIdentityMatrix();
void osuTranslate(double, double, double);
void osuScale(double, double, double);
void osuRotate(double angle, double ax, double ay, double az);
void osuOrtho(double, double, double, double, double, double);
void osuPerspective(double, double, double);
void osuLookat(double from[3], double at[3], double up[3]);
void osuBegin(OSUDrawable);
void osuEnd();
void osuVertex3f(double, double, double);
void osuNormal3f(double, double, double);
void osuEnable(int);
void osuClearZ();
void osuShadeModel(int);
void osuPointLight(float a[3], float);
void osuDirectionalLight(float dir[3], int);
void osuAmbientLight(float);
void osuDiffuse(float, float, float);
void osuSpecular(float, float, float, float);





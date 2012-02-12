#include <stdlib.h>
#include <GL/glut.h>
#include "GLCapture.hpp"

//OpenCV2.2の場合
#ifdef _DEBUG
    //Debugモードの場合
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_core220d.lib")            // opencv_core
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_imgproc220d.lib")        // opencv_imgproc
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_highgui220d.lib")        // opencv_highgui
#else
    //Releaseモードの場合
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_core220.lib")            // opencv_core
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_imgproc220.lib")        // opencv_imgproc
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_highgui220.lib")        // opencv_highgui
#endif

//openGLのための宣言・定義
//---変数宣言---
int FormWidth = 640;
int FormHeight = 480;
//---マクロ定義---
#define glFovy 45 //視角度
#define glZNear 1.0 //near面の距離
#define glZFar 150.0 //far面の距離

GLfloat lightPosition[4] = {0.25f, 1.0f, 0.25f, 0.0f};
GLfloat lightDiffuse[3] = {1.0f, 1.0f, 1.0f};
GLfloat lightAmbient[3] = {0.25f, 0.25f, 0.25f};
GLfloat lightSpecular[3] = {1.0f, 1.0f, 1.0f};
	
GLfloat diffuse[3] = {1.0f, 1.0f, 0.0f};
GLfloat ambient[3] = {0.25f, 0.25f, 0.25f};
GLfloat specular[3] = {1.0f, 1.0f, 1.0f};
GLfloat shininess[1] = {32.0f};

GLCapture glCapture;

//初期化
void init(){	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glCapture.setWriteFile();
	//引数で細かく設定
	//glCapture.setWriteFile("output2.avi",CV_FOURCC('M','J','P','G'),60.0f,cv::Size(320,240));

	//拡大縮小時の補間方法の設定（デフォルトはバイリニア補間）
	glCapture.setInterpolation(cv::INTER_AREA);		//ピクセル領域の関係を利用したリサンプリングに変更
}
//描画
void display(){

    // clear screen and depth buffer
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Reset the coordinate system before modifying
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST); //「Zバッファ」を有効


	glPushMatrix();
	glTranslatef(0,0,-5.0f);

	static float angle = 0.0f;
	glRotatef(angle,0,1,0);
	glutSolidTeapot(1);

	glPopMatrix();

	angle += 1;	//回転角度

  
    glFlush();
    glutSwapBuffers();

	glCapture.write();

}
// アイドル時のコールバック
void idle(){
    //再描画要求
    glutPostRedisplay();
}
//ウィンドウのサイズ変更
void reshape (int width, int height){
    FormWidth = width;
    FormHeight = height;
    glViewport (0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    //射影変換行列の指定
    gluPerspective (glFovy, (GLfloat)width / (GLfloat)height,glZNear,glZFar);
    glMatrixMode (GL_MODELVIEW);		
}
//メイン
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(FormWidth, FormHeight);
    glutCreateWindow("GLCapture Sample");
    //コールバック
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
	init();
    glutMainLoop();
    return 0;
}
#include <stdlib.h>
#include <GL/glut.h>
#include "GLCapture.hpp"

//OpenCV2.2�̏ꍇ
#ifdef _DEBUG
    //Debug���[�h�̏ꍇ
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_core220d.lib")            // opencv_core
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_imgproc220d.lib")        // opencv_imgproc
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_highgui220d.lib")        // opencv_highgui
#else
    //Release���[�h�̏ꍇ
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_core220.lib")            // opencv_core
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_imgproc220.lib")        // opencv_imgproc
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_highgui220.lib")        // opencv_highgui
#endif

//openGL�̂��߂̐錾�E��`
//---�ϐ��錾---
int FormWidth = 640;
int FormHeight = 480;
//---�}�N����`---
#define glFovy 45 //���p�x
#define glZNear 1.0 //near�ʂ̋���
#define glZFar 150.0 //far�ʂ̋���

GLfloat lightPosition[4] = {0.25f, 1.0f, 0.25f, 0.0f};
GLfloat lightDiffuse[3] = {1.0f, 1.0f, 1.0f};
GLfloat lightAmbient[3] = {0.25f, 0.25f, 0.25f};
GLfloat lightSpecular[3] = {1.0f, 1.0f, 1.0f};
	
GLfloat diffuse[3] = {1.0f, 1.0f, 0.0f};
GLfloat ambient[3] = {0.25f, 0.25f, 0.25f};
GLfloat specular[3] = {1.0f, 1.0f, 1.0f};
GLfloat shininess[1] = {32.0f};

GLCapture glCapture;

//������
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
	//�����ōׂ����ݒ�
	//glCapture.setWriteFile("output2.avi",CV_FOURCC('M','J','P','G'),60.0f,cv::Size(320,240));

	//�g��k�����̕�ԕ��@�̐ݒ�i�f�t�H���g�̓o�C���j�A��ԁj
	glCapture.setInterpolation(cv::INTER_AREA);		//�s�N�Z���̈�̊֌W�𗘗p�������T���v�����O�ɕύX
}
//�`��
void display(){

    // clear screen and depth buffer
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Reset the coordinate system before modifying
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST); //�uZ�o�b�t�@�v��L��


	glPushMatrix();
	glTranslatef(0,0,-5.0f);

	static float angle = 0.0f;
	glRotatef(angle,0,1,0);
	glutSolidTeapot(1);

	glPopMatrix();

	angle += 1;	//��]�p�x

  
    glFlush();
    glutSwapBuffers();

	glCapture.write();

}
// �A�C�h�����̃R�[���o�b�N
void idle(){
    //�ĕ`��v��
    glutPostRedisplay();
}
//�E�B���h�E�̃T�C�Y�ύX
void reshape (int width, int height){
    FormWidth = width;
    FormHeight = height;
    glViewport (0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    //�ˉe�ϊ��s��̎w��
    gluPerspective (glFovy, (GLfloat)width / (GLfloat)height,glZNear,glZFar);
    glMatrixMode (GL_MODELVIEW);		
}
//���C��
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(FormWidth, FormHeight);
    glutCreateWindow("GLCapture Sample");
    //�R�[���o�b�N
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
	init();
    glutMainLoop();
    return 0;
}
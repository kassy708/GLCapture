/**
	GLCapture.hpp
	@version 1.0
	@require Opencv2.0 or later
	@require OpenGL
	@author kassy708 http://twitter.com/kassy708

	@description
	OpenGL�̃E�B���h�E���L���v�`������N���X

	@license
	The MIT License
	Copyright (c) 2012/02/12 kassy708

	�ȉ��ɒ�߂�����ɏ]���A
	�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C���i�ȉ��u�\�t�g�E�F�A�v�j�̕������擾���邷�ׂĂ̐l�ɑ΂��A
	�\�t�g�E�F�A�𖳐����Ɉ������Ƃ𖳏��ŋ����܂��B
	����ɂ́A�\�t�g�E�F�A�̕������g�p�A���ʁA�ύX�A�����A�f�ځA�Еz�A�T�u���C�Z���X�A
	�����/�܂��͔̔����錠���A����у\�t�g�E�F�A��񋟂��鑊��ɓ������Ƃ������錠�����������Ɋ܂܂�܂��B

	��L�̒��쌠�\������і{�����\�����A�\�t�g�E�F�A�̂��ׂĂ̕����܂��͏d�v�ȕ����ɋL�ڂ�����̂Ƃ��܂��B

	�\�t�g�E�F�A�́u����̂܂܁v�ŁA�����ł��邩�Öقł��邩���킸�A����̕ۏ؂��Ȃ��񋟂���܂��B
	�����ł����ۏ؂Ƃ́A���i���A����̖ړI�ւ̓K�����A����ь�����N�Q�ɂ��Ă̕ۏ؂��܂݂܂����A����Ɍ��肳�����̂ł͂���܂���B 
	��҂܂��͒��쌠�҂́A�_��s�ׁA�s�@�s�ׁA�܂��͂���ȊO�ł��낤�ƁA�\�t�g�E�F�A�ɋN���܂��͊֘A���A
	���邢�̓\�t�g�E�F�A�̎g�p�܂��͂��̑��̈����ɂ���Đ������؂̐����A���Q�A���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��B 
*/

#include <GL/glut.h>
#include <opencv2/opencv.hpp>

//�����̃R�[�f�b�N�A�p�\�R���ɓ����Ă�D���ȃR�[�f�b�N�ɂ���
#define DEFAULT_CODEC CV_FOURCC('X','V','I','D')

/*����R�[�f�b�N��*/
//#define DEFAULT_CODEC CV_FOURCC('P','I','M','1')	//MPEG-1
//#define DEFAULT_CODEC CV_FOURCC('M','P','G','4')	//MPEG-4
//#define DEFAULT_CODEC CV_FOURCC('M','P','4','2')	//MPEG-4.2
//#define DEFAULT_CODEC CV_FOURCC('D','I','V','X')	//DivX
//#define DEFAULT_CODEC CV_FOURCC('D','X','5','0')	//DivX ver 5.0
//#define DEFAULT_CODEC CV_FOURCC('X','V','I','D')	//Xvid
//#define DEFAULT_CODEC CV_FOURCC('U','2','6','3')	//H.263
//#define DEFAULT_CODEC CV_FOURCC('H','2','6','4')	//H.264
//#define DEFAULT_CODEC CV_FOURCC('F','L','V','1')	//FLV1
//#define DEFAULT_CODEC CV_FOURCC('M','J','P','G')	//Motion JPEG
//#define DEFAULT_CODEC 0							//�񈳏k
//#define DEFAULT_CODEC -1							//�I��


//GL_TEXTURE_2D���g�������@�i�������̂ق��������Ƃ����\�j
#define USE_TEXTURE_2D

class GLCapture{
private:
	cv::Mat captureImage;
	cv::VideoWriter videoWriter;
	int interpolation;	//��Ԏ�@
public:
	GLCapture(){
		interpolation = cv::INTER_LINEAR;
	}
	/**
	const char* filename	�t�@�C����
	int fourcc				�R�[�f�b�N
	double fps				fps
	cv::Size size			�T�C�Y
	*/
	GLCapture(const char* filename,int fourcc = DEFAULT_CODEC,double fps = 30.0f, cv::Size size = cv::Size(glutGet(GLUT_WINDOW_WIDTH),glutGet( GLUT_WINDOW_HEIGHT))){
		setWriteFile(filename,fourcc, fps, size);
	}
	~GLCapture(){
	}
	/**
	const char* filename	�t�@�C����
	int fourcc				�R�[�f�b�N
	double fps				fps
	cv::Size size			�T�C�Y
	*/
	void setWriteFile(const char* filename = "output.avi",int fourcc = DEFAULT_CODEC,double fps = 30.0f, cv::Size size = cv::Size(glutGet(GLUT_WINDOW_WIDTH),glutGet( GLUT_WINDOW_HEIGHT))){
		videoWriter = cv::VideoWriter::VideoWriter(filename,fourcc, fps, size); 
		captureImage = cv::Mat(size,CV_8UC3);
	}
	/**
		����t�@�C���ɏ�������
	*/
	bool write(){
		if( videoWriter.isOpened()){
			cv::Size size = cv::Size(glutGet(GLUT_WINDOW_WIDTH),glutGet( GLUT_WINDOW_HEIGHT));
			if(size.width == captureImage.cols && size.height == captureImage.rows){
#ifndef USE_TEXTURE_2D
				glReadPixels(0,0, captureImage.cols,captureImage.rows,GL_RGB,GL_UNSIGNED_BYTE,captureImage.data);
#else
				glCopyTexImage2D( GL_TEXTURE_2D,0,GL_RGB,0,0,captureImage.cols,captureImage.rows,0);
				glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,captureImage.data);
#endif
			}
			else{	//�E�B���h�E���g��k������Ă���ꍇ
				size.width -= size.width % 4;	//4�̔{���ɂ���
				cv::Mat temp(size,CV_8UC3);
#ifndef USE_TEXTURE_2D
				glReadPixels(0,0, size.width,size.height,GL_RGB,GL_UNSIGNED_BYTE,temp.data);
#else
				glCopyTexImage2D( GL_TEXTURE_2D,0,GL_RGB,0,0,size.width,size.height,0);
				glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,temp.data);
#endif
				cv::resize(temp,captureImage,captureImage.size(),0.0,0.0,interpolation);
			}
			cvtColor(captureImage,captureImage,CV_RGB2BGR);   
			flip(captureImage,captureImage,0);
			videoWriter << captureImage;
			return true;

		}
		else
			return false;
	}
	/**
		�g��k�����̕�ԕ��@���擾
	*/
	int getInterpolation(){
		return interpolation;
	}
	/**
		�g��k�����̕�ԕ��@��ݒ�
	*/
	void setInterpolation(int interpolation){
		this->interpolation = interpolation;
	}

};
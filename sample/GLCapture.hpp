/**
	GLCapture.hpp
	@version 1.0
	@require Opencv2.0 or later
	@require OpenGL
	@author kassy708 http://twitter.com/kassy708

	@description
	OpenGLのウィンドウをキャプチャするクラス

	@license
	The MIT License
	Copyright (c) 2012/02/12 kassy708

	以下に定める条件に従い、
	本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）の複製を取得するすべての人に対し、
	ソフトウェアを無制限に扱うことを無償で許可します。
	これには、ソフトウェアの複製を使用、複写、変更、結合、掲載、頒布、サブライセンス、
	および/または販売する権利、およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます。

	上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製または重要な部分に記載するものとします。

	ソフトウェアは「現状のまま」で、明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。
	ここでいう保証とは、商品性、特定の目的への適合性、および権利非侵害についての保証も含みますが、それに限定されるものではありません。 
	作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフトウェアに起因または関連し、
	あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求、損害、その他の義務について何らの責任も負わないものとします。 
*/

#include <GL/glut.h>
#include <opencv2/opencv.hpp>

//初期のコーデック、パソコンに入ってる好きなコーデックにする
#define DEFAULT_CODEC CV_FOURCC('X','V','I','D')

/*動画コーデック例*/
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
//#define DEFAULT_CODEC 0							//非圧縮
//#define DEFAULT_CODEC -1							//選択


//GL_TEXTURE_2Dを使った方法（こっちのほうが速いという噂）
#define USE_TEXTURE_2D

class GLCapture{
private:
	cv::Mat captureImage;
	cv::VideoWriter videoWriter;
	int interpolation;	//補間手法
public:
	GLCapture(){
		interpolation = cv::INTER_LINEAR;
	}
	/**
	const char* filename	ファイル名
	int fourcc				コーデック
	double fps				fps
	cv::Size size			サイズ
	*/
	GLCapture(const char* filename,int fourcc = DEFAULT_CODEC,double fps = 30.0f, cv::Size size = cv::Size(glutGet(GLUT_WINDOW_WIDTH),glutGet( GLUT_WINDOW_HEIGHT))){
		setWriteFile(filename,fourcc, fps, size);
	}
	~GLCapture(){
	}
	/**
	const char* filename	ファイル名
	int fourcc				コーデック
	double fps				fps
	cv::Size size			サイズ
	*/
	void setWriteFile(const char* filename = "output.avi",int fourcc = DEFAULT_CODEC,double fps = 30.0f, cv::Size size = cv::Size(glutGet(GLUT_WINDOW_WIDTH),glutGet( GLUT_WINDOW_HEIGHT))){
		videoWriter = cv::VideoWriter::VideoWriter(filename,fourcc, fps, size); 
		captureImage = cv::Mat(size,CV_8UC3);
	}
	/**
		動画ファイルに書き込み
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
			else{	//ウィンドウが拡大縮小されている場合
				size.width -= size.width % 4;	//4の倍数にする
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
		拡大縮小時の補間方法を取得
	*/
	int getInterpolation(){
		return interpolation;
	}
	/**
		拡大縮小時の補間方法を設定
	*/
	void setInterpolation(int interpolation){
		this->interpolation = interpolation;
	}

};
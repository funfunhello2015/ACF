/*
 * Util.h
 *
 *  Created on: 2015年11月25日
 *      Author: edison
 */

#ifndef SRC_UTIL_UTIL_H_
#define SRC_UTIL_UTIL_H_

#include "common.h"

//enum COLORSPACE{
//	YUV,RGB,HSV,GRAY
//};

namespace acf{
class BoundingBox{
private:
	cv::Rect_<float> box;
	float score;
public:
	BoundingBox(float x,float y,float width,float height,float score):box(x,y,width,height),score(score){};
	~BoundingBox(){};
};

class Size{
public:
	int height;
	int width;
	Size(int h,int w):height(h),width(w){}
	Size():height(0),width(0){}
	friend std::ostream& operator<<(std::ostream&os ,const Size& s){
		os << "["<<s.height <<" X "<<s.width<< "]";
		return os;
	};
};

/**
 * here is Matrix class to store datas
 * the Matrix will stored int column first order
 * the index is started from 0
 */
template < typename dataType>
class Matrix{
private:
	int chOffset;
	int colOffset;
	int * refCount;
//	int* index;
//	Matrix& operator=(const Matrix& m);
public:
	int cols;
	int rows;
	int nChns;
	dataType* datas;
	Matrix(int c,int r,int nC=1):cols(c),rows(r),nChns(nC){
		assert(c>0&&r>0&&nC>0);
		OUT("Matrix() init with value");
		Size imgSize(rows,cols);
		OUT_V(imgSize);
		datas = new dataType[cols*rows*nChns];
		chOffset = rows*cols;
		colOffset = rows;
		refCount = new int;
		*refCount = 1;
//		index = new int[cols*rows*nChns];
//		int i = 0;
//		for(int ch=0;ch<nChns;ch++){
//			int chOff = rows*cols*ch;
//			for(int co=0;co<cols;co++){
//				int coOff = co*rows;
//				for(int ro=0;ro<rows;ro++){
//					index[ i++] = chOff+coOff+ro;
//				}
//			}
//		}
	}
	Matrix():cols(0),rows(0),nChns(0){
		OUT("Matrix()");
		chOffset = 0;
		colOffset = 0;
		datas = NULL;
		refCount = new int;
		*refCount = 0;
	}
	Matrix(const Matrix& m):cols(m.cols),rows(m.rows),nChns(m.nChns){
		chOffset = m.chOffset;
		colOffset = m.colOffset;
		datas = m.datas;
		refCount = m.refCount;
		(*refCount)++;
	}
	~Matrix(){
		OUT("~Matrix()");
		(*refCount)--;
		if(*refCount==0){
			OUT("delete Matrix Data");
			delete[] datas;
			delete refCount;
		}
//		delete[] index;
	}
	void setData(dataType* d,int w,int h,int c){
		assert(this->datas==NULL);
		this->datas = d;
		this->rows = h;
		this->cols = w;
		this->nChns = c;
		this->chOffset = rows*cols;
		this->colOffset = rows;
		(*refCount)++;
	}

	dataType at(int c,int r,int ch){
		if((c<0||c>cols) ||(r<0||r>rows) ||(ch<0||ch>nChns) ){
			throw std::range_error("Invalid Index");
		}
		return datas[ch*chOffset+c*colOffset+r];
	}
	friend std::ostream& operator<<(std::ostream&os ,const Matrix& m){
		os << "Mat *refCount="<< *(m.refCount)<< "\t[" << m.rows << " X " << m.cols << " X " << m.nChns << "]:\n";
		for(int i=0;i<m.nChns;i++){
			for(int r=0;r<m.rows;r++){
				for(int c=0;c<m.cols;c++)
					os << m.datas[i*m.chOffset+c*m.colOffset+r] << "\t";
				os << "\n";
			}
			os << "\n";
		}
		return os;
	}
};

typedef Matrix<float> MatrixD;

}
#endif /* SRC_UTIL_UTIL_H_ */

//
//  xuQuadWarp.cpp
//  warpTest
//
//  Created by Rodrigo Torres González on 29/09/13.
//
//

#include "xuQuadWarp.h"


xuQuadWarp::xuQuadWarp(void)
{
	puntoActivo = -1;
	//de entrada nos vamos a un cuadro de 320x240
	puntos[0] = ofPoint( 10, 10 );
	puntos[1] = ofPoint( 330, 10 );
	puntos[2] = ofPoint( 330, 250 );
	puntos[3] = ofPoint( 10,250 );
	tol = 6.0;
	radio = 4.0;
	seleccionado = true;
	pintarTh = false;
	offSet = ofPoint(400,300);
    modificar = true;
    
	for(int i = 0; i < 16; i++){
		if(i % 5 != 0) homografia[i] = 0.0;
		else homografia[i] = 1.0;
	}
    
}


void xuQuadWarp::setSuperficie(ofTexture &superficie)
{
	inSuperficie = superficie;
	updateImage();
}

void xuQuadWarp::setPuntos(ofRectangle dims)
{
	puntos[0] = ofPoint( dims.getLeft(), dims.getTop() );
	puntos[1] = ofPoint( dims.getRight(), dims.getTop() );
	puntos[2] = ofPoint( dims.getRight(), dims.getBottom() );
	puntos[3] = ofPoint( dims.getLeft(),dims.getBottom() );
}

int xuQuadWarp::masCercano( const ofPoint &punto, float tolerancia )
{
	int result = -1;
	float minDist;
	for( size_t i = 0; i < 4; ++i ) {
		float dist = puntos[i].distance( punto );
		if( dist < tolerancia ) {
			if( ( result == -1 ) || ( dist < minDist ) ) {
				result = i;
				minDist = dist;
			}
		}
	}
	
	return result;
}

void xuQuadWarp::mouseDown(int x, int y)
{
	if(isInBounds(ofPoint(x,y)))
    {
        cout<<"in bounds"<<endl;
        overMe = true;
        offSet = ofPoint(x,y);
    
    }else
    {
        overMe = false;
    }
    
    puntoActivo = masCercano(ofPoint(x,y),tol);
    
}

void xuQuadWarp::mouseDrag( int x, int y )
{
	if( puntoActivo >= 0 ) {
		puntos[puntoActivo] = ofPoint(x,y);
		updateImage();
	}else
    {
        if(overMe)
        {
            float deltaX = offSet.x - x;
            float deltaY = offSet.y - y;
            for( int i = 0; i < 4; ++i )
            {
                puntos[i].x -= deltaX;
                puntos[i].y -= deltaY;
            }
            offSet = ofPoint(x,y);
            updateImage();
            
        }
    }
}

void xuQuadWarp::mouseUp()
{
	puntoActivo = -1;
    overMe = false;
}


void xuQuadWarp::updateImage()
{
	//cv::Mat input( toOcv( inSuperficie ) ), output;
	ofPoint src[4];
	src[0] = ofPoint( 0, 0 );
	src[1] = ofPoint( inSuperficie.getWidth(), 0 );
	src[2] = ofPoint( inSuperficie.getWidth(), inSuperficie.getHeight() );
	src[3] = ofPoint( 0, inSuperficie.getHeight() );
	
	
	findHomography( src, puntos,  homografia);
	
}

void xuQuadWarp::draw()
{
	
	if(seleccionado)
	{
		glPushMatrix();
		ofSetColor(255);
		glMultMatrixf(homografia);
		inSuperficie.draw(0, 0);
        
		glColor3f( 1.0f, 0.5f, 0.25f );
		glPopMatrix();
		
        if (modificar) {
            //ofBeginShape();
            for( int i = 0; i < 4; ++i )
            {
                
                ofCircle( puntos[i], radio );
            }
            ofNoFill();
            //ofEndShape();
        }
        
        
        
	}
    
    
}

bool xuQuadWarp::isInBounds(ofPoint px )
{
    if (px.x > getLeft().x && px.x < getRight().x && px.y > getBottom().y && px.y < getTop().y)
    {
        return true;
    }else
    {
        return false;
    }
    
}

ofPoint xuQuadWarp::getLeft()
{
    ofPoint left = puntos[0];
    for( int i = 1; i < 4; ++i )
    {
        if(left.x > puntos[i].x)
        {
            left = puntos[i];
            
        }
    }
    return left;
}

ofPoint xuQuadWarp::getRight()
{
    ofPoint right = puntos[0];
    for( int i = 1; i < 4; ++i )
    {
        if(right.x < puntos[i].x)
        {
            right = puntos[i];
            
        }
    }
    return right;
}

ofPoint xuQuadWarp::getTop()
{
    ofPoint top = puntos[0];
    for( int i = 1; i < 4; ++i )
    {
        if(top.y < puntos[i].y)
        {
            top = puntos[i];
            
        }
    }
    return top;
    
}
ofPoint xuQuadWarp::getBottom()
{
    ofPoint bottom = puntos[0];
    for( int i = 1; i < 4; ++i )
    {
        if(bottom.y > puntos[i].y)
        {
            bottom = puntos[i];
            
        }
    }
    return bottom;
    
}

xuQuadWarp::~xuQuadWarp(void)
{
}


/*
 * homography.h
 *
 *  Created on: 08/01/2010
 *  Author: arturo castro
 */



void xuQuadWarp::gaussian_elimination(float *input, int n){
	// ported to c from pseudocode in
	// http://en.wikipedia.org/wiki/Gaussian_elimination
    
	float * A = input;
	int i = 0;
	int j = 0;
	int m = n-1;
	while (i < m && j < n){
        // Find pivot in column j, starting in row i:
        int maxi = i;
        for(int k = i+1; k<m; k++){
            if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
                maxi = k;
            }
        }
        if (A[maxi*n+j] != 0){
            //swap rows i and maxi, but do not change the value of i
            if(i!=maxi)
                for(int k=0;k<n;k++){
                    float aux = A[i*n+k];
                    A[i*n+k]=A[maxi*n+k];
                    A[maxi*n+k]=aux;
                }
            //Now A[i,j] will contain the old value of A[maxi,j].
            //divide each entry in row i by A[i,j]
            float A_ij=A[i*n+j];
            for(int k=0;k<n;k++){
                A[i*n+k]/=A_ij;
            }
            //Now A[i,j] will have the value 1.
            for(int u = i+1; u< m; u++){
                //subtract A[u,j] * row i from row u
                float A_uj = A[u*n+j];
                for(int k=0;k<n;k++){
                    A[u*n+k]-=A_uj*A[i*n+k];
                }
                //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
            }
            
            i++;
        }
        j++;
	}
    
	//back substitution
	for(int i=m-2;i>=0;i--){
		for(int j=i+1;j<n-1;j++){
			A[i*n+m]-=A[i*n+j]*A[j*n+m];
			//A[i*n+j]=0;
		}
	}
}

void xuQuadWarp::findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]){
    
	// create the equation system to be solved
	//
	// from: Multiple View Geometry in Computer Vision 2ed
	//       Hartley R. and Zisserman A.
	//
	// x' = xH
	// where H is the homography: a 3 by 3 matrix
	// that transformed to inhomogeneous coordinates for each point
	// gives the following equations for each point:
	//
	// x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
	// y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
	//
	// as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
	// so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
	// after ordering the terms it gives the following matrix
	// that can be solved with gaussian elimination:
    
	float P[8][9]={
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
        
        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
        
        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
        
        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
	};
    
	gaussian_elimination(&P[0][0],9);
    
	// gaussian elimination gives the results of the equation system
	// in the last column of the original matrix.
	// opengl needs the transposed 4x4 matrix:
	float aux_H[]={ P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31
        P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32
        0      ,      0,0,0,       // 0    0   0 0
        P[2][8],P[5][8],0,1};      // h13  h23 0 h33
    
	for(int i=0;i<16;i++) homography[i] = aux_H[i];
}

ofMatrix4x4 xuQuadWarp::findHomography(ofPoint src[4], ofPoint dst[4]){
	ofMatrix4x4 matrix;
    
	// create the equation system to be solved
	//
	// from: Multiple View Geometry in Computer Vision 2ed
	//       Hartley R. and Zisserman A.
	//
	// x' = xH
	// where H is the homography: a 3 by 3 matrix
	// that transformed to inhomogeneous coordinates for each point
	// gives the following equations for each point:
	//
	// x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
	// y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
	//
	// as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
	// so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
	// after ordering the terms it gives the following matrix
	// that can be solved with gaussian elimination:
    
	float P[8][9]={
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
        
        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
        
        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
        
        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
	};
    
	gaussian_elimination(&P[0][0],9);
    
	matrix(0,0)=P[0][8];
	matrix(0,1)=P[1][8];
	matrix(0,2)=0;
	matrix(0,3)=P[2][8];
    
	matrix(1,0)=P[3][8];
	matrix(1,1)=P[4][8];
	matrix(1,2)=0;
	matrix(1,3)=P[5][8];
    
	matrix(2,0)=0;
	matrix(2,1)=0;
	matrix(2,2)=0;
	matrix(2,3)=0;
    
	matrix(3,0)=P[6][8];
	matrix(3,1)=P[7][8];
	matrix(3,2)=0;
	matrix(3,3)=1;
    
	return matrix;
}



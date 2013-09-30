//
//  xuQuadWarp.h
//  warpTest
//
//  Created by Rodrigo Torres González on 29/09/13.
//
//

#ifndef __warpTest__xuQuadWarp__
#define __warpTest__xuQuadWarp__


#include <iostream>
#include "ofMain.h"

class xuQuadWarp
{
public:
	xuQuadWarp(void);
	~xuQuadWarp(void);
	
	
	void	draw();
	void	mouseDown(int x, int y);
	void	mouseDrag(int x, int y);
	void	mouseUp();
	void    updateImage();
    void    gaussian_elimination(float *input, int n);
    void    findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]);
    ofMatrix4x4    findHomography(ofPoint src[4], ofPoint dst[4]);
    
	void setSuperficie(ofTexture &entrada);
	void setPuntos(ofRectangle dims);
    
    ofPoint getLeft();
    ofPoint getRight();
    ofPoint getTop();
    ofPoint getBottom();
    bool    isInBounds(ofPoint px);
    
    
	int masCercano(const ofPoint &punto, float tolerancia);
    
	int puntoActivo;
	float tol, radio;
    
    bool    modificar, overMe;
    
    
	ofPoint puntos[4], puntosDst[4];
	ofPoint offSet;
	bool seleccionado, pintarTh;
	//lets make a matrix for openGL
	//this will be the matrix that peforms the transformation
	GLfloat homografia[16];
    
	ofTexture inSuperficie, thSuperficie;
	ofTexture	outTextura;
    
};

#endif /* defined(__warpTest__xuQuadWarp__) */

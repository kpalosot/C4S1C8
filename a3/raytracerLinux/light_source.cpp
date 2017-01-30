/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

//define render types: SCENE_SIGNATURE, DIFFUSE or PHONG
RenderType LightSource::RENDER_TYPE = DIFFUSE;

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  

	//find normal vector
	Vector3D n_vec = ray.intersection.normal;
    n_vec.normalize();

    //find light vector
    Vector3D l_vec = _pos - ray.intersection.point;
    l_vec.normalize();

    //find the angle between the light source & the normal
    double lnangle = l_vec.dot(n_vec);

    //reflect vector
    Vector3D r_vec = (2 * lnangle * n_vec) - l_vec;
    r_vec.normalize();

    //view vector
    Vector3D view = -ray.dir;
    view.normalize();

    //Material & shading constructor
    Material *material = ray.intersection.mat;

    Colour color;

	//define light settings
    switch(LightSource::RENDER_TYPE){
    	case SCENE_SIGNATURE:

    		//scene signature = diffuse color setting 
    		color = material->diffuse;
    	break;

    	//diffuse ambient lighting
    	case DIFFUSE:
    		{	
    			//check if angle is negative or 0
	    		double res0 = 0;
	    		if(lnangle > res0){
	    			res0 = lnangle;
	    		}

	    		//diffuse color settings
	    		color = res0 * material->diffuse * _col_diffuse;
    		}
    	break;

    	case PHONG:
    		{
    			//find angle between view and reflection vectors
    			double vrangle = view.dot(r_vec);

    			//check if angles are negative or 0
    			double res1 = 0;
    			if(lnangle > res1){
    				res1 = lnangle;
    			}

    			double res2 = 0;
    			double temp = pow(vrangle, material->specular_exp);
    			if(temp > res2){
    				res2 = temp;
    			}

    			//phong color settings
    			color = res1 * material->diffuse * _col_diffuse +
                  res2 * material->specular * _col_specular;
            }
            break;
            default: 
            	throw "Invalid render type";
    }
   
    color.clamp();
    ray.col = ray.col + color;
    ray.col.clamp();
}


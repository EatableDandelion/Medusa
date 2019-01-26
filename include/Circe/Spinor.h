#pragma once

#include <ostream>

namespace Circe
{
	struct Complex;
	struct Quaternion;
	
	struct Complex
	{
		public:
			Complex(const float& angle);
			Complex(const float& real, const float& imaginary);
			
			float getAngle() const;
			Complex operator*(const Complex& q) const;
			Complex operator=(const Complex& q);
			float length() const;			
			void normalize();			
			void conjugate();
			void addAngle(const float& angle);//rotate the angle
			float getReal() const;
			float getImaginary() const;
			
		private:
			float c,s;
	};
	
	
	struct Quaternion
	{
		public:
			Quaternion();
			Quaternion(const float& w, const float& x, const float& y, const float& z);
			
			Quaternion operator=(const Quaternion& q);
			Quaternion operator*(const Quaternion& q)const;					
			float length() const;			
			void normalize();			
			void conjugate();
			void addAngle(const float& roll, const float& pitch, const float& yaw); //add the angles in the three dimensions, equivalent to adding dtheta, angles in the local reference frame
			Quaternion getConjugate() const;		
			float getW() const;			
			float getX() const;			
			float getY() const;			
			float getZ() const;
		
		private:
			float x,y,z,w;
	};
	
	
	std::ostream& operator<<(std::ostream &strm, const Complex &q);
	
	
	std::ostream& operator<<(std::ostream &strm, const Quaternion &q);
}
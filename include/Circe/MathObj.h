#pragma once
#include <assert.h>
#include <iostream>
#include <ostream>
#include <math.h>
#include <memory>
#include "Spinor.h"

namespace Circe
{	
	//Fwd declaration
	template<std::size_t N> struct Vec;
	template<> struct Vec<2>;
	template<> struct Vec<3>;
	template<> struct Vec<4>;
	template<std::size_t N, std::size_t M> struct Mat;
	struct Mat44;
	struct ITransform;
	template<std::size_t N> struct Transform;
	template<std::size_t N> struct Direction;
	template<std::size_t N> struct Position;
	
	
	//A few useful functions
	extern float min(const float& a, const float& b);
	extern float max(const float& a, const float& b);
	extern float operator ""_deg(long double angle);
	extern float operator ""_rad(long double angle);
	
	float cross(const Vec<2>& a, const Vec<2>& b);
	Vec<3> cross(const Vec<3>& a, const Vec<3>& b);
	
	//Scalar product
	template<std::size_t N>
	float dot(const Vec<N>& a, const Vec<N>& b)
	{
		return a.dot(b);
	}
	
	//Inequality operator
	template<std::size_t N>
	bool operator!=(const Vec<N>& a, const Vec<N>& b)
	{
		return !(a==b);
	}
	
	//Add two vectors
	template<std::size_t N>
	Vec<N> operator+(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)+b(i);
		}
		return res;
	}
				
	//Subtract two vectors
	template<std::size_t N>
	Vec<N> operator-(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)-b(i);
		}
		return res;
	}
	
	//Negative vector
	template<std::size_t N>
	Vec<N> operator-(const Vec<N>& a)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=-a(i);
		}
		return res;
	}
	
	//Multiply vector term by term
	template<std::size_t N>
	Vec<N> operator*(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)*b(i);
		}
		return res;
	}
	
	//Divide vector term by term
	template<std::size_t N>
	Vec<N> operator/(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)/b(i);
		}
		return res;
	}
	
	//Add float to a vector
	template<std::size_t N>
	Vec<N> operator+(const Vec<N>& a, const float& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)+b;
		}
		return res;
	}
	
	//Multiply float to this vector
	template<std::size_t N>
	Vec<N> operator*(const Vec<N>& a, const float& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)*b;
		}
		return res;
	}
	
	//Divide float to this vector
	template<std::size_t N>
	Vec<N> operator/(const Vec<N>& a, const float& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)/b;
		}
		return res;
	}
	
	//Get the norm of the vector
	template<std::size_t N>
	float length(const Vec<N>& a)
	{
		return sqrt(dot(a, a));
	}
	
	//Normalize the vector
	template<std::size_t N>
	void normalize(Vec<N>& v)
	{
		v=v/Circe::length(v);
	}
	
	//Get distance square
	template<std::size_t N>
	float distanceSquare(const Vec<N>& a, const Vec<N>& b)
	{
		return (a-b).dot(a-b);
	}
	
	//Get distance
	template<std::size_t N>
	float distance(const Vec<N>& a, const Vec<N>& b)
	{
		return sqrt(distanceSquare(a, b));
	}
	
	//Matrix multiplication, returns v*A
	template<std::size_t N, std::size_t M>
	Vec<M> operator*(const Vec<N>& a, const Mat<N, M>& m)
	{
		Vec<M> result;
		for(int j = 0; j<M; ++j)
		{
			float value = 0.0f;
			for(int i=0; i<N;++i)
			{
				value+=a(i)*m(i,j);
			}
			result(j)=value;
		}
		return result;
	}

	
	template<std::size_t N>
	struct Vec
	{
		public:
			Vec()
			{
				for(int i=0;i<N;++i)
				{
					data[i]=0.0f;
				}
			}
			
			Vec(const std::initializer_list<float>& values)
			{
				size_t i = 0;
				for(float f : values)
				{
					data[i]=f;
					i++;
				}
			}
			
			//Copy constructor
			Vec(const Vec<N>& v)
			{
				for(int i=0;i<N;++i)
				{
					data[i]=v(i);
				}
			}
			
			Vec(const float& f)
			{
				for(int i=0;i<N;++i)
				{
					data[i]=f;
				}
			}
			
			//Copy assignment
			Vec<N>& operator=(const Vec<N>& b)
			{
				for(int i=0;i<N;++i)
				{
					data[i]=b(i);
				}
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]+=b(i);
				}
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]-=b(i);
				}
			}

			//Multiply vector term by term
			void operator*=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]*=b(i);
				}
			}
			
			//Divide vector term by term
			void operator/=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]/=b(i);
				}
			}

			//Add float to this vector
			void operator+=(const float& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]+=b;
				}
			}
			
			//Subtract float to this vector
			void operator-=(const float& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]-=b;
				}
			}

			//Multiply float to this vector
			void operator*=(const float& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]*=b;
				}
			}
			
			//Divide float to this vector
			void operator/=(const float& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]/=b;
				}
			}
			
			//Equality operator
			bool operator==(const Vec<N>& b) const
			{
				for(int i=0;i<N;++i)
				{
					if(data[i]!=b(i))
					{
						return false;
					}
				}
				return true;
			}
	
			//Dot product
			float dot(const Vec<N>& b) const
			{
				float result(0.0f);
				for(int i=0; i<N;++i)
				{
					result+=data[i]*b(i);
				}
				return result;
			}
		
			//Get one element of this vector
			float operator()(const unsigned int& index) const
			{
				assert(index<N);
				return data[index];
			}
	
			//Set one element of this vector
			float &operator()(const unsigned int& index)
			{
				assert(index<N);
				return data[index];
			}
			
			void reset()
			{
				for(int i =0; i<N; ++i)
				{
					data[i]=0.0f;
				}
			}
			
		private:
			float data[N];
	};
	
	template<>
	struct Vec<2>
	{
		public:
			Vec():x(0.0f), y(0.0f)
			{}
		
			Vec(const float& x, const float& y):x(x), y(y)
			{}
			
			//Copy constructor
			Vec(const Vec<2>& v):x(v.x), y(v.y)
			{}
			
			Vec(const float& f)
			{
				x=f;
				y=f;
			}
			
			//Copy assignment
			Vec<2>& operator=(const Vec<2>& b)
			{
				x=b.x;
				y=b.y;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<2>& b)
			{
				x+=b.x;
				y+=b.y;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<2>& b)
			{
				x-=b.x;
				y-=b.y;
			}

			//Multiply vector term by term
			void operator*=(const Vec<2>& b)
			{
				x*=b.x;
				y*=b.y;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<2>& b)
			{
				x/=b.x;
				y/=b.y;
			}

			//Add float to this vector
			void operator+=(const float& b)
			{
				x+=b;
				y+=b;
			}
			
			//Subtract float to this vector
			void operator-=(const float& b)
			{
				x-=b;
				y-=b;
			}

			//Multiply float to this vector
			void operator*=(const float& b)
			{
				x*=b;
				y*=b;
			}
			
			//Divide float to this vector
			void operator/=(const float& b)
			{
				x/=b;
				y/=b;
			}
			
			//Equality operator
			bool operator==(const Vec<2>& b) const
			{
				return (x==b.x && y==b.y);
			}
	
			//Dot product
			float dot(const Vec<2>& b) const
			{
				return x*b.x + y*b.y;
			}
	
			//Get one element of this vector
			float operator()(const unsigned int& index) const
			{
				assert(index<2);
				if(index==0)return x;
				if(index==1)return y;
				return 0.0f;
			}
	
			//Set one element of this vector
			float &operator()(const unsigned int& index)
			{
				assert(index<2);
				if(index==0)return x;
				if(index==1)return y;
				return x;
			}
			
			void reset()
			{
				x=0.0f;
				y=0.0f;
			}
			
			void rotate(const Complex& q)
			{
				float c=q.getReal();
				float s=q.getImaginary();
				float xtemp=c*x-s*y;
				y=s*x+c*y;
				x=xtemp;
			}
			
			void rotateInv(const Complex& q)
			{
				float c=q.getReal();
				float s=q.getImaginary();
				float xtemp=c*x+s*y;
				y=-s*x+c*y;
				x=xtemp;
			}
			
		private:
			float x, y;
	};
	
	template<>
	struct Vec<3>
	{
		public:
			Vec():x(0.0f), y(0.0f), z(0.0f)
			{}
		
			Vec(const float& x, const float& y, const float& z):x(x), y(y), z(z)
			{}
			
			//Copy constructor
			Vec(const Vec<3>& v):x(v.x), y(v.y), z(v.z)
			{}
			
			Vec(const float& f)
			{
				x=f;
				y=f;
				z=f;
			}
			
			//Copy assignment
			Vec<3>& operator=(const Vec<3>& b)
			{
				x=b.x;
				y=b.y;
				z=b.z;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<3>& b)
			{
				x+=b.x;
				y+=b.y;
				z+=b.z;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<3>& b)
			{
				x-=b.x;
				y-=b.y;
				z-=b.z;
			}

			//Multiply vector term by term
			void operator*=(const Vec<3>& b)
			{
				x*=b.x;
				y*=b.y;
				z*=b.z;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<3>& b)
			{
				x/=b.x;
				y/=b.y;
				z/=b.z;
			}

			//Add float to this vector
			void operator+=(const float& b)
			{
				x+=b;
				y+=b;
				z+=b;
			}
			
			//Subtract float to this vector
			void operator-=(const float& b)
			{
				x-=b;
				y-=b;
				z-=b;
			}

			//Multiply float to this vector
			void operator*=(const float& b)
			{
				x*=b;
				y*=b;
				z*=b;
			}
			
			//Divide float to this vector
			void operator/=(const float& b)
			{
				x/=b;
				y/=b;
				z/=b;
			}
			
			//Equality operator
			bool operator==(const Vec<3>& b) const
			{
				return (x==b.x && y==b.y && z==b.z);
			}
	
			//Dot product
			float dot(const Vec<3>& b) const
			{
				return x*b.x + y*b.y + z*b.z;
			}
			
			//Get one element of this vector
			float operator()(const unsigned int& index) const
			{
				assert(index<3);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				return 0.0f;
			}
	
			//Set one element of this vector
			float &operator()(const unsigned int& index)
			{
				assert(index<3);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				return x;
			}
			
			void reset()
			{
				x=0.0f;
				y=0.0f;
				z=0.0f;
			}
			
			void rotate(const Quaternion& q)
			{
				Quaternion v(0.0f, x, y, z);
				Quaternion p=q;
				p.normalize();
				Quaternion pConj=p.getConjugate();
				Quaternion result = p*v*pConj;
				x=result.getX();
				y=result.getY();
				z=result.getZ();
			}
			
			void rotateInv(const Quaternion& p)
			{
				rotate(p.getConjugate());
			}
			
		private:
			float x, y, z, padding;
	};
	
	template<>
	struct Vec<4>
	{
		public:
			Vec():x(0.0f), y(0.0f), z(0.0f), w(0.0f)
			{}
		
			Vec(const float& x, const float& y, const float& z, const float& w):x(x), y(y), z(z), w(w)
			{}
			
			//Copy constructor
			Vec(const Vec<4>& v):x(v.x), y(v.y), z(v.z), w(v.w)
			{}
			
			Vec(const float& f)
			{
				x=f;
				y=f;
				z=f;
				w=f;
			}
			
			//Copy assignment
			Vec<4>& operator=(const Vec<4>& b)
			{
				x=b.x;
				y=b.y;
				z=b.z;
				w=b.w;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<4>& b)
			{
				x+=b.x;
				y+=b.y;
				z+=b.z;
				w+=b.w;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<4>& b)
			{
				x-=b.x;
				y-=b.y;
				z-=b.z;
				w-=b.w;
			}

			//Multiply vector term by term
			void operator*=(const Vec<4>& b)
			{
				x*=b.x;
				y*=b.y;
				z*=b.z;
				w*=b.w;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<4>& b)
			{
				x/=b.x;
				y/=b.y;
				z/=b.z;
				z/=b.w;
			}

			//Add float to this vector
			void operator+=(const float& b)
			{
				x+=b;
				y+=b;
				z+=b;
				w+=b;
			}
			
			//Subtract float to this vector
			void operator-=(const float& b)
			{
				x-=b;
				y-=b;
				z-=b;
				w-=b;
			}

			//Multiply float to this vector
			void operator*=(const float& b)
			{
				x*=b;
				y*=b;
				z*=b;
				w*=b;
			}
			
			//Divide float to this vector
			void operator/=(const float& b)
			{
				x/=b;
				y/=b;
				z/=b;
				z/=b;
			}
			
			//Equality operator
			bool operator==(const Vec<4>& b) const
			{
				return (x==b.x && y==b.y && z==b.z && w==b.w);
			}
	
			//Dot product
			float dot(const Vec<4>& b) const
			{
				return x*b.x + y*b.y + z*b.z + w*b.w;
			}
	
			//Get one element of this vector
			float operator()(const unsigned int& index) const
			{
				assert(index<4);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				if(index==3)return w;
				return 0.0f;
			}
	
			//Set one element of this vector
			float &operator()(const unsigned int& index)
			{
				assert(index<4);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				if(index==3)return w;
				return x;
			}
			
			void reset()
			{
				x=0.0f;
				y=0.0f;
				z=0.0f;
				w=0.0f;
			}
			
		private:
			float x, y, z, w;
	};
	
	using Vec2=Vec<2>;
	using Vec3=Vec<3>;
	using Vec4=Vec<4>;
	
	template<std::size_t N, std::size_t M=N>
	struct Mat
	{	
		public:
			Mat()
			{	
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i][j]=0.0f;
					}
				}	
			}
			
			Mat(const std::initializer_list<float>& values)
			{
				size_t i = 0;
				size_t j = 0;
				for(float f : values)
				{
					if(j==M)
					{
						i++;
						j=0;
					}
					data[i][j]=f;
					j++;
				}
			}
			
			
			Mat<N,M> &operator=(const Mat<N,M>& m)
			{
				for(int i=0;i<N;++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i][j]=m(i,j);
					}
				}
				return *this;
			}
			
			//Turn the matrix to I
			void setIdentity()
			{
				for(int i = 0; i<min(N, M); ++i)
				{
					data[i][i]=1.0f;
				}	
			}
			
			//Set all the coeffs to 0
			void reset()
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i][j]=0.0f;
					}
				}	
			}
			
			Mat<M,N> getTranspose() const
			{
				Mat<M,N> result;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						result(j,i)=data[i][j];
					}
				}	
				return result;
			}
			
			//Matrix multiplication to matrix
			template<std::size_t O>
			Mat<N,O> operator*(const Mat<M,O>& m) const
			{
				Mat<N,O> result;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<O; ++j)
					{
						float value = 0.0f;
						for(int k=0;k<M; ++k)
						{
							value+=data[i][k]*m(k,j);
						}
						result(i,j)=value;
					}
				}	
				return result;
			}
			
			//Matrix multiplication to vector, returns A*v
			Vec<N> operator*(const Vec<M>& v) const
			{
				Vec<N> result;
				for(int i=0; i<N;++i)
				{
					float value = 0.0f;
					for(int j = 0; j<M; ++j)
					{
						value+=v(j)*data[i][j];
					}
					result(i)=value;
				}
				return result;
			}
			
			//Get the matrix multiplied by a scalar
			Vec<M> operator*(const float f) const
			{
				Mat<N,M> result;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						result(i,j)=data[i][j]*f;
					}
				}	
				return result;	
			}
			
			void operator+=(const Mat<N,M>& m)
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i][j]+=m(i,j);
					}
				}	
			}
			
			void operator-=(const Mat<N,M>& m)
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i][j]-=m(i,j);
					}
				}	
			}
			
			//Multiply by a scalar
			void operator*=(const float& f)
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i][j]*=f;
					}
				}
			}
			
			//Get the coefficient at row i and column j
			float operator()(const int& i, const int& j) const
			{
				return data[i][j];
			}
			
			//Set the coefficient at row i and column j
			float &operator()(const int& i, const int& j)
			{
				return data[i][j];
			}
			
			float* getData()
			{
				static float res[N*M];
				int k=0;
				for(int i=0; i<N; i++)
				{
					for(int j=0; j<M; j++)
					{
						res[k]=data[i][j];
						k++;
					}
				}
				return res;
			}
			
		private:
			float data[N][M];
	};
	
	struct Mat44:public Mat<4>
	{
		public:
			static Mat44 positionMatrix(const Vec<2>& v)
			{
				return Mat44::positionMatrix(v(0), v(1), 0.0f);
			}
			
			static Mat44 positionMatrix(const Vec<3>& v)
			{
				return Mat44::positionMatrix(v(0), v(1), v(2));
			}
		
			static Mat44 positionMatrix(const float& x, const float& y, const float& z)
			{
				Mat44 m;
				m.setIdentity();
				m(0,3)=x;
				m(1,3)=y;
				m(2,3)=z;
				return m;
			}
			
			static Mat44 scaleMatrix(const Vec<2>& v)
			{
				return Mat44::scaleMatrix(v(0), v(1), 1.0f);
			}
			
			static Mat44 scaleMatrix(const Vec<3>& v)
			{
				return Mat44::scaleMatrix(v(0), v(1), v(2));
			}
			
			static Mat44 scaleMatrix(const float& x, const float& y, const float& z)
			{
				Mat44 m;
				m.setIdentity();
				m(0,0)=x;
				m(1,1)=y;
				m(2,2)=z;
				return m;
			}
			
			//Get the rotation matrix
			static Mat44 rotationMatrix(const Complex& q)
			{
				float c = q.getReal();
				float s = q.getImaginary();
				
				Mat44 m;
				
				m(0,0)=c;		m(0,1)=s;		m(0,2)=0.0f;	m(0,3)=0.0f;
				m(1,0)=-s;		m(1,1)=c;		m(1,2)=0.0f;	m(1,3)=0.0f;
				m(2,0)=0.0f;	m(2,1)=0.0f;	m(2,2)=1.0f;	m(2,3)=0.0f;
				m(3,0)=0.0f;	m(3,1)=0.0f;	m(3,2)=0.0f;	m(3,3)=1.0f;
				
				return m;
			}
			
			static Mat44 rotationMatrix(const Quaternion& q)
			{
				float x = q.getX();
				float y = q.getY();
				float z = q.getZ();
				float w = q.getW();
				
				Mat44 m;
				
				m(0,0)=1.0f-(2.0f*y*y+2.0f*z*z);	m(0,1)=2.0f*x*y+2.0f*z*w;			m(0,2)=2.0f*x*z-2.0f*y*w;			m(0,3)=0.0f;
				m(1,0)=2.0f*x*y-2.0f*z*w;			m(1,1)=1.0f-(2.0f*x*x+2.0f*z*z);	m(1,2)=2.0f*y*z+2.0f*x*w;			m(1,3)=0.0f;
				m(2,0)=2.0f*x*z+2.0f*y*w;			m(2,1)=2.0f*y*z-2.0f*x*w;			m(2,2)=1.0f-(2.0f*x*x+2.0f*y*y);	m(2,3)=0.0f;
				m(3,0)=0.0f;						m(3,1)=0.0f;						m(3,2)=0.0f;						m(3,3)=1.0f;
				
				return m;
			}
			
			
			static Mat44 identity()
			{
				Mat44 m;
				m.setIdentity();
				return m;
			}
			
			static Mat44 orthoProjection(const float& width, const float& height, const float& nearField, const float& farField)
			{
				float left=-width/2.0f;
				float right=width/2.0f;
				float bottom=-height/2.0f;
				float top=height/2.0f;
				Mat44 m;
				m(0,0)=2.0f/width;
				m(1,1)=2.0f/height;
				m(2,2)=2.0f/(farField-nearField);
				m(3,3)=1.0f;
				
				m(0,3)=-(right+left)/(right-left);
				m(1,3)=-(top+bottom)/(top-bottom);
				m(2,3)=-(farField+nearField)/(farField-nearField);
				
				return m;
			}
	};
	
	struct ITransform
	{
		virtual Mat<4> getTransformMatrix() const = 0;
	};
	
	template<std::size_t N>
	struct Transform : public ITransform
	{
		public:
			Transform():position(), rotation(), scale(1.0f)
			{}
			
			Transform(const Vec<N>& position, const Quaternion& rotation, const Vec<N>& scale):position(position), rotation(rotation), scale(scale)
			{}
			
			Transform(const Transform<N>& transform):position(transform.position), rotation(transform.rotation), scale(transform.scale)
			{}
			
			Transform<N> &operator=(const Transform<N>& transform)
			{
				position=transform.position;
				rotation=transform.rotation;
				scale=transform.scale;
				return *this;
			}
			
			void rotate(const float& roll, const float& pitch, const float& yaw)
			{
				rotation.addAngle(roll, pitch, yaw);
			}
			
			void translate(const Vec<N>& v)
			{
				position+=v;
			}
			
			void resize(const float& scaleRatio)
			{
				scale*=scaleRatio;
			}
			
			Mat<4> getTransformMatrix() const
			{
				return (Mat44::positionMatrix(position)) * (Mat44::rotationMatrix(rotation)) * (Mat44::scaleMatrix(scale));
			}
			
			Vec<N> getPosition() const
			{
				return position;
			}
			
			Quaternion getRotation() const
			{
				return rotation;
			}
			
			Vec<N> getScale() const
			{
				return scale;
			}
			
		private:
			Vec<N> position;
			Quaternion rotation;
			Vec<N> scale;			
	};
	
	//Wrapper for change of reference frame of a positional vector
	template<std::size_t N>
	struct Position
	{
		public:
			Position(Vec<N>& vectorToTransform):v(&vectorToTransform)
			{}
			
			void toGlobalFrame(const Transform<N>& transform)
			{
				(*v).rotateInv(transform.getRotation());
				(*v)-=transform.getPosition();
			}
			
			void toLocalFrame(const Transform<N>& transform)
			{
				(*v).rotate(transform.getRotation());
				(*v)+=transform.getPosition();
			}
			
		private:
			std::shared_ptr<Vec<N>> v;
	};
	
	
	//Wrapper for change of reference frame of a directional vector
	template<std::size_t N>
	struct Direction
	{
		public:
			Direction(Vec<N>& vectorToTransform):v(&vectorToTransform)
			{}
			
			void toGlobalFrame(const Transform<N>& transform)
			{
				(*v).rotateInv(transform.getRotation());
			}
			
			void toLocalFrame(const Transform<N>& transform)
			{
				(*v).rotate(transform.getRotation());
			}
			
		private:
			std::shared_ptr<Vec<N>> v;
	};
	
	
	template<std::size_t N>
	std::ostream& operator<<(std::ostream &strm, const Vec<N> &v)
	{
		strm << "[" << v(0);
		for(int i=1; i<N;++i){
			strm << ", " << v(i);
		}
		return strm << "]";
	}
	
	template<std::size_t N, std::size_t M>
	std::ostream& operator<<(std::ostream &strm, const Mat<N, M> &m)
	{
		std::cout.precision(3);
		for(int i=0; i<N;++i){
			strm << "|" << m(i,0);
			for(int j=1; j<M; ++j)
			{
				strm << ",\t" << m(i,j);
			}
			strm << "|" << std::endl;
		}
		return strm;
	}
	
}
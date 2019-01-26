#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <stdarg.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <map>
#include <sstream>
#include <fstream>
#include "Utils.h"


#define CIRCE_INFO(text) {Circe::log.print("INFO", __FILE__, __LINE__, text);}
#define CIRCE_WARNING(text) {Circe::log.print("WARNING", __FILE__, __LINE__, text);}
#define CIRCE_ERROR(text) {Circe::log.print("ERROR", __FILE__, __LINE__, text);}
#define CIRCE_PRINT(text) {Circe::log.print(text);}


#define CIRCE_MARK {Circe::Debug::mark(__FILE__, __LINE__);}
#define CIRCE_BREAK {Circe::Debug::breakPoint(__FILE__, __LINE__);}


#define CIRCE_CONCAT(x, y) CIRCE_CONCAT2(x, y)
#define CIRCE_CONCAT2(x, y) x ## y
#define CIRCE_PROFILEBLOCK Circe::ProfileMarker CIRCE_CONCAT(marker_,__LINE)(__FILE__, __PRETTY_FUNCTION__, __LINE__)
#define CIRCE_INITPROFILER {std::shared_ptr<Profiler> manager = std::make_shared<Profiler>(); ProfilerService::provide(manager);}

/**
How to use profiler:

{
	CIRCE_INITPROFILER;
	CIRCE_PROFILEBLOCK;
	...
}
**/

namespace Circe
{
	using namespace std::chrono;
	class Debug;
	struct Output;
	class OutputPlatform;
	class ProfileMarker;
	class Profile;
	class Profiler;
	class ProfilerService;
	
	
	class Debug
	{
		public:
			static void mark(const char* file, const int line){
				std::string fileName(file);

				
				auto dt = duration_cast<microseconds>(high_resolution_clock::now() - timeStamp);
				timeStamp = high_resolution_clock::now();
				
				std::string text = "[MARK "+std::to_string(markIndex)+": "+fileName+" - line "+std::to_string(line)+"]\t\t"+"Time since last mark = "+std::to_string(dt.count()/1000.0)+" ms.";
				std::cout << text << std::endl;
				markIndex++;
			}
			
			static void breakPoint(const char* file, const int line){
				mark(file, line);
				std::cout << "Press any key to continue..." << std::endl;
				getchar();
			}
			
		private:
			static unsigned int markIndex;
			static high_resolution_clock::time_point timeStamp;
	};
	
	
	class Log
	{
		public:
			void print(const std::string& type, const char* file, const int line, const std::string text) const;
			
			template<typename T>
			void print(T& t)
			{
				std::cout << t;
			}
			
			template<std::size_t N, std::size_t M>
			void printArray(const char charArray[N][M]) const
			{
				for(int j = 0;j < M; j++){
					for(int i = 0;i < N; i++){
						std::cout << charArray[i][j];
					}
					std::cout << std::endl;
				}
			}
	};
	
	static Log log = Log();

	/**
	ProfilerMarker is a trigger to start and finish a profile stat using RAII.
	**/
	class ProfileMarker
	{
		public:
			ProfileMarker(const char* file, const char* functionName, const int line);
			
			~ProfileMarker();
			
		private:
			std::string name;
	};
	
	/**
	Profile is a wrapper around the time of execution of the branch.
	**/
	class Profile
	{
		public:
			Profile(const std::string& fileName, const std::string& functionName, const int line, std::shared_ptr<Profile> parent = NULL);
			
			void printHead(const double& scale);

			template<std::size_t N, std::size_t M>
			void print(int& x, int& y, double scale, double totalTime, char data[N][M])
			{
				int x0(x);
				int length = scale*m_time/totalTime;
				
				for(int i = 0; i<length; i++)
				{
					data[x][y] = '='; 
					x++;
				}
				
				for(const auto& entry : m_children)
				{
					std::shared_ptr<Profile> profile = entry.second;
					y++;
					profile->print<N,M>(x0, y, scale, totalTime, data);
				}
			}
			
			void printTreeHead();
			
			void printTree(const int depth, const double& totalTime);
		
		private:
			/** Members for the tree and organization **/
			std::shared_ptr<Profile> m_parent;
			std::map<std::size_t, std::shared_ptr<Profile>> m_children;
			friend class Profiler;
			
			/** Real meat **/
			std::string m_fileName;
			std::string m_functionName;
			unsigned int m_line;
			double m_time;
			unsigned int m_count;
			high_resolution_clock::time_point startTime;
			
	};
	

	/** 
	Profiler manages the profiles
	**/
	
	class Profiler
	{
		public:
			Profiler();
			~Profiler();
		
			void push(const char* file, const char* functionName, const int line);
	
			void pull();
			
		
		private:
			std::shared_ptr<Profile> m_head;
	};
	
	
	
	
	class ProfilerService{
		public:		
			static std::shared_ptr<Profiler> get()
			{
				return m_profiler;
			}
			
			static void provide(std::shared_ptr<Profiler> profiler)
			{
				m_profiler=profiler;
			}
		
		private:
			static std::shared_ptr<Profiler> m_profiler;
	};
}

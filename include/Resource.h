#pragma once
#include <Circe/Circe.h>
#include <map>
#include <string>
#include <memory>
#include <utility>
#include <iostream>

namespace Medusa
{
	
	using namespace std;
	
	template<typename R> class ResourceLoader;
	template<typename ResHandle, typename R, typename RLoader> class ResourceManager;	
	template<typename T, typename R> class ResourceHandle;

	
	template <typename R>
	class ResourceLoader
	{
		public:
			virtual ~ResourceLoader(){};
			//virtual void load(const string& folderLocation, const string& fileName, R& resource){};
			virtual void unload(R& resource) = 0;
	};
	
	template<typename ResHandle, typename R, typename RLoader>// = ResourceLoader<R>
	class ResourceManager
	{
	public:	
		template<typename... RLArgs> 
		ResourceManager(const string& folderLocation, RLArgs&&... args):folderLocation(folderLocation), resourceLoader(std::make_shared<RLoader>(std::forward<RLArgs>(args)...))
		{}
		
		~ResourceManager()
		{
			unloadAll();
		}
		
		template<typename... ResourceArgs>
		void load(const string& name, ResourceArgs&&... args)
		{
			shared_ptr<R> resourcePtr = make_shared<R>(std::forward<ResourceArgs>(args)...);
			resourceLoader->load(folderLocation, name, *resourcePtr);
			resources.insert(pair<size_t, shared_ptr<R>>(Circe::getId(name), resourcePtr));
		}
		
		void unloadResource(const string& name)
		{
			unloadResource(Circe::getId(name));			
		}		
		
		void unloadAll()
		{
			for(pair<size_t, shared_ptr<R>> element : resources)
			{
				unloadResource(element.first);
			}
			resources.clear();
		}
		
		template<typename... ResourceArgs> 
		ResHandle getResource(const string& name, ResourceArgs&&... args)
		{
			if(resources.find(Circe::getId(name)) == resources.end())
			{
				CIRCE_ERROR("Resource " +name+" has not been loaded.");
			}
			return ResHandle(*resources[Circe::getId(name)], std::forward<ResourceArgs>(args)...);
		}
	
	private:
		std::shared_ptr<RLoader> resourceLoader;
		map<size_t, std::shared_ptr<R>> resources; //The key is the ID of the resource
		string folderLocation;
		
		void unloadResource(const size_t& key)
		{
			resourceLoader->unload(*(resources[key]));
			resources[key].reset();
			resources.erase(key);
		}

	};
	
	template<typename T, typename R>
	class ResourceHandle
	{
		public:
			static unsigned int ref;
			
			ResourceHandle()
			{}
			
			ResourceHandle(R& resource):m_resource(std::make_shared<R>(resource))
			{
				++ref;
			}
			
			~ResourceHandle()
			{
				--ref;
			}
			
			std::shared_ptr<R> operator->()
			{
				return m_resource;
			}
			
			std::shared_ptr<R> operator->() const
			{
				return m_resource;
			}
			
		protected:
			std::shared_ptr<R> m_resource;
	};
	
	template<typename T, typename R> unsigned int ResourceHandle<T, R>::ref(0);
	
}

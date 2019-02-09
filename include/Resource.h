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
	
	template<class RHandle, class R, class RLoader> class ResourceManager;
	template<class R> class ResourceLoader;
	template<class T, class R> class ResourceHandle;

	
	template<class RHandle, class R, class RLoader>
	class ResourceManager
	{
	public:	
		template<typename... RLArgs> 
		ResourceManager(const string& folderLocation, RLArgs&&... args):folderLocation(folderLocation), resourceLoader(std::forward<RLArgs>(args)...)
		{}
		
		~ResourceManager()
		{
			unloadAll();
		}
		
		
		void loadResource(const string& name)
		{
			shared_ptr<R> resourcePtr = make_shared<R>();
			resourceLoader.load(folderLocation, name, *resourcePtr);
			resources.insert(pair<size_t, shared_ptr<R>>(Circe::getId(name), resourcePtr));
		}
		
		void unloadResource(const size_t& key)
		{
			resourceLoader.unload(*resources[key]);
			resources[key].reset();
			resources.erase(key);
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
		RHandle getResource(const string& name, ResourceArgs&&... args)
		{
			size_t id = Circe::getId(name);
			if(resources.find(id)==resources.end())
			{
				loadResource(name);
			}
			return RHandle(*resources[id], std::forward<ResourceArgs>(args)...);
		}
	
	private:
		RLoader resourceLoader;
		map<size_t, std::shared_ptr<R>> resources; //The key is the ID of the resource
		string folderLocation;

	};
	
	template <class R>
	class ResourceLoader
	{
		public:
			virtual ~ResourceLoader(){};
			virtual void load(const string& folderLocation, const string& fileName, R& resource)=0;
			virtual void unload(R& resource)=0;
	};
	
	template<class T, class R>
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
			
		protected:
			std::shared_ptr<R> m_resource;
	};
	
	template<typename T, typename R> unsigned int ResourceHandle<T, R>::ref(0);
	

	/*
	class ResourceData
	{
		public:
			ResourceData(const std::string& folderLocation, const std::string& fileName);
		
			~ResourceData();
			
			void read(const int& location) const;
		
		private:
			unsigned int m_id;
			std::string m_fileName;
	};
	
	
	class ResourceHandle
	{
		public:
			void read(const int& location) const;
			
		private:
			std::vector<std::size_t> m_ids;
			std::shared_ptr<IResourceManager> m_manager;
			ResourceHandle(IResourceManager* manager);
			friend class IResourceManager;
	};
	
	class IResourceManager
	{
		public:
			virtual void read(const std::size_t id) = 0;
	};
	
	template<typename Handle>
	class ResourceManager : public IResourceManager
	{
		public:
			ResourceManager(const std::string& folderLocation);
			
			~ResourceManager();
			
			Handle getResource(const std::string& fileName)
			{
				Handle newResourceHandle(this);
				
				size_t id = Circe::getId(fileName);
				if(m_resources.find(id)==m_resources.end())
				{
					m_resources[id] = std::make_unique<ResourceData>(m_folderLocation, fileName);
					newResourceHandle.setId(id);
				}
				
				return newResourceHandle;
			}
			
			virtual void read(const std::size_t id);
			
		private:
			std::map<std::size_t, std::unique_ptr<ResourceData>> m_resources;
			std::string m_folderLocation;

	};*/
	
}

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
	
	
	
	
	
	class RData
	{
		public:
			RData(const std::string& folderLocation, const std::string& fileName)
			{}
			
			virtual ~RData() = 0;
	};
	
	template<typename Handle, typename Data>
	class RManager
	{
		public:
			RManager(const std::string& folderLocation):m_folderLocation(folderLocation)
			{}
		
			Handle getHandle(const std::string& fileName)
			{
				int id(Circe::getId(fileName));
				if(m_resources.find(id) == m_resources.end())
				{
					m_resources.emplace(id, std::make_shared<Data>(m_folderLocation, fileName));
				}
				return Handle(this, id);
			}
			
			Data* get(const int& id)
			{
				return m_resources[id].get();
			}
			
		private:
			map<int, std::shared_ptr<Data>> m_resources;
			std::string m_folderLocation;
	};
	
	template<typename T, typename Data>
	class RHandle
	{
		public:
			Data* operator->()
			{
				return m_manager.get(m_id);
			}
			
		private:
			int m_id;
			std::shared_ptr<RManager<T, Data>> m_manager;
			friend class RManager<T, Data>;
		
			RHandle(const RManager<T, Data>* manager, const int& id):m_manager(manager), m_id(id)
			{}
	};
	
}

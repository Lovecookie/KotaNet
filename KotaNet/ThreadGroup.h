#pragma once

namespace Kota
{
	class ThreadGroup final
	{
	public:
		using FuncInside = std::function<void()>;

	public:	
		ThreadGroup() = default;
		~ThreadGroup() = default;

		template<class Func, class ...Args>
		void Associate( const Func&& func, Args&&... args)
		{
			_threads.emplace_back( std::thread( func, args...) );
		}

		void FuncAll( FuncInside&& func );
		void JoinAll();

	private:
		std::vector<std::thread> _threads;
	};
}

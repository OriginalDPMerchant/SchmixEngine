#pragma once
#include <SchmixEngine/Core/Debugging/Logger.h>
#include <cassert>

#ifdef NDEBUG
	#define SMX_LOG(msg, level) do { SchmixEngine::Logger::GlobalLog(msg, level); } while(0)

	#define SMX_LOG_WSRC(msg, src, level) do { SchmixEngine::Logger::GlobalLog(msg, level, src, true); } while(0)
	
	#define SMX_ASSERT(x) do { if( !(x) ) { SMX_LOG("Assersion failed: "#x); abort(); } } while(0)

	#define SMX_DBG_LOG(msg, level) ((void)0)

	#define SMX_DBG_LOG_WSRC(msg, src, level) ((void)0)

	#define SMX_DBG_ASSERT(x) ((void)0)
#else
	#define SMX_LOG(msg, level) SMX_DBG_LOG(msg, level)

	#define SMX_LOG_WSRC(msg, src, level)  SMX_DBG_LOG_WSRC(msg, src, level)

	#define SMX_ASSERT(x) SMX_DBG_ASSERT(x)

	#define SMX_DBG_LOG(msg, level) do { SchmixEngine::Logger::GlobalLog(msg, level); } while(0)

	#define SMX_DBG_LOG_WSRC(msg, src, level) do { SchmixEngine::Logger::GlobalLog(msg, level, src, true); } while(0)

	#define SMX_DBG_ASSERT(x) do { assert(x); } while(0)
#endif
#pragma once
#ifndef __NATIVA_COLLECTIONS_COMMON
#define __NATIVA_COLLECTIONS_COMMON

#define __NATIVA_ENSURE_TYPE_SAFETY(ClassTemplateType, FunctionTemplateType) static_assert(std::is_same<ClassTemplateType, FunctionTemplateType>::value, "The type of the element enqueued should be the same as the type given when the template class was instantiated.")

namespace nativa
{
	namespace collections
	{
		using index_type = size_t;
    }
}

#endif
#pragma once
#ifndef __NATIVA_COLLECTIONS_COMMON
#define __NATIVA_COLLECTIONS_COMMON

#define __NATIVA_ENSURE_TYPE_SAFETY(ClassTemplateType, FunctionTemplateType) static_assert(std::is_convertible<FunctionTemplateType, ClassTemplateType>::value, "The type of the argument should be convertible to the type given when the template class was instantiated.")

namespace nativa
{
	namespace collections
	{
		using index_type = size_t;
    }
}

#endif
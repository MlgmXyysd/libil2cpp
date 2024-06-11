#pragma once

#include "../external/google/sparsehash/dense_hash_set.h"
#include "KeyWrapper.h"

template <class Value,
		  class HashFcn,
		  class EqualKey = std::equal_to<KeyWrapper<Value> >,
		  class Alloc = std::allocator<KeyWrapper<Value> > >
class Il2CppHashSet : public dense_hash_set<KeyWrapper<Value>, HashFcn, EqualKey, Alloc>
{
private:
	typedef dense_hash_set<KeyWrapper<Value>, HashFcn, EqualKey, Alloc> Base;

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::hasher hasher;
	typedef typename Base::key_equal key_equal;
	typedef typename Base::key_type key_type;

	explicit Il2CppHashSet(size_type n = 0,
		const hasher& hf = hasher(),
		const key_equal& eql = key_equal()) : 
		Base(n, hf, eql)
	{ 
		Base::set_empty_key(key_type(key_type::KeyType_Empty));
		Base::set_deleted_key(key_type(key_type::KeyType_Deleted));
	}

	template <class InputIterator>
	Il2CppHashSet(InputIterator f, InputIterator l,
		size_type n = 0,
		const hasher& hf = hasher(),
		const key_equal& eql = key_equal()) :
		Base(f, l, n, hf, eql)
	{
		Base::set_empty_key(key_type(key_type::KeyType_Empty));
		Base::set_deleted_key(key_type(key_type::KeyType_Deleted));
	}
};


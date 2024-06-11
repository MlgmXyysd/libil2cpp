#pragma once

template<class T>
struct KeyWrapper
{
	typedef T wrapped_type;
	typedef KeyWrapper<T> self_type;

	enum KeyTypeEnum { KeyType_Normal, KeyType_Empty, KeyType_Deleted };
	
	KeyTypeEnum type;
	T key;

	KeyWrapper() : type(KeyType_Normal) {}
	KeyWrapper(KeyTypeEnum type_) : type(type_) {}
	KeyWrapper(const T& key_) : key(key_), type(KeyType_Normal) {}
	KeyWrapper(const self_type& other) : type(other.type), key(other.key) {}
	
	bool operator==(const self_type& other) const
	{
		if (type != other.type)
			return false;
		else if (type != KeyType_Normal)
			return true;
		else
			return (key == other.key);
	}

	bool operator!=(const self_type& other) const
	{
		if (type != other.type)
			return true;
		else if (type != KeyType_Normal)
			return false;
		else
			return (key != other.key);
	}

	operator const T& () const { return key; }
	bool isNormal() const { return (type == KeyType_Normal); }
};
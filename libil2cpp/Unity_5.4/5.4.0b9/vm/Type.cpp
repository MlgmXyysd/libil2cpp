#include "il2cpp-config.h"

#include <string>
#include <cassert>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <ctype.h>

#include "utils/StringUtils.h"
#include "vm/Assembly.h"
#include "vm/AssemblyName.h"
#include "vm/Class.h"
#include "vm/GenericClass.h"
#include "vm/GenericContainer.h"
#include "vm/MetadataCache.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "class-internals.h"
#include "object-internals.h"
#include "tabledefs.h"

static char* copy_name(const char* name)
{
	const size_t len = strlen (name);
	char* cname = new char[len + 1];

	strncpy (cname, name, len);

	cname[len] = '\0';

	return cname;
}

namespace il2cpp
{
namespace vm
{

TypeNameParser::TypeNameParser (std::string &name, TypeNameParseInfo &info, bool is_nested) :
	_info (info),
	_is_nested (is_nested),
	_accept_assembly_name (true),
	_p (name.begin ()),
	_end (name.end ())
{

}

TypeNameParser::TypeNameParser (std::string::const_iterator &begin, std::string::const_iterator &end, TypeNameParseInfo &info, bool is_nested) :
	_info (info),
	_is_nested (is_nested),
	_accept_assembly_name (true),
	_p (begin),
	_end (end)
{

}

bool TypeNameParser::Parse(bool acceptAssemblyName)
{
	_accept_assembly_name = acceptAssemblyName;

	int32_t arity = 0;

	InitializeParser ();

	if (IsEOL ()) return false;
	if (!ParseTypeName (arity)) return false;
	if (!ParseNestedTypeOptional (arity)) return false;
	if (!ParseTypeArgumentsOptional (arity)) return false;
	if (!ParsePointerModifiersOptional ()) return false;
	if (!ParseArrayModifierOptional ()) return false;
	if (!ParseByRefModifiersOptional ()) return false;
	if (!ParseAssemblyNameOptional ()) return false;

	return (_p == _end) || _is_nested;
}

bool TypeNameParser::ParseAssembly ()
{
	InitializeParser ();

	if (IsEOL ()) return false;
	if (!ParseAssemblyName ()) return false;

	return true;
}

bool TypeNameParser::ParseTypeName (int32_t &arity)
{
	std::string::const_iterator begin = _p;
	std::string::const_iterator last_dot = _end;

	while (true)
	{
		ConsumeIdentifier ();

		if (!CurrentIs ('.'))
			break;

		last_dot = _p;

		if (!Next ())
			return false; // Invalid format
	}

	if (CurrentIs ('`'))
	{
		if (!Next ())
			return false; // Invalid format

		if (!ConsumeNumber (arity))
			return false; // Invalid format
	}

	if(last_dot == _end)
	{
		_info._name.assign (begin, _p);
	} else
	{
		_info._namespace.assign (begin, last_dot);
		_info._name.assign (last_dot + 1, _p);
	}

	return true;
}

bool TypeNameParser::ParseNestedTypeOptional (int32_t &arity)
{
	while (CurrentIs ('+'))
	{
		if (!Next ())
			return false; // Invalid format

		std::string::const_iterator begin = _p;

		ConsumeIdentifier ();

		if (CurrentIs ('`'))
		{
			if (!Next ())
				return false; // Invalid format

			int32_t nested_arity = 0;

			if (!ConsumeNumber (nested_arity))
				return false; // Invalid format

			arity += nested_arity;
		}

		_info._nested.push_back (std::string(begin, _p));
	}

	return true;
}

bool TypeNameParser::ParsePointerModifiersOptional ()
{
	if (IsEOL ())
		return true;

	while (CurrentIs ('*'))
	{
		_info._modifiers.push_back (-1);
		if (!Next (true))
			break;
	}

	return true;
}

bool TypeNameParser::ParseByRefModifiersOptional ()
{
	if (IsEOL ())
		return true;

	if (CurrentIs ('&'))
	{
		if (std::find (_info._modifiers.begin (), _info._modifiers.end (), 0) != _info._modifiers.end ())
			return false; // Invalid format, already marked as reference

		_info._modifiers.push_back (0);

		Next (true);
	}

	return true;
}

bool TypeNameParser::ParseTypeArgumentsOptional (int32_t &arity)
{
	SkipWhites ();
	
	if (IsEOL ())
		return true;
	
	if (!CurrentIs ('['))
		return true;
	
	if (NextWillBe (']', true) || NextWillBe (',', true) || NextWillBe ('*', true))
		return true;

	if (!Next (true))
		return false; // Invalid format

	_info._type_arguments.reserve (arity);

	while (true)
	{
		bool acceptAssemblyName = false;

		if (CurrentIs('['))
		{
			acceptAssemblyName = true;

			if (!Next (true))
				return false; // Invalid format
		}

		TypeNameParseInfo info;
		TypeNameParser parser (_p, _end, info, true);

		if (!parser.Parse (acceptAssemblyName))
			return false; // Invalid format

		_p = parser._p;

		_info._type_arguments.push_back (info);

		SkipWhites ();
	
		if (IsEOL ())
			return false; // Invalid format

		if (acceptAssemblyName)
		{
			if (!CurrentIs (']'))
				return false; // Invalid format

			if (!Next (true))
				return false; // Invalid format
		}

		if (CurrentIs (']'))
			break;

		if (CurrentIs (','))
		{
			if (!Next (true))
				return false; // Invalid format
		} else
		{
			return false; // Invalid format
		}
	}

	if (_info._type_arguments.size () != arity)
		return false; // Invalid format

	Next (true);

	return true;
}

bool TypeNameParser::ParseArrayModifierOptional ()
{
	SkipWhites ();
	
	if (IsEOL ())
		return true;
	
	if (!CurrentIs ('['))
		return true;
	
	if (!NextWillBe (']', true) && !NextWillBe (',', true) && !NextWillBe ('*', true))
		return true;

	if (!Next (true))
		return false; // Invalid format

	int32_t rank = 1;

	while (true)
	{
		if (CurrentIs (']'))
		{
			Next (true);
			break;
		} else if (CurrentIs (','))
		{
			++rank;
			if (!Next (true))
				return false; // invalid format
		} else if (CurrentIs ('*'))
		{
			_info._modifiers.push_back (-2);

			if (!Next (true))
				return false; // invalid format
		} else
		{
			return false; // invalid format
		}
	}

	_info._modifiers.push_back (rank);

	// This is to support arrays of array
	return ParseArrayModifierOptional ();
}

bool TypeNameParser::ParseAssemblyNameOptional ()
{
	if (!_accept_assembly_name)
		return true;

	if (!CurrentIs (','))
		return true;

	if (!Next ())
		return false; // Invalid format

	SkipWhites ();

	return ParseAssemblyName ();
}

bool TypeNameParser::ParseAssemblyName ()
{
	std::string::const_iterator begin = _p;

	ConsumeAssemblyIdentifier ();

	_info._assembly_name.name.assign (begin, _p);

	SkipWhites ();

	ParsePropertiesOptional ();

	return true;
}

bool TypeNameParser::ParsePropertiesOptional ()
{
	while (CurrentIs (','))
	{
		if (!Next (true))
			return false; // Invalid format

		std::string::const_iterator begin = _p;
		ConsumePropertyIdentifier ();
		std::string propertyName (begin, _p);

		if (!CurrentIs ('='))
			return false;

		if (!Next ())
			return false; // Invalid format
	
		begin = _p;
		ConsumePropertyValue ();
		std::string propertyValue (begin, _p);

		if (propertyName == "Version")
		{
			if (!ParseVersion (propertyValue, _info._assembly_name.major, _info._assembly_name.minor, _info._assembly_name.build, _info._assembly_name.revision))
				return false;
		}
		else if (propertyName == "PublicKey")
		{
			if (propertyValue != "null")
				_info._assembly_name.public_key = propertyValue;
		}
		else if (propertyName == "PublicKeyToken")
		{
			if (propertyValue != "null")
			{
				if ((kPublicKeyTokenLength - 1) != propertyValue.size ())
					return false;
				strncpy(_info._assembly_name.public_key_token, propertyValue.c_str (), kPublicKeyTokenLength);
			}
		}
		else if (propertyName == "Culture")
		{
			_info._assembly_name.culture = propertyValue;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool TypeNameParser::ParseVersion (const std::string& version, uint16_t& major, uint16_t& minor, uint16_t& build, uint16_t& revision)
{
	size_t start = 0;
	size_t index = version.find ('.');
	if (-1 == index)
		return false;
	std::string component = version.substr (start, index - start);
	major = atoi (component.c_str ());
	
	start = index + 1;
	index = version.find ('.', start);
	if (-1 == index)
		return false;
	component = version.substr (start, index - start);
	minor = atoi (component.c_str ());
	
	start = index + 1;
	index = version.find ('.', start);
	if (-1 == index)
		return false;
	component = version.substr (start, index - start);
	build = atoi (component.c_str ());
	
	start = index + 1;
	component = version.substr (start, version.size () - start);
	revision = atoi (component.c_str ());

	return true;
}

bool TypeNameParser::NextWillBe (char v, bool skipWhites) const
{
	if (IsEOL ())
		return false;
	
	int32_t i = 1;
	if (skipWhites)
	{
		if((*(_p + i) == ' ') || (*(_p + i) == '\t'))
			++i;

		if ((_p + i) >= _end)
			return false;
	}
	
	return *(_p + i) == v;
}

bool TypeNameParser::ConsumeNumber (int32_t &value)
{
	if (!isdigit (*_p))
		return false;

	std::string::const_iterator begin = _p;

	while (isdigit (*_p))
	{
		if (!Next ())
			break;
	}

	value = (int32_t)strtol (&(*begin), NULL, 10);

	return true;
}

void TypeNameParser::ConsumeAssemblyIdentifier ()
{
	do
	{
		switch (*_p)
		{
			case ',':
			case '+':
			case '&':
			case '*':
			case '[':
			case ']':
			case '=':
			case '"':
			case '`':
				return;

			case '\\':
				Next ();
				break;
		}

	} while (Next ());
}

void TypeNameParser::ConsumePropertyIdentifier ()
{
	do
	{
		switch (*_p)
		{
			case '=':
				return;
		}

	} while (Next ());
}

void TypeNameParser::ConsumePropertyValue ()
{
	do
	{
		switch (*_p)
		{
			case ',':
			case ']':
				return;
		}

	} while (Next ());
}

void TypeNameParser::ConsumeIdentifier ()
{
	do
	{
		switch (*_p)
		{
			case ',':
			case '+':
			case '&':
			case '*':
			case '[':
			case ']':
			case '.':
			case '=':
			case '"':
			case '`':
				return;

			case '\\':
				Next ();
				break;
		}

	} while (Next ());
}

void TypeNameParser::InitializeParser ()
{
	SkipWhites ();
}

void TypeNameParser::SkipWhites ()
{
	while ((CurrentIs (' ') || CurrentIs ('\t')) && !IsEOL ())
		++_p;
}

TypeNameParseInfo::TypeNameParseInfo ()
{

}

TypeNameParseInfo::~TypeNameParseInfo ()
{
	_type_arguments.clear ();
	_modifiers.clear ();
	_nested.clear ();
}

int Type::GetType (const Il2CppType *type)
{
	return type->type;
}

void Type::GetNameInternal (std::ostringstream &oss, const Il2CppType *type, Il2CppTypeNameFormat format, bool is_nested)
{
	switch (type->type)
	{
		case IL2CPP_TYPE_ARRAY:
			{
			TypeInfo* arrayClass = Class::FromIl2CppType (type);
			TypeInfo* elementClass = Class::GetElementClass (arrayClass);
				Type::GetNameInternal (
					oss,
					elementClass->byval_arg,
					format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED ? IL2CPP_TYPE_NAME_FORMAT_FULL_NAME : format,
					false);
			
				oss << '[';

				if (arrayClass->rank == 1)
					oss << '*';
				
				for (int32_t i = 1; i < arrayClass->rank; i++)
					oss << ',';

				oss << ']';

				if (type->byref)
					oss << '&';

				if (format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED)
				{
					const Il2CppAssembly *ta = MetadataCache::GetAssemblyFromIndex (elementClass->image->assemblyIndex);
					oss << ", " << vm::AssemblyName::AssemblyNameToString(ta->aname);
				}

				break;
			}

		case IL2CPP_TYPE_SZARRAY:
			{
				TypeInfo* elementClass = Class::FromIl2CppType (type->data.type);
				Type::GetNameInternal (
					oss,
					elementClass->byval_arg,
					format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED ? IL2CPP_TYPE_NAME_FORMAT_FULL_NAME : format,
					false);

				oss << "[]";
		
				if (type->byref)
					oss << '&';

				if (format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED)
				{
					const Il2CppAssembly *ta = MetadataCache::GetAssemblyFromIndex (elementClass->image->assemblyIndex);
					oss << ", " << vm::AssemblyName::AssemblyNameToString(ta->aname);
				}
				break;
			}

		case IL2CPP_TYPE_PTR:
			{
				Type::GetNameInternal (
					oss,
					type->data.type,
					format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED ? IL2CPP_TYPE_NAME_FORMAT_FULL_NAME : format,
					false);

				oss << '*';

				if (type->byref)
					oss << '&';

				if (format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED)
				{
					const Il2CppAssembly *ta = MetadataCache::GetAssemblyFromIndex (Class::FromIl2CppType (type->data.type)->image->assemblyIndex);
					oss << ", " << vm::AssemblyName::AssemblyNameToString(ta->aname);
				}
				break;
			}

		case IL2CPP_TYPE_VAR:
		case IL2CPP_TYPE_MVAR:
			oss << MetadataCache::GetStringFromIndex (Type::GetGenericParameter (type)->nameIndex);

			if (type->byref)
				oss << '&';
			break;

		default:
			{
				TypeInfo *klass = Class::FromIl2CppType (type);
				Class::Init (klass);

				TypeInfo* declaringType = Class::GetDeclaringType (klass);
				if (declaringType)
				{
					Type::GetNameInternal (oss, declaringType->byval_arg, format, true);
					oss << (format == IL2CPP_TYPE_NAME_FORMAT_IL ? '.' : '+');
				} else if (*klass->namespaze)
				{
					oss << klass->namespaze << '.';
				}

				if (format == IL2CPP_TYPE_NAME_FORMAT_IL)
				{
					const char *s = strchr (klass->name, '`');
					oss << (s ? std::string(klass->name, s) : klass->name);
				} else
					oss << klass->name;

				if (is_nested)
					break;

				if (klass->generic_class)
				{
					Il2CppGenericClass *gclass = klass->generic_class;
					const Il2CppGenericInst *inst = gclass->context.class_inst;
					Il2CppTypeNameFormat nested_format;
					
					nested_format = format == IL2CPP_TYPE_NAME_FORMAT_FULL_NAME ? IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED : format;

					oss << (format == IL2CPP_TYPE_NAME_FORMAT_IL ? '<' : '[');

					for (uint32_t i = 0; i < inst->type_argc; i++)
					{
						const Il2CppType *t = inst->type_argv [i];

						if (i) oss << ',';

						if ((nested_format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED) && (t->type != IL2CPP_TYPE_VAR) && (type->type != IL2CPP_TYPE_MVAR))
							oss << '[';

						Type::GetNameInternal (oss, inst->type_argv [i], nested_format, false);

						if ((nested_format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED) && (t->type != IL2CPP_TYPE_VAR) && (type->type != IL2CPP_TYPE_MVAR))
							oss << ']';
					}

					oss << (format == IL2CPP_TYPE_NAME_FORMAT_IL ? '>' : ']');
				}
				else if (Class::IsGeneric (klass) && (format != IL2CPP_TYPE_NAME_FORMAT_FULL_NAME) && (format != IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED))
				{
					const Il2CppGenericContainer* container = Class::GetGenericContainer (klass);

					oss << (format == IL2CPP_TYPE_NAME_FORMAT_IL ? '<' : '[');

					for (int32_t i = 0; i < container->type_argc; i++)
					{
						if (i) oss << ',';
						oss << MetadataCache::GetStringFromIndex (GenericContainer::GetGenericParameter (container, i)->nameIndex);
					}

					oss << (format == IL2CPP_TYPE_NAME_FORMAT_IL ? '>' : ']');
				}

				if (type->byref)
					oss << '&';

				if ((format == IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED) && (type->type != IL2CPP_TYPE_VAR) && (type->type != IL2CPP_TYPE_MVAR))
				{
					const Il2CppAssembly *ta = MetadataCache::GetAssemblyFromIndex (klass->image->assemblyIndex);
					oss << ", " << vm::AssemblyName::AssemblyNameToString(ta->aname);
				}
				break;
			}
	}
}

std::string Type::GetName (const Il2CppType *type, Il2CppTypeNameFormat format)
{
	std::ostringstream oss;
	Type::GetNameInternal (oss, type, format, false);
	return oss.str();
}

TypeInfo* Type::GetClassOrElementClass (const Il2CppType *type)
{
	// This is a weird function to mimic old mono behaviour.
	// We incorrectly used the analogous mono function in Unity.
	// Expectations: Return class if type is not an array. Return element type if it is an array.
	if (type->type == IL2CPP_TYPE_ARRAY)
		return Class::FromIl2CppType (type->data.array->etype);

	if (type->type == IL2CPP_TYPE_SZARRAY)
		return Class::FromIl2CppType (type->data.type);

	// IL2CPP_TYPE_SZARRAY stores element class in klass
	return MetadataCache::GetTypeInfoFromTypeDefinitionIndex (type->data.klassIndex);
}

const Il2CppType* Type::GetUnderlyingType (const Il2CppType *type)
{
	if (type->type == IL2CPP_TYPE_VALUETYPE && MetadataCache::GetTypeInfoFromTypeDefinitionIndex (type->data.klassIndex)->enumtype && !type->byref)
		return Class::GetEnumBaseType (MetadataCache::GetTypeInfoFromTypeDefinitionIndex (type->data.klassIndex));
	if (IsGenericInstance (type))
	{
		TypeInfo* definition = GenericClass::GetTypeDefinition (type->data.generic_class);
		if (definition != NULL && definition->enumtype && !type->byref)
			return Class::GetEnumBaseType (definition);
	}
	return type;
}

bool Type::IsGenericInstance (const Il2CppType* type)
{
	return type->type == IL2CPP_TYPE_GENERICINST;
}

uint32_t Type::GetToken (const Il2CppType *type)
{
	if (IsGenericInstance (type))
		return GenericClass::GetTypeDefinition (type->data.generic_class)->token;
	return GetClass (type)->token;
}

bool Type::IsReference (const Il2CppType* type)
{
	if (!type)
		return false;

	if (type->type == IL2CPP_TYPE_STRING ||
		type->type == IL2CPP_TYPE_SZARRAY ||
		type->type == IL2CPP_TYPE_CLASS ||
		type->type == IL2CPP_TYPE_OBJECT ||
		type->type == IL2CPP_TYPE_ARRAY)
		return true;

	if (IsGenericInstance(type) && !GenericClass::IsValueType (type->data.generic_class))
		return true;

	return false;
}

bool Type::IsStruct (const Il2CppType* type)
{
	if (type->byref)
		return false;

	if (type->type == IL2CPP_TYPE_VALUETYPE && !MetadataCache::GetTypeInfoFromTypeDefinitionIndex (type->data.klassIndex)->enumtype)
		return true;

	if (type->type == IL2CPP_TYPE_TYPEDBYREF)
		return true;

	if (IsGenericInstance (type) &&
		GenericClass::IsValueType (type->data.generic_class) &&
		!GenericClass::IsEnum (type->data.generic_class))
		return true;

	return false;
}

bool Type::GenericInstIsValuetype (const Il2CppType* type)
{
	assert (IsGenericInstance (type));
	return GenericClass::IsValueType (type->data.generic_class);
}

bool Type::IsEnum (const Il2CppType *type)
{
	if (type->type != IL2CPP_TYPE_VALUETYPE)
		return false;

	TypeInfo* klass = GetClass (type);
	return klass->enumtype;
}

bool Type::IsValueType (const Il2CppType *type)
{
	TypeInfo* klass = GetClass (type);
	return klass->valuetype;
}
bool Type::IsEmptyType (const Il2CppType *type)
{
	return IsGenericInstance (type) && type->data.generic_class->typeDefinitionIndex == kTypeIndexInvalid;
}

bool Type::IsSystemDBNull (const Il2CppType *type)
{
	TypeInfo* klass = GetClass (type);
	return (klass->image == il2cpp_defaults.corlib && strcmp (klass->namespaze, "System") == 0 && strcmp (klass->name, "DBNull") == 0);
}

bool Type::IsSystemDateTime (const Il2CppType *type)
{
	TypeInfo* klass = GetClass (type);
	return (klass->image == il2cpp_defaults.corlib && strcmp (klass->namespaze, "System") == 0 && strcmp (klass->name, "DateTime") == 0);
}

bool Type::IsSystemDecimal (const Il2CppType *type)
{
	TypeInfo* klass = GetClass (type);
	return (klass->image == il2cpp_defaults.corlib && strcmp (klass->namespaze, "System") == 0 && strcmp (klass->name, "Decimal") == 0);
}

TypeInfo* Type::GetClass (const Il2CppType *type)
{
	assert (type->type == IL2CPP_TYPE_CLASS || type->type == IL2CPP_TYPE_VALUETYPE);
	return MetadataCache::GetTypeInfoFromTypeDefinitionIndex (type->data.klassIndex);
}

const Il2CppGenericParameter* Type::GetGenericParameter (const Il2CppType *type)
{
	assert (type->type == IL2CPP_TYPE_VAR || type->type == IL2CPP_TYPE_MVAR);
	return MetadataCache::GetGenericParameterFromIndex (type->data.genericParameterIndex);
}

/**
* Type::ConstructDelegate:
* @delegate: pointer to an uninitialized delegate object
* @target: target object
* @addr: pointer to native code
* @method: method
*
* Initialize a delegate and set a specific method, not the one
* associated with addr. This is useful when sharing generic code.
* In that case addr will most probably not be associated with the
* correct instantiation of the method.
*/
void Type::ConstructDelegate (Il2CppDelegate* delegate, Il2CppObject* target, methodPointerType addr, const MethodInfo* method)
{
	assert(delegate);

	if (method)
		delegate->method = method;

	delegate->method_ptr = addr;
	if (target != NULL)
		IL2CPP_OBJECT_SETREF(delegate, target, target);

	delegate->invoke_impl = method->invoker_method; //TODO:figure out if this is needed at all
}

Il2CppString* Type::AppendAssemblyNameIfNecessary(Il2CppString* typeName, const char* assemblyName)
{
	if (typeName != NULL)
	{
		std::string name = utils::StringUtils::Utf16ToUtf8(String::GetChars(typeName));

		il2cpp::vm::TypeNameParseInfo info;
		il2cpp::vm::TypeNameParser parser(name, info, false);

		if (parser.Parse())
		{
			if (info.assembly_name().name.empty())
			{
				std::stringstream assemblyQualifiedName;
				assemblyQualifiedName << name << ", " << assemblyName;
				return vm::String::New(assemblyQualifiedName.str().c_str());
			}
		}
	}

	return typeName;
}

} /* namespace vm */
} /* namespace il2cpp */
